// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"


#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

//TODO core routines investigation

const FName MySessionName = FName("KidsPlaySession");

UEOSGameInstance::UEOSGameInstance()
{
	bIsLoggedIn = false;
}

void UEOSGameInstance::Init()
{
	Super::Init();
	bIsLoggedIn = false;
	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UEOSGameInstance::CreateSession(FString SessionName)
{
if(SessionName == "")
{
	SessionName = "Name your session!";
}
	if( bIsLoggedIn)
	{
		if(OnlineSubsystem)
		{
			if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bAllowJoinInProgress = false;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.bUseLobbiesVoiceChatIfAvailable = false;
				
				SessionSettings.Set(SEARCH_KEYWORDS, SessionName,EOnlineDataAdvertisementType::ViaOnlineService);

			
				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this,&UEOSGameInstance::OnCreateSessionsComplete);
				SessionPtr->CreateSession(0, MySessionName,SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Create Session: Not Logged In"));
	}
}

void UEOSGameInstance::FinishedLoggingIn_Implementation()
{
}

void UEOSGameInstance::LoginCheck(int& isLoggedIn)
{

	//EOS_Auth_GetLoginStatus(,0)
	UE_LOG(LogTemp, Warning, TEXT("isLoggedInEquals: %d"), isLoggedIn);

	isLoggedIn = bIsLoggedIn;
	switch (isLoggedIn)
	{
	case 0:
		isLoggedIn = true;

	default:
		isLoggedIn = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Returning: %d"), bIsLoggedIn);

	return;
}

void UEOSGameInstance::DestroySession()
{
	
	if( bIsLoggedIn)
	{
		if(OnlineSubsystem)
		{
			if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionComplete);
				SessionPtr->DestroySession(MySessionName);
			}
		}
	}
}

void UEOSGameInstance::LonginReattempt()
{
	Login();
}

void UEOSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(OnlineSubsystem)
	{
		if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
		{
			SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
		}
	}
}


void UEOSGameInstance::Login()
{
	if(OnlineSubsystem)
	{
		if(IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString();
			Credentials.Token= FString();
			Credentials.Type = FString("accountportal");

			Identity->OnLoginCompleteDelegates->AddUObject(this,&UEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID,
	const FString& Error)
{
	bIsLoggedIn = 1;
	UE_LOG(LogTemp, Warning, TEXT("Running Login Complete Delegate"));
	UE_LOG(LogTemp, Warning, TEXT("LoggedIn: %d"), bWasSuccessful);

	//LocalPlayerName = BasicProfile
	if(OnlineSubsystem)
	{
		if(IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
		//CreateSession("Just Waiting");
	}
	
}



void UEOSGameInstance::OnCreateSessionsComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bWasSuccessful);

	OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}	
}

FString UEOSGameInstance::LocalPlayerName()
{
	if(OnlineSubsystem && bIsLoggedIn)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
		{
			if (OnlineSubsystem->GetSubsystemName() == "Epic")
			{
				//EOS_UserInfo.
			}
		}
	}
	return "NULL";
}

void UEOSGameInstance::FindSessions()
{
	if( bIsLoggedIn)
	{
		if(OnlineSubsystem)
		{
			if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
			{

				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS,FString("tmp"), EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES,true, EOnlineComparisonOp::Equals);
				SearchSettings->MaxSearchResults = 50;



				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionsComplete);
				SessionPtr->FindSessions(0,SearchSettings.ToSharedRef());
			}
		}
	}
}

void UEOSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bWasSuccessful);

	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d  Lobbies"), SearchSettings->SearchResults.Num());
		if(OnlineSubsystem)
		{
			if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
			{
				if(SearchSettings->SearchResults.Num())
				{
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this,&UEOSGameInstance::OnJoinSessionComplete);
					SessionPtr->JoinSession(0,FName("KidsPlaySession"),SearchSettings->SearchResults[0]);

				}
			}
		}
		if(IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}

	
	}
}

void UEOSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(OnlineSubsystem)
	{
		if(IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(SessionName,ConnectionInfo);
			if(!ConnectionInfo.IsEmpty())
			{
				if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0))
				{
					PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
}