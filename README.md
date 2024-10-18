# BO1-AntiCheat
An anticheat program for Black Ops 1 Zombies to serve as an unofficial alternative to the showing files rule.

## Why?
Modern problems require modern solutions. While the current solution may be a good way to catch cheaters one way, there are others that are not accounted for. Based a few players opinions in the community, something like this seems to be the only way to solve all of the current cheating issues, as anti cheat programs can evolve like the cheating methods themselves.

This tool can also be updated to verify setup patches if the community votes on allowing such in the future.

## Detection methods
- Disallows modification of vanilla fastfile patches related to zombies.
- Disallows any extra files in the `zone/Common` folder, as they could be a form of stealth patch.
- Checks the integrity of `frontend_patch.ff` as that is a mod loader file and could be used to cheat on community leaderboards if changed.
- Searches for known stealth patches injected into the game.

## Planned features
- Automatic updates.
- If a community leaderboard mod is being used, verify the integrity of the mod files. Additionally may check the integrity of game_mod itself, but only if necessary.

## People who helped
- [lveez](https://github.com/lveez) - Verification method so that the program can be open source.
