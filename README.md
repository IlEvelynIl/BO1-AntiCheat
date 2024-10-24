# BO1-AntiCheat
An anticheat tool for Black Ops 1 Zombies to serve as an unofficial alternative to the showing files rule.

## Why?
Modern problems require modern solutions. While the current solution may be a good way to catch cheaters one way, there are others that are not accounted for. Based a few players opinions in the community, something like this seems to be the only way to solve all of the current cheating issues, as anti cheat programs can evolve like the cheating methods themselves.

This tool can also be updated to verify setup patches if the community votes on allowing such in the future.

## Detection methods
A game is verified by this tool when it has performed an integrity check on a map load/quit.

- Disallows modification of vanilla fastfile patches related to zombies.
- Disallows any extra files in the `zone/Common` folder, as they could be a form of stealth patch.
- Checks the integrity of `frontend_patch.ff` as that is a mod loader file and could be used to cheat on community leaderboards if changed.
- Searches for known stealth patches injected into the game.
- Checks the integrity of all community leaderboard mods.

## Why did my antivirus block the download?
This is likely due to Windows Defender's sensitivity to programs without a signature. Rest assured, there's nothing harmful in the tool, hence why its open source. 
If you want to verify that, you can build it from source, its virtually the same thing, just missing proper verification values. However, without those verification values, the build can't be verified that its not modified further.

## People who helped
- [lveez](https://github.com/lveez) - Verification method so that the program can be open source.
