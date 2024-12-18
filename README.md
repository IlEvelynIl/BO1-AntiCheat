# BO1-AntiCheat
A tool for competitive Black Ops 1 Zombies specifically designed to catch people cheating.

![image](https://github.com/user-attachments/assets/500d7426-96c0-4cd9-9d22-e137a2f3cac5)

## Why?
Modern problems require modern solutions. While the current solution may be a good way to catch cheaters one way, there are others that are not accounted for. Based on a few players' opinions in the community, something like this seems to be the only way to solve all of the current cheating issues, as anti cheat programs can evolve just like the cheating methods themselves.

This tool can also be updated to verify setup patches if the community votes on allowing such in the future.

## Using it in your games
If you decide to use this for your games, simply open it along side Black Ops 1 and it will automatically connect to the game.

Also, it is important to display it on your stream/recording somewhere, and must be fully legible.

You may also notice that there is not a minimize button, this is on purpose to prevent confusion with OBS not picking it up. Just let the program sit behind the game.

## Detection methods
When a map is loaded/quit:

- Looks for any extra files in the `zone/Common` folder and the current language folder, as they could be a form of stealth patch.
- Searches for known stealth patch loaders injected into the game.
- Checks for modification of:
   - Fastfiles related to zombies in both `Common` and your current language folder.
   - `frontend_patch.ff` as that is a mod loader file and could be used to cheat on community leaderboards if changed.
   - Community leaderboard mods if they are loaded.

Throughout the duration of a game with the tool open, the following player states are checked for any changes:
- God Mode
- Demi God Mode
- No Target

## Why did my antivirus block the download?
This is likely due to Windows Defender's sensitivity to programs without a signature. Rest assured, there's nothing harmful in the tool, hence why its open source. 
If you want to verify that, you can build it from source, its virtually the same thing, just missing proper verification values. However, without those verification values, the build can't be verified that its not modified further.

## People who helped
- [lveez](https://github.com/lveez) - Verification method so that the program can be open source.
- Spacey - Helped test checking of certain values in coop. (godmode, notarget, etc.)
- Plant - Helped test some stuff out.
- Prompt - Heavily influenced design.
