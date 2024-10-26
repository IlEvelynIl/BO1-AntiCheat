#include <SFML/Graphics.hpp>

#include "Constants.h"

#include "game/game_handler.hpp"

#include "game/game_integrity.hpp"

#include "statuses.h"

#include "utilities/verification.hpp"

#include "utilities/memory.hpp"

#include "utilities/updater.hpp"

using namespace std;

bool initialized = false;
bool performed_integrity_check = false;
bool cheats_detected = false;
bool notified_cheats_detected = false;

int last_map_id = 0;
int current_map_id = 0;

vector<string> scannable_maps;
vector<string> cheats_found;

string gameStatus = Statuses::GAME_NOT_CONNECTED;

static void init()
{
    scannable_maps.push_back("frontend");
    scannable_maps.push_back("common_zombie");
    scannable_maps.push_back("zombie_theater");
    scannable_maps.push_back("zombie_pentagon");
    scannable_maps.push_back("zombietron");
    scannable_maps.push_back("zombie_cosmodrome");
    scannable_maps.push_back("zombie_coast");
    scannable_maps.push_back("zombie_temple");
    scannable_maps.push_back("zombie_moon");
    scannable_maps.push_back("zombie_cod5_prototype");
    scannable_maps.push_back("zombie_cod5_asylum");
    scannable_maps.push_back("zombie_cod5_sumpf");
    scannable_maps.push_back("zombie_cod5_factory");
}

static void NotifyGameClosed()
{
    gameStatus = Statuses::GAME_NOT_CONNECTED;
    initialized = false;
    performed_integrity_check = false;
}

static void AddCheatsFound(string found_cheat)
{
    cheats_detected = true;
    cheats_found.push_back(found_cheat);
}

static void NotifyCheatsDetected()
{
    gameStatus = "A known cheating method was detected.";
    notified_cheats_detected = true;
    GameHandler gl;
    gl.CloseBlackOps();

    string cheats = "The following cheating methods were detected:\n";

    for (string cheat_found : cheats_found)
    {
        cheats += "\n- " + cheat_found;
    }
    MessageBoxA(NULL, cheats.c_str(), "BO1 Anti Cheat (Detections)", MB_OK);
}

static void OnGameReopen()
{
    notified_cheats_detected = false;
    if (cheats_detected)
    {
        cheats_detected = false;
    }
    initialized = false;
    cheats_found.clear();
}

static void AttemptIntegrityCheck()
{
    GameIntegrity gi;
    GameHandler gh;

    last_map_id = current_map_id;
    current_map_id = gh.GetMapId();

    bool map_id_changed = performed_integrity_check && last_map_id != 0 && current_map_id == 0;
    if (map_id_changed)
    {
        performed_integrity_check = false;
    }

    if (!performed_integrity_check)
    {
        bool last_map_id_valid = last_map_id != 0 && last_map_id != -1;
        if (last_map_id_valid && current_map_id == 0)
        {
            gameStatus = Statuses::PERFORMING_SCANS;
            performed_integrity_check = true;
            Sleep(1000);

            if (gi.IsStealthPatchDLLPresent())
            {
                AddCheatsFound("A known stealth patch DLL was injected.");
            }

            if (gh.IsGameModLoaded() && gh.IsModLoaded())
            {
                if (!gi.IsModFileValid())
                {
                    AddCheatsFound("mod.ff was found to be modified.");
                }
            }

            for (string map : scannable_maps)
            {
                if (!gi.IsFastfilePatchValid(map))
                {
                    AddCheatsFound(map + "_patch.ff was found to be modified.");
                }
            }

            if (gi.DoExtraFilesExist())
            {
                AddCheatsFound("Extra files found in zone/Common, could be a stealth patch.");
            }

            if (cheats_detected)
            {
                NotifyCheatsDetected();
                return;
            }

            gameStatus = Statuses::NO_CHEATING_DETECTED;
        }
    }
    
    if (gh.GetMapId() == Constants::MAIN_MENU_ID)
    {
        gameStatus = Statuses::WAITING_FOR_MAP_LOAD;
        performed_integrity_check = false;
    }
}

static void CheckForBlackOpsProcess()
{
    GameHandler gh;

    if (!gh.IsGameOpen() && !cheats_detected)
    {
        NotifyGameClosed();
        gameStatus = Statuses::GAME_NOT_CONNECTED;
        gh.CheckGameMod();
        return;
    }

    if (notified_cheats_detected && gh.IsGameOpen())
    {
        OnGameReopen();
    }

    if (!initialized)
    {
        if (gh.GetMapId() != Constants::MAIN_MENU_ID)
        {
            gameStatus = Statuses::GAME_CONNECTED;
        }

        gh.CheckGameMod();
        initialized = true;
    }

    AttemptIntegrityCheck();
    Sleep(50);
}

