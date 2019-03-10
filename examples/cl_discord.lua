
local loaded = pcall(require,"discord_rpc")
if not loaded then MsgN("Discord RPC not loaded!") return end
MsgN("Discord RPC module found")

local appid = "1234"

-- Having it global allows us to hotreload it without having to remake the timer
function DiscordUpdatePresence()
	discord.updatePresence({
		state = "Not in a team",
		details = string.format("%s (%i / %i players)", engine.ActiveGamemode(), player.GetCount(), game.MaxPlayers()),
		smallImageKey = "small_image",
		smallImageText = "This is a small image tooltip",
		largeImageKey = "large_image",
		largeImageText = "This is a large image tooltip",
	})
end


hook.Add("Initialize", "DiscordRPC", function()
	discord.init(appid) -- Initialize the RPC connection

	-- We need to wait for LocalPlayer to become available before we can do anything
	timer.Create("DiscordRPCWaitForLocalPlayer", 0, 0, function()
		if IsValid(LocalPlayer()) then
			timer.Remove("DiscordRPCWaitForLocalPlayer")

			DiscordUpdatePresence()
			timer.Create("DiscordRPCUpdatePresence", 15, 0, function() -- Can't call this more often than every 15 seconds
				DiscordUpdatePresence()
			end)
		end
	end)
end)
