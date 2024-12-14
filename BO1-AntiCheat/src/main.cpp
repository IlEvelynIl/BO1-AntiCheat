#include <SFML/Graphics.hpp>

#include <windows.h>

#include "anticheat/anticheat.hpp"

#include "anticheat/updater/updater.hpp"

#include "anticheat/verification/verification.hpp"

#include "constants.h"

#include <thread>
#include <atomic>

std::atomic<bool> processThreadRunning(true);

// we put the process function on a new thread to prevent it from being on the main thread
static void CheckForBlackOpsProcessThread()
{
    while (processThreadRunning)
    {
        anticheat::WaitForBlackOpsProcess();
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main()
{
    // setup anti cheat
    anticheat::Initialize();

    // when the tool is opened, check for updates
    anticheat::updater::CheckForUpdates();

    int windowWidth = 650;
    int windowHeight = 230;

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(10);

    // remove minimize button
    HWND hwnd = window.getSystemHandle();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_MINIMIZEBOX;
    SetWindowLong(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // attempt to load the window icon, this can be changed by players if they want
    sf::Image icon;
    if (!icon.loadFromFile("main/icon.png"))
    {
        MessageBox(NULL, L"Could not load \"icon.png\".", L"Error", MB_OK);
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // load fonts
    sf::Font boldFont, semiBoldFont;
    if (!boldFont.loadFromFile("main/Bold.ttf") || !semiBoldFont.loadFromFile("main/SemiBold.ttf"))
    {
        MessageBox(NULL, L"Could not load \"Bold.ttf\" or \"SemiBold.ttf\".", L"Error", MB_OK);
        return -1;
    }

    // "BO1 Anti Cheat v(version number)" text
    sf::Text anticheatText("BO1 Anti Cheat v" + Constants::VERSION, semiBoldFont, 20);
    anticheatText.setFillColor(sf::Color(255, 255, 255, 155));

    int padding = 20;
    float anticheatTextWidth = anticheatText.getGlobalBounds().width;
    anticheatText.setPosition(((windowWidth - anticheatTextWidth) / 2) + padding, 20);

    // BO1 image
    sf::Texture blackOpsLogo;
    if (!blackOpsLogo.loadFromFile("main/bo1.png")) {
        MessageBox(NULL, L"Could not load \"bo1.png\".", L"Error", MB_OK);
        return -1;
    }
    blackOpsLogo.setSmooth(true);
    sf::Sprite bo1Sprite(blackOpsLogo);
    bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

    // status msg text
    sf::Text statusText("", semiBoldFont, 25);
    statusText.setFillColor(sf::Color::White);

    // "will continue to search for other methods of cheating" text
    sf::Text extraStatusText(Constants::WILL_CONTINUE_SEARCH, semiBoldFont, 15);
    extraStatusText.setFillColor(sf::Color(255, 255, 255, 155));

    // uids background
    sf::RectangleShape uidsBackground(sf::Vector2f(615, 100));
    uidsBackground.setFillColor(sf::Color(0, 0, 0, 125));

    float backgroundWidth = uidsBackground.getGlobalBounds().width;
    uidsBackground.setPosition((windowWidth - backgroundWidth) / 2, 115);

    // "verification uids:" text
    sf::Text verificationText("Verification UIDs", semiBoldFont, 20);
    verificationText.setFillColor(sf::Color::White);

    float verificationTextWidth = verificationText.getGlobalBounds().width;
    verificationText.setPosition((windowWidth - verificationTextWidth) / 2, 109 + 15);

    // uid #1 text
    sf::Text uid1Text(anticheat::verification::GetUID_1(), boldFont, 20);
    uid1Text.setFillColor(sf::Color::White);

    float uid1TextWidth = uid1Text.getGlobalBounds().width;
    uid1Text.setPosition((windowWidth - uid1TextWidth) / 2, 140 + 15);

    // uid #2 text
    sf::Text uid2Text(anticheat::verification::GetUID_2(), boldFont, 20);
    uid2Text.setFillColor(sf::Color::White);

    float uid2TextWidth = uid2Text.getGlobalBounds().width;
    uid2Text.setPosition((windowWidth - uid2TextWidth) / 2, 165 + 15);

    // start the game process thread
    thread checkProcessThread(CheckForBlackOpsProcessThread);

    while (window.isOpen())
    {
        // wait for the close button to be pressed
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        statusText.setString(anticheat::GetMainStatus()); // constantly set the status text
        float statusTextWidth = statusText.getGlobalBounds().width;
        statusText.setPosition((windowWidth - statusTextWidth) / 2, 57); // adjust position based on text width and window width

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
        extraStatusText.setPosition((windowWidth - extraStatusTextWidth) / 2, 90);

        // uid #1 text
        uid1Text.setString(anticheat::verification::GetUID_1());
        float uid1TextWidth = uid1Text.getGlobalBounds().width;
        uid1Text.setPosition((windowWidth - uid1TextWidth) / 2, 140 + 15);

        // uid #2 text
        uid2Text.setString(anticheat::verification::GetUID_2());
        float uid2TextWidth = uid2Text.getGlobalBounds().width;
        uid2Text.setPosition((windowWidth - uid2TextWidth) / 2, 165 + 15);

        // draw everything to the window
        window.clear(sf::Color(10, 10, 10));
        window.draw(anticheatText);
        window.draw(bo1Sprite);
        window.draw(statusText);
        window.draw(extraStatusText);
        window.draw(uidsBackground);
        window.draw(verificationText);
        window.draw(uid1Text);
        window.draw(uid2Text);
        window.display();
    }

    // when the window is closed, stop the game process thread from running
    processThreadRunning = false;
    checkProcessThread.join();

    return 0;
}