#include "LobbyStartListener.h"

#include "OnlineSessionGOG.h"

#include "Online.h"

FLobbyStartListener::FLobbyStartListener(galaxy::api::GalaxyID InLobbyID, FName InSessionName)
	: lobbyID{MoveTemp(InLobbyID)}
	, sessionName{MoveTemp(InSessionName)}
{
}

void FLobbyStartListener::OnLobbyDataUpdateSuccess(const galaxy::api::GalaxyID& InLobbyID)
{
	UE_LOG_ONLINE(Display, TEXT("OnLobbyDataUpdated: lobbyID=%llu"), InLobbyID.ToUint64());

	checkf(lobbyID == InLobbyID, TEXT("Unknown lobby (lobbyID=%llu). This shall never happen. Please contact GalaxySDK team"), InLobbyID.ToUint64());

	auto isLobbyJoinable = galaxy::api::Matchmaking()->IsLobbyJoinable(InLobbyID);
	auto err = galaxy::api::GetError();
	if (err)
		UE_LOG_ONLINE(Error, TEXT("Failed to check lobby joinability: %s, %s"), ANSI_TO_TCHAR(err->GetName()), ANSI_TO_TCHAR(err->GetMsg()));

	if (!isLobbyJoinable)
		UE_LOG_ONLINE(Error, TEXT("Failed to set Lobby as joinable"));

	TriggerOnStartSessionCompleteDelegates(isLobbyJoinable);
}

void FLobbyStartListener::OnLobbyDataUpdateFailure(const galaxy::api::GalaxyID& InLobbyID, galaxy::api::ILobbyDataUpdateListener::FailureReason InFailureReason)
{
	UE_LOG_ONLINE(Display, TEXT("FLobbyStartListener::OnLobbyDataUpdateFailure: lobbyID=%llu"), InLobbyID.ToUint64());

	checkf(lobbyID == InLobbyID, TEXT("Unknown lobby (lobbyID=%llu). This shall never happen. Please contact GalaxySDK team"), InLobbyID.ToUint64());

	UE_LOG_ONLINE(Error, InFailureReason == galaxy::api::ILobbyDataUpdateListener::FAILURE_REASON_LOBBY_DOES_NOT_EXIST
		? TEXT("Specified lobby does not exists")
		: TEXT("Unknown error"));

	TriggerOnStartSessionCompleteDelegates(false);
}

bool FLobbyStartListener::MarkSessionStarted(bool IsJoinable) const
{
	auto onlineSessionInterface = Online::GetSessionInterface();
	if (!onlineSessionInterface.IsValid())
	{
		return false;
	}

	auto storedSession = onlineSessionInterface->GetNamedSession(sessionName);
	if (!storedSession)
	{
		UE_LOG_ONLINE(Error, TEXT("Failed to finalize session stating as OnlineSession interface is invalid: sessionName='%s'"), *sessionName.ToString());
		return false;
	}

	storedSession->SessionState = IsJoinable ? EOnlineSessionState::InProgress : EOnlineSessionState::Pending;
	return IsJoinable;
}

void FLobbyStartListener::TriggerOnStartSessionCompleteDelegates(bool InResult)
{
	auto onlineSessionInterface = StaticCastSharedPtr<FOnlineSessionGOG>(Online::GetSessionInterface());
	if (!onlineSessionInterface.IsValid())
	{
		UE_LOG_ONLINE(Error, TEXT("Failed to finalize session stating as OnlineSession interface is invalid: sessionName='%s'"), *sessionName.ToString());
		return;
	}

	onlineSessionInterface->TriggerOnStartSessionCompleteDelegates(sessionName, MarkSessionStarted(InResult));
	onlineSessionInterface->FreeListener(ListenerID);
}
