# BO1-AntiCheat
An anticheat program for Black Ops 1 Zombies to serve as an unofficial alternative to the showing files rule.

## Why?
Cheating has never been easier in this game, and just showing how you start the game is not the most reliable way to detect cheating as you can inject the stealth patch mid game, which allows the use of a custom fastfile with any name.

## Detection methods
- Disallows modification of vanilla fastfile patches related to zombies.
- Disallows any extra files in the `zone/Common` folder, as those could be stealth patches.
- Checks the integrity of `frontend_patch.ff` as that is a mod loader file and could be used to cheat on community leaderboards if changed.
- Searches for known stealth patches injected into the game.

## People who helped
- [lveez](https://github.com/lveez) - Verification method so that the program can be open source.
