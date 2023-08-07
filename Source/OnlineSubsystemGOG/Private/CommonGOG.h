#pragma once

#if ENGINE_MAJOR_VERSION > 4
#include "Online/CoreOnline.h"
#else
#include "UObject/CoreOnline.h"
#endif

#include "OnlineSubsystem.h"

#include <galaxy/GalaxyApi.h>

#define TEXT_GOG TEXT("GOG")

#define TEXT_ONLINE_SUBSYSTEM_GOG TEXT("OnlineSubsystemGOG")

#define TEXT_CONFIG_SECTION_GOG TEXT_ONLINE_SUBSYSTEM_GOG
#define TEXT_CONFIG_KEY_ACHIEVEMENTS TEXT("Achievements")

#define STRINGIFY(X) STRINGIFYIMPL(X)
#define STRINGIFYIMPL(X) #X

// The controller number of the associated local user
constexpr int32 LOCAL_USER_NUM{0};

inline void CheckLocalUserNum(int32 InLocalUserNum)
{
	if(InLocalUserNum != LOCAL_USER_NUM)
		UE_LOG_ONLINE(Error, TEXT("Only single local player is supported"));
}

inline uint64 CharLen(const FString& InString)
{
	return static_cast<uint64>(InString.Len()) * sizeof(FString::ElementType);
}