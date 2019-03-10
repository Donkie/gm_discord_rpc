# gm_discord_rpc
Discord rich presence clientside binary module for Garry's Mod

## How to install:
1. Download the latest release
2. Move `gmcl_discord_rpc_win32.dll` file to `steamapps/common/GarrysMod/garrysmod/lua/bin`, create the bin folder if it doesn't exist.
3. Move `discord-rpc.dll` and `send-presence.exe` to `steamapps/common/GarrysMod`

## Developers:
You need to register for a new application in Discord's developer portal. https://discordapp.com/developers/applications/
__IMPORTANT: NAME YOUR APPLICATION "Garry's Mod" or we'll probably get smacked by Facepunch__

### API
#### discord.init(*string* applicationID)
Initializes a connect to the Discord client. Important: Make sure you ever only call this once, perhaps in the Initialize hook to be safe from hot-reloads.

#### discord.updatePresence(*table* PresenceStruct)
Updates the user presence. The Discord API has an internal rate limiting of 15 seconds on this function so there's no use in calling this more often than that.

Leaving a field empty will unset its value.

#### PresenceStruct
| field          | type   | description                                                                                                                                 |
|----------------|--------|---------------------------------------------------------------------------------------------------------------------------------------------|
| state          | string | The user's state, e.g. "In a team", "Alone" or "In lobby".                                                                                  |
| details        | string | Game details, such as map name, server name, etc. Keep this as short as possible. Preferably throw more detailed info under largeImageText. |
| smallImageKey  | string | Image name of the small image.                                                                                                              |
| smallImageText | string | Tooltip text of the small image.                                                                                                            |
| largeImageKey  | string | Image name of the large image.                                                                                                              |
| largeImageText | string | Tooltip text of the large image.                                                                                                            |
| startTimestamp | number | UNIX timestamp of when for example the round started for rounds which have no specified end time.                                           |
| endTimestamp   | number | UNIX timestamp of when for example the current round will end. Overrides startTimestamp if specified.                                       |
| partyId        | string | Unique ID of the current party the player is in.                                                                                            |
| partySize      | number | How many players are currently in the player's party.                                                                                       |
| partyMax       | number | How many players can fit into the player's party.                                                                                           |
| joinSecret     | string | A secret ID used for the "Ask to join" button, doesn't work yet.          
