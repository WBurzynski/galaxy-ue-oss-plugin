#pragma once

#include "CommonGOG.h"
#include "Types/UniqueNetIdGOG.h"

#include "Engine/NetConnection.h"

#include "NetConnectionGOG.generated.h"

UCLASS(transient, config = Engine)
class UNetConnectionGOG
	: public UNetConnection
{

	GENERATED_BODY()

public:

	void InitBase(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;

	void InitRemoteConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, const class FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;

	void InitLocalConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;

#if ENGINE_MINOR_VERSION >= 21 || ENGINE_MAJOR_VERSION > 4
	void LowLevelSend(void* InData, int32 InCountBits, FOutPacketTraits& OutTraits) override;

#if ENGINE_MINOR_VERSION < 23 && ENGINE_MAJOR_VERSION < 5
	TSharedPtr<FInternetAddr> GetInternetAddr() override;

#endif
#else
	void LowLevelSend(void* InData, int32 InCountBytes, int32 InCountBits) override;
#endif
	FString LowLevelGetRemoteAddress(bool InAppendPort = false) override;

	FString LowLevelDescribe() override;

	FString RemoteAddressToString() override;

private:

	class LobbyMemberStateListener
		: public galaxy::api::GlobalLobbyMemberStateListener
	{
	public:

		LobbyMemberStateListener(UNetConnectionGOG& InConnection);

		void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& InLobbyID, const galaxy::api::GalaxyID& InMemberID, galaxy::api::LobbyMemberStateChange InMemberStateChange) override;

		UNetConnectionGOG& connection;
	};

	FUniqueNetIdGOG remotePeerID;

	TUniquePtr<LobbyMemberStateListener> serverMemberStateListener;
};


