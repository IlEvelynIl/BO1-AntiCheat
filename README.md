# BO1-AntiCheat
An anticheat program for Black Ops 1 Zombies to serve as an unofficial alternative to the showing files rule.

## Why?
Firstly, it is good to have more than one solution to cheating, no matter how big or small, there should always be more than one way. Secondly, cheating has never been easier in this game, and showing how you start the game is not the most reliable way to detect cheating, as that may not last forever due to workarounds being found.

## Detection methods
- Disallows modification of vanilla fastfile patches related to zombies.
- Disallows any extra files in the `zone/Common` folder, as they could be a form of stealth patch.
- Checks the integrity of `frontend_patch.ff` as that is a mod loader file and could be used to cheat on community leaderboards if changed.
- Searches for known stealth patches injected into the game.

## Planned features
- Automatic updates.
- Verify the integrity of a select few custom patches, this would only be added if the community voted for patches to be allowed, such as first box patches.
- If a community leaderboard mod is being used, verify the integrity of the mod files. Additionally may check the integrity of game_mod itself, but only if necessary.

## People who helped
- [lveez](https://github.com/lveez) - Verification method so that the program can be open source.
