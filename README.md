# BO1-AntiCheat
An anticheat program for Black Ops 1 Zombies to serve as an unofficial alternative to the showing files rule.

This works with Steam and the latest version of BGamer.

## Current detection methods
- Checks the integrity of all fastfile patches related to zombies upon a map load/quit
- Makes sure there are no extra files in the `zone/Common` folder, as those could be stealth patches
- Checks the integrity of `frontend_patch.ff` as that is a mod loader file and could be used to cheat if changed

## People who helped
- [lveez](https://github.com/lveez) for open source verification method