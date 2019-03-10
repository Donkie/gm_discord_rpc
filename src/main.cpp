#include "GarrysMod/Lua/Interface.h"
#include "discord-rpc/discord_rpc.h"

#include "cstring"
#include "stdio.h"
#include "main.h"

using namespace GarrysMod::Lua;

#define STEAMGAMEID "4000"

static const DiscordUser* connected_user;
static void handleDiscordReady(const DiscordUser* user)
{
	connected_user = user;
	/*printf("\nDiscord: connected to user %s#%s - %s\n",
		user->username,
		user->discriminator,
		user->userId);*/
}

static int disconnected_code;
static const char* disconnected_message;
static void handleDiscordDisconnected(int errcode, const char* message)
{
	disconnected_code = errcode;
	disconnected_message = message;
}

static int error_code;
static const char* error_message;
static void handleDiscordError(int errcode, const char* message)
{
	error_code = errcode;
	error_message = message;
}

static void handleDiscordJoinGame(const char* secret)
{
}

static void handleDiscordJoinRequest(const DiscordUser* request)
{
}

LUA_FUNCTION(polling)
{
	/*
	 * Attempt to get the discord thrown events by polling with a think
	 * For some reason, either the events are not being called, or there is some thread issue because these never
	 * print anything.
	 */
	if (connected_user) {
		char str_out[512];
		sprintf_s(str_out, "Discord: connected to user %s#%s\n", connected_user->username, connected_user->userId);
		printMessage(LUA, str_out, 114, 137, 218);

		connected_user = nullptr;
	}

	if (disconnected_message)
	{
		char str_out[512];
		sprintf_s(str_out, "Discord: disconnected (%d: %s)\n", disconnected_code, disconnected_message);
		printMessage(LUA, str_out, 114, 137, 218);

		disconnected_message = nullptr;
	}

	if (error_message)
	{
		char str_out[512];
		sprintf_s(str_out, "Discord: error (%d: %s)\n", error_code, error_message);
		printMessage(LUA, str_out, 218, 114, 114);

		error_message = nullptr;
	}

	return 1;
}

LUA_FUNCTION(discordUpdatePresence)
{
	LUA->CheckType(1, Type::TABLE);

	DiscordRichPresence discord_presence;
	memset(&discord_presence, 0, sizeof(discord_presence));

	LUA->GetField(1, "state");
	if(LUA->IsType(-1, Type::STRING))
		discord_presence.state = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "details");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.details = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "smallImageKey");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.smallImageKey = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "smallImageText");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.smallImageText = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "largeImageKey");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.largeImageKey = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "largeImageText");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.largeImageText = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "startTimestamp");
	if (LUA->IsType(-1, Type::NUMBER))
		discord_presence.startTimestamp = static_cast<int64_t>(LUA->GetNumber(-1));
	LUA->Pop();

	LUA->GetField(1, "endTimestamp");
	if (LUA->IsType(-1, Type::NUMBER))
		discord_presence.endTimestamp = static_cast<int64_t>(LUA->GetNumber(-1));
	LUA->Pop();

	LUA->GetField(1, "partyId");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.partyId = LUA->GetString(-1);
	LUA->Pop();

	LUA->GetField(1, "partySize");
	if (LUA->IsType(-1, Type::NUMBER))
		discord_presence.partySize = static_cast<int>(LUA->GetNumber(-1));
	LUA->Pop();

	LUA->GetField(1, "partyMax");
	if (LUA->IsType(-1, Type::NUMBER))
		discord_presence.partyMax = static_cast<int>(LUA->GetNumber(-1));
	LUA->Pop();

	LUA->GetField(1, "joinSecret");
	if (LUA->IsType(-1, Type::STRING))
		discord_presence.joinSecret = LUA->GetString(-1);
	LUA->Pop();

	Discord_UpdatePresence(&discord_presence);

	return 1;
}

LUA_FUNCTION(discordInit)
{
	LUA->CheckType(1, Type::STRING);

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = handleDiscordReady;
	handlers.errored = handleDiscordError;
	handlers.disconnected = handleDiscordDisconnected;
	handlers.joinGame = handleDiscordJoinGame;
	handlers.joinRequest = handleDiscordJoinRequest;

	Discord_Initialize(LUA->GetString(1), &handlers, 1, STEAMGAMEID);

	printMessage(LUA, "Discord initialized.\n", 114, 137, 218);

	return 1;
}

GMOD_MODULE_OPEN()
{
	LUA->PushSpecial(SPECIAL_GLOB);

	LUA->CreateTable();
	LUA->PushCFunction(discordInit); LUA->SetField(-2, "init");
	LUA->PushCFunction(discordUpdatePresence); LUA->SetField(-2, "updatePresence");
	LUA->SetField(-2, "discord");

	LUA->GetField(-1, "hook");
	LUA->GetField(-1, "Add");
	LUA->PushString("Think");
	LUA->PushString("DiscordRPCPolling");
	LUA->PushCFunction(polling);
	LUA->Call(3, 0);
	LUA->Pop();

	LUA->Pop();


    return 0;
}

GMOD_MODULE_CLOSE()
{
	Discord_Shutdown();

    return 0;
}
