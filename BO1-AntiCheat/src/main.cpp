#include <SFML/Graphics.hpp>

#include "anticheat/anticheat.hpp"

#include "anticheat/updater/updater.hpp"

#include "anticheat/settings/settings.hpp"

#include "anticheat/resources/bo1_png_data.h"

#include "anticheat/resources/icon_png_data.h"

#include "anticheat/resources/bold_font_data.h"

#include "anticheat/resources/semibold_font_data.h"

#include "utils/strings.hpp"

#include "constants.h"

#include <windows.h>

#include <thread>

#include <atomic>

#include <sstream>

#include <iostream>

#include <chrono>

#include <winternl.h>

#include <intrin.h>

bool HasDebugRegisters()
{
    CONTEXT ctx = {};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (GetThreadContext(GetCurrentThread(), &ctx))
    {
        return (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3);
    }
    return false;
}

bool CheckPEBFlags()
{
#ifdef _M_X64
    PEB* peb = (PEB*)__readgsqword(0x60);
#elif _M_IX86
    PEB* peb = (PEB*)__readfsdword(0x30);
#else
#error Unsupported architecture
#endif

    bool beingDebugged = peb->BeingDebugged != 0;
    ULONG ntGlobalFlag = *(ULONG*)((BYTE*)peb + 0xBC);
    return beingDebugged || (ntGlobalFlag & 0x70);
}

std::atomic<bool> processThreadRunning(true);
std::atomic<bool> debuggerThreadRunning(true);

