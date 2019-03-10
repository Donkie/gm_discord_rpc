@echo off

rmdir /s /q "gm_discord_rpc_release"
mkdir "gm_discord_rpc_release"
mkdir "gm_discord_rpc_release\add-to-lua-bin-folder"
mkdir "gm_discord_rpc_release\add-to-common-garrysmod-folder"
xcopy /s/y/i "build\gmcl_discord_rpc_win32.dll" "gm_discord_rpc_release\add-to-lua-bin-folder"
xcopy /s/y/i "lib\discord-rpc.dll" "gm_discord_rpc_release\add-to-common-garrysmod-folder"
xcopy /s/y/i "lib\send-presence.exe" "gm_discord_rpc_release\add-to-common-garrysmod-folder"
