#include <SFML/Graphics.hpp>

#include "anticheat/anticheat.hpp"

#include "anticheat/updater/updater.hpp"

#include "anticheat/settings/settings.hpp"

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

    // when the tool is opened, check for updates
    anticheat::updater::CheckForUpdates();

    // load the settings
    anticheat::settings::LoadSettings();

    // check for any debugger tools
    thread checkDebuggerThread(CheckForDebugger);

    int main_window_width = 650;
    int main_window_height = 155;

    // attempt to load the main_window icon, this can be changed by players if they want
    sf::Image icon;
    if (!icon.loadFromFile("main/icon.png"))
    {
        MessageBoxA(NULL, "Could not load \"icon.png\".", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
        return -1;
    }

    // load fonts
    sf::Font boldFont, semiBoldFont;
    if (!boldFont.loadFromFile("main/Bold.ttf") || !semiBoldFont.loadFromFile("main/SemiBold.ttf"))
    {
        MessageBoxA(NULL, "Could not load \"Bold.ttf\" or \"SemiBold.ttf\".", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
        return -1;
    }

    // "BO1 Anti Cheat v(version number)" text
    sf::Text anticheatText("BO1 Anti Cheat v" + Constants::VERSION, semiBoldFont, 21);
    anticheatText.setFillColor(sf::Color(255, 255, 255, 155));

    int padding = 20;
    float anticheatTextWidth = anticheatText.getGlobalBounds().width;
    anticheatText.setPosition(((main_window_width - anticheatTextWidth) / 2) + padding, 21);

    // BO1 image
    sf::Texture blackOpsLogo;
    if (!blackOpsLogo.loadFromFile("main/bo1.png")) {
        MessageBoxA(NULL, "Could not load \"bo1.png\".", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
        return -1;
    }
    blackOpsLogo.setSmooth(true);
    sf::Sprite bo1Sprite(blackOpsLogo);
    bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

    // status msg text
    sf::Text statusText("", semiBoldFont, 25);
    statusText.setFillColor(sf::Color::White);

    // second status text
    sf::Text extraStatusText("", semiBoldFont, 15);
    extraStatusText.setFillColor(sf::Color(255, 255, 255, 155));

    // watermark text
    sf::Text watermarkText("Created with <3 by IlEvelynIl", semiBoldFont, 15);
    watermarkText.setFillColor(sf::Color::White);
    float watermarkTextWidth = watermarkText.getGlobalBounds().width;
    watermarkText.setPosition((main_window_width - watermarkText.getGlobalBounds().width) / 2, 120);

    // make sure players are aware of the rules with the tool
    bool rules_window_closed = false;
    std::string display_rules = anticheat::settings::GetValue("DisplayRules");
    if (utils::strings::ToLower(display_rules) == "true")
    {
        int rules_window_width = 650;
        int rules_window_height = 325;

        std::string rules = "Before using this tool, be sure to follow these guidelines, or your game could be dismissed:\n\n" + Constants::RULES;
        std::string wrappedRules = WrapText(rules, semiBoldFont, 15, rules_window_width - 30);

        sf::RenderWindow rules_window(sf::VideoMode(rules_window_width, rules_window_height), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
        rules_window.setFramerateLimit(10);
        rules_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        // header
        sf::Text rulesHeader("Anti Cheat Guidelines", semiBoldFont, 25);
        rulesHeader.setFillColor(sf::Color::White);

        float rulesHeaderWidth = rulesHeader.getGlobalBounds().width;
        rulesHeader.setPosition((main_window_width - rulesHeaderWidth) / 2, 55);

        // rules text
        sf::Text rulesText(wrappedRules, semiBoldFont, 15);
        rulesText.setFillColor(sf::Color::White);

        float rulesTextWidth = rulesText.getGlobalBounds().width;
        rulesText.setPosition((rules_window_width - rulesTextWidth) / 2, 120);

        // OK button setup with outline and hover effect
        sf::RectangleShape okButton(sf::Vector2f(100, 35));
        okButton.setOutlineThickness(2);
        okButton.setOutlineColor(sf::Color::White); // outline color
        okButton.setFillColor(sf::Color::Transparent); // initially transparent
        okButton.setPosition((rules_window_width - okButton.getSize().x) / 2, rules_window_height - 60);

        // ok button text
        sf::Text okText("OK", semiBoldFont, 18);
        okText.setFillColor(sf::Color::White);
        float okTextWidth = okText.getGlobalBounds().width;
        float okTextHeight = okText.getGlobalBounds().height;
        okText.setPosition(
            okButton.getPosition().x + (okButton.getSize().x - okTextWidth) / 2,
            okButton.getPosition().y + (okButton.getSize().y - okTextHeight) / 2 - 5
        );

        while (rules_window.isOpen())
        {
            sf::Event event;
            while (rules_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    rules_window.close();
                    rules_window_closed = true;
                }

                // Check for mouse click on the OK button
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (okButton.getGlobalBounds().contains(mousePos))
                    {
                        rules_window.close();
                    }
                }
            }

            // hover effect for the ok button
            sf::Vector2i mousePosition = sf::Mouse::getPosition(rules_window);
            if (okButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                okButton.setFillColor(sf::Color(255, 255, 255));
                okText.setFillColor(sf::Color::Black);
            }
            else {
                okButton.setFillColor(sf::Color::Transparent);
                okText.setFillColor(sf::Color::White);
            }

            anticheatText.setPosition(((rules_window_width - anticheatTextWidth) / 2) + padding, 21);
            bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

            rules_window.clear(sf::Color(5, 5, 5));
            rules_window.draw(anticheatText);
            rules_window.draw(bo1Sprite);
            rules_window.draw(rulesHeader);
            rules_window.draw(rulesText);
            rules_window.draw(okButton);
            rules_window.draw(okText);
            rules_window.display();
        }
    }

    // if the rules window was closed, they must wanna close the whole program
    if (rules_window_closed)
    {
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