#include <thread>
#include <atomic>

std::atomic<bool> processThreadRunning(true);

static void CheckForBlackOpsProcessThread()
{
    while (processThreadRunning)
    {
        CheckForBlackOpsProcess();
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    init();

    Verification v;
    v.init();

    GameIntegrity gi;
    gi.init();

    GameHandler gh;

    Updater updater;
    updater.CheckForUpdates();

    int windowWidth = 650;
    int windowHeight = 215;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Image icon;
    if (!icon.loadFromFile("main/icon.png"))
    {
        MessageBox(NULL, L"Could not load \"icon.png\".", L"Error", MB_OK);
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Fonts
    sf::Font boldFont, semiBoldFont;
    if (!boldFont.loadFromFile("main/Bold.ttf") || !semiBoldFont.loadFromFile("main/SemiBold.ttf"))
    {
        MessageBox(NULL, L"Could not load \"Bold.ttf\" or \"SemiBold.ttf\".", L"Error", MB_OK);
        return -1;
    }

    // "BO1 Anti Cheat"
    sf::Text anticheatText(Constants::TITLE, semiBoldFont, 20);
    anticheatText.setFillColor(sf::Color(255, 255, 255, 155));

    int padding = 20;
    float anticheatTextWidth = anticheatText.getGlobalBounds().width;
    anticheatText.setPosition(((windowWidth - anticheatTextWidth) / 2) + padding, 20);

    // Black Ops Icon
    sf::Texture blackOpsLogo;
    if (!blackOpsLogo.loadFromFile("main/bo1.png")) {
        MessageBox(NULL, L"Could not load \"bo1.png\".", L"Error", MB_OK);
        return -1;
    }
    blackOpsLogo.setSmooth(true);
    sf::Sprite bo1Sprite(blackOpsLogo);
    bo1Sprite.setPosition(anticheatText.getGlobalBounds().left - (padding + 19), 18);

    // status text
    sf::Text statusText("", semiBoldFont, 25);
    statusText.setFillColor(sf::Color::White);

    // uids background
    sf::RectangleShape uidsBackground(sf::Vector2f(615, 100));
    uidsBackground.setFillColor(sf::Color(0, 0, 0, 125));

    float backgroundWidth = uidsBackground.getGlobalBounds().width;
    uidsBackground.setPosition((windowWidth - backgroundWidth) / 2, 100);

    // "verification uids:"
    sf::Text verificationText("Verification UIDs:", semiBoldFont, 20);
    verificationText.setFillColor(sf::Color::White);

    float verificationTextWidth = verificationText.getGlobalBounds().width;
    verificationText.setPosition((windowWidth - verificationTextWidth) / 2, 109);

    // uid #1
    sf::Text uid1Text(v.GetUID_1(), boldFont, 20);
    uid1Text.setFillColor(sf::Color::White);

    float uid1TextWidth = uid1Text.getGlobalBounds().width;
    uid1Text.setPosition((windowWidth - uid1TextWidth) / 2, 140);

    // uid #2
    sf::Text uid2Text(v.GetUID_2(), boldFont, 20);
    uid2Text.setFillColor(sf::Color::White);

    float uid2TextWidth = uid2Text.getGlobalBounds().width;
    uid2Text.setPosition((windowWidth - uid2TextWidth) / 2, 165);

    thread checkProcessThread(CheckForBlackOpsProcessThread);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        statusText.setString(gameStatus);
        float statusTextWidth = statusText.getGlobalBounds().width;
        statusText.setPosition((windowWidth - statusTextWidth) / 2, 59);

        if (cheats_detected)
        {
            statusText.setFillColor(sf::Color(255, 58, 58));
        }
        else
        {
            statusText.setFillColor(sf::Color::White);
        }

        window.clear(sf::Color(10, 10, 10));
        window.draw(anticheatText);
        window.draw(bo1Sprite);
        window.draw(statusText);

        window.draw(uidsBackground);
        window.draw(verificationText);
        window.draw(uid1Text);
        window.draw(uid2Text);
        window.display();
    }

    processThreadRunning = false;
    checkProcessThread.join();

    return 0;
}