// we put the process function on a new thread to prevent it from being on the main thread
static void CheckForBlackOpsProcessThread()
{
    while (processThreadRunning)
    {
        anticheat::WaitForBlackOpsProcess();
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

static void CheckForDebugger()
{
    while (debuggerThreadRunning)
    {
        // check for debugger
        if (IsDebuggerPresent())
        {
            ExitProcess(1);
        }

        // check for remote debugger
        BOOL isDebuggerAttached = FALSE;
        if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerAttached) && isDebuggerAttached)
        {
            ExitProcess(1);
        }

        // check for hardware breakpoints via debug registers
        if (HasDebugRegisters())
        {
            ExitProcess(1);
        }

        // check PEB flags for debugging indicators
        if (CheckPEBFlags())
        {
            ExitProcess(1);
        }

        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// since \n doesnt work on text with sfml, we need to make our own function for it
std::string WrapText(std::string text, sf::Font font, int fontSize, float maxWidth) {
    std::istringstream wordStream(text);
    std::string word;
    std::string wrappedText;
    std::string currentLine;

    sf::Text tempText("", font, fontSize);

    while (std::getline(wordStream, word, ' ')) {
        if (word.find('\n') != std::string::npos) {
            size_t pos;
            while ((pos = word.find('\n')) != std::string::npos) {
                std::string beforeNewline = word.substr(0, pos);
                if (!beforeNewline.empty()) {
                    std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + beforeNewline;
                    tempText.setString(testLine);

                    if (tempText.getGlobalBounds().width > maxWidth) {
                        wrappedText += currentLine + "\n";
                        currentLine = beforeNewline;
                    }
                    else {
                        currentLine = testLine;
                    }
                }

                wrappedText += currentLine + "\n";
                currentLine.clear();
                word.erase(0, pos + 1);
            }

            if (!word.empty()) {
                currentLine = word;
            }
        }
        else {
            std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
            tempText.setString(testLine);

            if (tempText.getGlobalBounds().width > maxWidth) {
                wrappedText += currentLine + "\n";
                currentLine = word;
            }
            else {
                currentLine = testLine;
            }
        }
    }

    if (!currentLine.empty()) {
        wrappedText += currentLine;
    }

    return wrappedText;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main()
{
    // setup anti cheat
    anticheat::Initialize();

    // load the settings
    anticheat::settings::LoadSettings();

    // check for any debugger tools
    thread checkDebuggerThread(CheckForDebugger);

    int main_window_width = 650;
    int main_window_height = 155;

    // attempt to load the main_window icon, this can be changed by players if they want
    sf::Image icon;
    if (!icon.loadFromMemory(anticheat::resources::icon_png, anticheat::resources::icon_png_len))
    {
        MessageBoxA(NULL, "Could not load asset \"icon_png\".", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
        return -1;
    }

    // load fonts
    sf::Font boldFont, semi_bold_font;
    if (!boldFont.loadFromMemory(anticheat::resources::bold_ttf, anticheat::resources::bold_ttf_len)
        || !semi_bold_font.loadFromMemory(anticheat::resources::semi_bold_ttf, anticheat::resources::semi_bold_ttf_len))
    {
        MessageBoxA(NULL, "Could not load asset \"bold_ttf\" or \"semi_bold_ttf\".", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
        return -1;
    }

    // BO1 image
    sf::Texture blackOpsLogo;
    if (!blackOpsLogo.loadFromMemory(anticheat::resources::bo1_png, anticheat::resources::bo1_png_len)) {
        MessageBoxA(NULL, "Could not load asset \"bo1_png\".", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
        return -1;
    }
    blackOpsLogo.setSmooth(true);

    // "BO1 Anti Cheat v(version number)" text
    sf::Text anticheatText("BO1 Anti Cheat v" + Constants::VERSION, semi_bold_font, 21);
    anticheatText.setFillColor(sf::Color(255, 255, 255, 155));

    int padding = 20;
    float anticheatTextWidth = anticheatText.getGlobalBounds().width;
    anticheatText.setPosition(((main_window_width - anticheatTextWidth) / 2) + padding, 21);

    // bo1 image
    sf::Sprite bo1Sprite(blackOpsLogo);
    bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

    // status msg text
    sf::Text statusText("", semi_bold_font, 25);
    statusText.setFillColor(sf::Color::White);

    // second status text
    sf::Text extraStatusText("", semi_bold_font, 15);
    extraStatusText.setFillColor(sf::Color(255, 255, 255, 155));

    // watermark text
    sf::Text watermarkText("Created with <3 by IlEvelynIl", semi_bold_font, 15);
    watermarkText.setFillColor(sf::Color::White);
    float watermarkTextWidth = watermarkText.getGlobalBounds().width;
    watermarkText.setPosition((main_window_width - watermarkText.getGlobalBounds().width) / 2, 120);

    // updates window
    std::string available_updates = anticheat::updater::GetAvailableUpdates();
    bool updates_window_closed = false;
    if (!available_updates.empty())
    {
        int updates_window_width = 650;
        int updates_window_height = 350;
        std::string wrapped_changes = WrapText(available_updates, semi_bold_font, 15, updates_window_width - 30);

        sf::RenderWindow updates_window(sf::VideoMode(updates_window_width, updates_window_height), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
        updates_window.setFramerateLimit(10);
        updates_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        // header
        sf::Text updates_header("New Update Available!", semi_bold_font, 25);
        updates_header.setFillColor(sf::Color::White);
        updates_header.setPosition((main_window_width - updates_header.getGlobalBounds().width) / 2, 55);

        // what changed
        sf::Text changes_text(wrapped_changes, semi_bold_font, 15);
        changes_text.setFillColor(sf::Color::White);
        changes_text.setPosition((updates_window_width - changes_text.getGlobalBounds().width) / 2, 120);

        // "Update" button
        sf::RectangleShape updateButton(sf::Vector2f(100, 35));
        updateButton.setOutlineThickness(2);
        updateButton.setOutlineColor(sf::Color::White);
        updateButton.setFillColor(sf::Color::Transparent);
        updateButton.setPosition((updates_window_width - 240) / 2, updates_window_height - 60);

        sf::Text updateButtonText("Update", semi_bold_font, 18);
        updateButtonText.setFillColor(sf::Color::White);
        float updateButtonTextWidth = updateButtonText.getGlobalBounds().width;
        float updateButtonTextHeight = updateButtonText.getGlobalBounds().height;
        updateButtonText.setPosition(
            updateButton.getPosition().x + (updateButton.getSize().x - updateButtonTextWidth) / 2,
            updateButton.getPosition().y + (updateButton.getSize().y - updateButtonTextHeight) / 2 - 5
        );

        // "Ignore" button
        sf::RectangleShape ignoreUpdateButton(sf::Vector2f(100, 35));
        ignoreUpdateButton.setOutlineThickness(2);
        ignoreUpdateButton.setOutlineColor(sf::Color::White);
        ignoreUpdateButton.setFillColor(sf::Color::Transparent);
        ignoreUpdateButton.setPosition((updates_window_width + 40) / 2, updates_window_height - 60);

        sf::Text ignoreButtonText("Ignore", semi_bold_font, 18);
        ignoreButtonText.setFillColor(sf::Color::White);
        float ignoreButtonTextWidth = ignoreButtonText.getGlobalBounds().width;
        float ignoreButtonTextHeight = ignoreButtonText.getGlobalBounds().height;
        ignoreButtonText.setPosition(
            ignoreUpdateButton.getPosition().x + (ignoreUpdateButton.getSize().x - ignoreButtonTextWidth) / 2,
            ignoreUpdateButton.getPosition().y + (ignoreUpdateButton.getSize().y - ignoreButtonTextHeight) / 2 - 5
        );

        while (updates_window.isOpen())
        {
            sf::Event event;
            while (updates_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    updates_window.close();
                    updates_window_closed = true;
                }

                // Check for mouse click on the Yes button
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (updateButton.getGlobalBounds().contains(mousePos))
                    {
                        ShellExecute(NULL, L"open", L"https://github.com/IlEvelynIl/BO1-AntiCheat/releases/latest", NULL, NULL, SW_SHOWNORMAL);
                        debuggerThreadRunning = false;
                        checkDebuggerThread.join();
                        anticheat::settings::SaveSettings(false);
                        return 0;
                    }

                    // Check for mouse click on the No button
                    if (ignoreUpdateButton.getGlobalBounds().contains(mousePos))
                    {
                        updates_window.close();
                    }
                }
            }

            // hover effect for the ok button
            sf::Vector2i mousePosition = sf::Mouse::getPosition(updates_window);
            if (updateButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                updateButton.setFillColor(sf::Color(255, 255, 255));
                updateButtonText.setFillColor(sf::Color::Black);
            }
            else {
                updateButton.setFillColor(sf::Color::Transparent);
                updateButtonText.setFillColor(sf::Color::White);
            }

            if (ignoreUpdateButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                ignoreUpdateButton.setFillColor(sf::Color(255, 255, 255));
                ignoreButtonText.setFillColor(sf::Color::Black);
            }
            else {
                ignoreUpdateButton.setFillColor(sf::Color::Transparent);
                ignoreButtonText.setFillColor(sf::Color::White);
            }

            anticheatText.setPosition(((updates_window_width - anticheatTextWidth) / 2) + padding, 21);
            bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

            updates_window.clear(sf::Color(5, 5, 5));
            updates_window.draw(anticheatText);
            updates_window.draw(bo1Sprite);
            updates_window.draw(updates_header);
            updates_window.draw(changes_text);
            updates_window.draw(updateButton);
            updates_window.draw(updateButtonText);
            updates_window.draw(ignoreUpdateButton);
            updates_window.draw(ignoreButtonText);
            updates_window.display();
        }
    }

    // make sure players are aware of the guidelines with the tool
    bool guidelines_window_closed = false;
    std::string display_guidelines = anticheat::settings::GetValue("DisplayGuidelines");
    if (utils::strings::ToLower(display_guidelines) == "true")
    {
        int guidelines_window_width = 650;
        int guidelines_window_height = 350;

        std::string guidelines = "Before using this tool, be sure to follow these guidelines, or your game could be dismissed:\n\n" + Constants::GUIDELINES + "\n\nYou can stop this from appearing by editing the config file.";
        std::string wrappedguidelines = WrapText(guidelines, semi_bold_font, 15, guidelines_window_width - 30);

        sf::RenderWindow guidelines_window(sf::VideoMode(guidelines_window_width, guidelines_window_height), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
        guidelines_window.setFramerateLimit(10);
        guidelines_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        // header
        sf::Text guidelinesHeader("Anti Cheat Guidelines", semi_bold_font, 25);
        guidelinesHeader.setFillColor(sf::Color::White);

        float guidelinesHeaderWidth = guidelinesHeader.getGlobalBounds().width;
        guidelinesHeader.setPosition((main_window_width - guidelinesHeaderWidth) / 2, 55);

        // guidelines text
        sf::Text guidelinesText(wrappedguidelines, semi_bold_font, 15);
        guidelinesText.setFillColor(sf::Color::White);

        float guidelinesTextWidth = guidelinesText.getGlobalBounds().width;
        guidelinesText.setPosition((guidelines_window_width - guidelinesTextWidth) / 2, 120);

        // OK button setup with outline and hover effect
        sf::RectangleShape okButton(sf::Vector2f(100, 35));
        okButton.setOutlineThickness(2);
        okButton.setOutlineColor(sf::Color::White); // outline color
        okButton.setFillColor(sf::Color::Transparent); // initially transparent
        okButton.setPosition((guidelines_window_width - okButton.getSize().x) / 2, guidelines_window_height - 60);

        // ok button text
        sf::Text okText("OK", semi_bold_font, 18);
        okText.setFillColor(sf::Color::White);
        float okTextWidth = okText.getGlobalBounds().width;
        float okTextHeight = okText.getGlobalBounds().height;
        okText.setPosition(
            okButton.getPosition().x + (okButton.getSize().x - okTextWidth) / 2,
            okButton.getPosition().y + (okButton.getSize().y - okTextHeight) / 2 - 5
        );

        while (guidelines_window.isOpen())
        {
            sf::Event event;
            while (guidelines_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    guidelines_window.close();
                    guidelines_window_closed = true;
                }

                // Check for mouse click on the OK button
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (okButton.getGlobalBounds().contains(mousePos))
                    {
                        guidelines_window.close();
                    }
                }
            }

            // hover effect for the ok button
            sf::Vector2i mousePosition = sf::Mouse::getPosition(guidelines_window);
            if (okButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                okButton.setFillColor(sf::Color(255, 255, 255));
                okText.setFillColor(sf::Color::Black);
            }
            else {
                okButton.setFillColor(sf::Color::Transparent);
                okText.setFillColor(sf::Color::White);
            }

            anticheatText.setPosition(((guidelines_window_width - anticheatTextWidth) / 2) + padding, 21);
            bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

            guidelines_window.clear(sf::Color(5, 5, 5));
            guidelines_window.draw(anticheatText);
            guidelines_window.draw(bo1Sprite);
            guidelines_window.draw(guidelinesHeader);
            guidelines_window.draw(guidelinesText);
            guidelines_window.draw(okButton);
            guidelines_window.draw(okText);
            guidelines_window.display();
        }
    }

    // if the guidelines window was closed, they must wanna close the whole program
    if (guidelines_window_closed)
    {
        debuggerThreadRunning = false;
        checkDebuggerThread.join();
        anticheat::settings::SaveSettings(false);
        return 0;
    }

    // start the game process thread
    thread checkProcessThread(CheckForBlackOpsProcessThread);

    anticheatText.setPosition(((main_window_width - anticheatTextWidth) / 2) + padding, 21);

    // create the main_window
    sf::RenderWindow main_window(sf::VideoMode(main_window_width, main_window_height), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
    main_window.setFramerateLimit(10);

    // remove minimize button
    HWND hwnd = main_window.getSystemHandle();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_MINIMIZEBOX;
    SetWindowLong(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // set the icon
    main_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    while (main_window.isOpen())
    {
        // wait for the close button to be pressed
        sf::Event event;
        while (main_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                main_window.close();
            }
        }

        anticheatText.setPosition(((main_window_width - anticheatTextWidth) / 2) + padding, 21);
        bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

        statusText.setString(anticheat::GetMainStatus()); // constantly set the status text
        float statusTextWidth = statusText.getGlobalBounds().width;
        statusText.setPosition((main_window_width - statusTextWidth) / 2, 57); // adjust position based on text width and main_window width

        // if cheats have been detected then set it to a red/pink color
        if (anticheat::CheatingDetected())
        {
            statusText.setFillColor(sf::Color(255, 58, 58));
        }
        else { // or we just set it back to white
            statusText.setFillColor(sf::Color::White);
        }

        // set the extra status text
        extraStatusText.setString(anticheat::GetInfoStatus());
        float extraStatusTextWidth = extraStatusText.getGlobalBounds().width;
        extraStatusText.setPosition((main_window_width - extraStatusTextWidth) / 2, 90);

        // draw everything to the main_window
        main_window.clear(sf::Color(5, 5, 5));
        main_window.draw(anticheatText);
        main_window.draw(bo1Sprite);
        main_window.draw(statusText);
        main_window.draw(extraStatusText);
        main_window.draw(watermarkText);
        main_window.display();
    }

    // when the main_window is closed, stop the game process thread from running
    processThreadRunning = false;
    debuggerThreadRunning = false;
    checkProcessThread.join();
    checkDebuggerThread.join();
    anticheat::settings::SaveSettings(false);

    return 0;
}