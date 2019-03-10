#pragma once
#include "GarrysMod/Lua/Interface.h"

using namespace GarrysMod::Lua;

// Thanks to MYSQLOO
static void printMessage(ILuaBase* LUA, const char* str, const int r, const int g, const int b) {
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "Color");
	LUA->PushNumber(r);
	LUA->PushNumber(g);
	LUA->PushNumber(b);
	LUA->Call(3, 1);
	const auto ref = LUA->ReferenceCreate();
	LUA->GetField(-1, "MsgC");
	LUA->ReferencePush(ref);
	LUA->PushString(str);
	LUA->Call(2, 0);
	LUA->Pop();
	LUA->ReferenceFree(ref);
}
