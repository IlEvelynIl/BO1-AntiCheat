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
bool integrity_check_override = false;

int last_map_id = 0;
int current_map_id = 0;

vector<string> scannable_maps;
vector<string> cheats_found;

string gameStatus = Statuses::GAME_NOT_CONNECTED;
string extraStatus = "";

// adds the maps that need to be scanned every map launch/quit
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

// displays the "Game not connected." message
static void NotifyGameClosed()
{
    gameStatus = Statuses::GAME_NOT_CONNECTED;
    extraStatus = "";
    initialized = false;
    performed_integrity_check = false;
}

// adds a cheating method to a list, this will be shown in a second window
static void AddCheatsFound(string found_cheat)
{
    cheats_detected = true;
    cheats_found.push_back(found_cheat);
}

// crashes bo1 when a cheating method is found,
// also pops up the second window that explains what was found
// this is especially good for players who may accidentally leave something in their files
static void NotifyCheatsDetected()
{
    gameStatus = Statuses::CHEATING_DETECTED;
    extraStatus = Statuses::MORE_INFO_WINDOW;
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

// specifically makes it so that we dont have to restart the anti cheat when cheats were previously detected
static void OnGameReopen()
{
    notified_cheats_detected = false;
    if (cheats_detected)
    {
        cheats_detected = false;
    }
    initialized = false;
    cheats_found.clear();
    extraStatus = "";
}

// this is the magic of the tool, handles all checks performed to ensure a bo1 game is safe
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

    // only check when the map is being loaded/quit
    if (!performed_integrity_check)
    {
        bool last_map_id_valid = last_map_id != 0 && last_map_id != -1;
        if ((last_map_id_valid && current_map_id == 0) || integrity_check_override)
        {
            gameStatus = Statuses::CHECKING_FOR_PATCHES;
            extraStatus = "This may take a moment";

            integrity_check_override = false;
            performed_integrity_check = true;
            Sleep(1000);

            // check for any known stealth patch injections
            if (gi.IsStealthPatchDLLPresent())
            {
                AddCheatsFound("A known stealth patch DLL was injected.");
            }

            // community leaderboard mods: check hashes of the currently loaded mod
            if (gh.IsGameModLoaded() && gh.IsModLoaded())
            {
                if (!gi.IsModFileValid())
                {
                    AddCheatsFound("mod.ff was found to be modified.");
                }
            }

            // check every single fastfile patches
            for (string map : scannable_maps)
            {
                if (!gi.IsFastfilePatchValid(map))
                {
                    AddCheatsFound(map + "_patch.ff was found to be modified.");
                }
            }

            // check for any extra files, they should not be there
            if (gi.DoExtraFilesExist())
            {
                AddCheatsFound("Extra files found in zone/Common, could be a stealth patch.");
            }

            // if theres any cheats detected, notify them and crash bo1
            if (cheats_detected)
            {
                NotifyCheatsDetected();
            }
            else // otherwise tell them they're good, but we will also tell them that the anti cheat is still gonna check during the game
            {
                gameStatus = Statuses::NO_PATCHING_DETECTED;
                extraStatus = Statuses::WILL_CONTINUE_SEARCH;
            }
        }
    }
    
    // check game values such as godmode, box movable, etc.
    int map_id = gh.GetMapId();
    if (map_id != -1 && map_id != 0)
    {
        string detectedBinds = gi.LookForActiveCheatingBinds();
        if (detectedBinds != "")
        {
            AddCheatsFound(detectedBinds);
            NotifyCheatsDetected();
            return;
        }
    }

    if (gh.GetMapId() == Constants::MAIN_MENU_ID)
    {
        gameStatus = Statuses::GAME_CONNECTED;
        extraStatus = Statuses::WAITING_FOR_MAP_LOAD_QUIT;
        performed_integrity_check = false;
    }
}

// waits for the game to be opened before we run any checks
static void CheckForBlackOpsProcess()
{
    GameHandler gh;

    if (!gh.IsGameOpen() && !cheats_detected)
    {
        NotifyGameClosed();
        gameStatus = Statuses::GAME_NOT_CONNECTED;
        extraStatus = "";
        gh.CheckGameMod();
        return;
    }

    // we want to be able to reuse the tool after cheats are detected
    if (notified_cheats_detected && gh.IsGameOpen())
    {
        OnGameReopen();
    }

    // this checks for certain things once, think of it as an "OnGameOpen"
    if (!initialized)
    {
        int map_id = gh.GetMapId();
        if (map_id != Constants::MAIN_MENU_ID)
        {
            if (map_id != 0 && map_id != -1 && extraStatus != Statuses::MORE_INFO_WINDOW)
            {
                extraStatus = Statuses::PATCHES_NOT_VERIFIED;
            }
            else {
                extraStatus = "";
            }
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

// we put the process function on a new thread to prevent it from being on the main thread
static void CheckForBlackOpsProcessThread()
{
    while (processThreadRunning)
    {
        CheckForBlackOpsProcess();
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main()
{
    init();

    // initialize the verification uids
    Verification v;
    v.init();

    // initialize the hashes for each fastfile
    GameIntegrity gi;
    gi.init();

    // when the tool is opened, check for updates
    Updater updater;
    updater.CheckForUpdates();

    int windowWidth = 650;
    int windowHeight = 230;

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), L"BO1 Anti Cheat", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(10);

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
    sf::Text anticheatText(Constants::TITLE, semiBoldFont, 20);
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
    sf::Text uid1Text(v.GetUID_1(), boldFont, 20);
    uid1Text.setFillColor(sf::Color::White);

    float uid1TextWidth = uid1Text.getGlobalBounds().width;
    uid1Text.setPosition((windowWidth - uid1TextWidth) / 2, 140 + 15);

    // uid #2 text
    sf::Text uid2Text(v.GetUID_2(), boldFont, 20);
    uid2Text.setFillColor(sf::Color::White);

    float uid2TextWidth = uid2Text.getGlobalBounds().width;
    uid2Text.setPosition((windowWidth - uid2TextWidth) / 2, 165 + 15);

    // start the game process thread
    thread checkProcessThread(CheckForBlackOpsProcessThread);
    
    while (window.isOpen())
    {
        sf::Event event;

        // wait for the close button to be pressed
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        statusText.setString(gameStatus); // constantly set the status text
        float statusTextWidth = statusText.getGlobalBounds().width;
        statusText.setPosition((windowWidth - statusTextWidth) / 2, 57); // adjust position based on text width and window width

        // if cheats have been detected then set it to a red/pink color
        if (cheats_detected)
        {
            statusText.setFillColor(sf::Color(255, 58, 58));
        }
        else // or we just set it back to white
        {
            statusText.setFillColor(sf::Color::White);
        }
        
        // set the extra status text
        extraStatusText.setString(extraStatus);
        float extraStatusTextWidth = extraStatusText.getGlobalBounds().width;
        extraStatusText.setPosition((windowWidth - extraStatusTextWidth) / 2, 90);

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
