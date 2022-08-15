// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

#include <eos_auth.h>
#include <eos_connect.h>
#include <eos_sessions.h>


#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h" 
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

//TODO core routines investigation

const FName MySessionName = FName("KidsPlaySession");

UEOSGameInstance::UEOSGameInstance()
{
	bIsLoggedIn = 0;
}

void UEOSGameInstance::Init()
{
	Super::Init();
	OnlineSubsystem = FOnlineSubsystemEOS::Get();
	//Login();
	this->LoggedIntoEOS();
}

void UEOSGameInstance::CreateSession()
{
	if( bIsLoggedIn == 1)
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
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.bUseLobbiesVoiceChatIfAvailable = false;
				
				SessionSettings.Set(SEARCH_KEYWORDS, MySessionName.ToString(),EOnlineDataAdvertisementType::ViaOnlineService);

			
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

	if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
	{
		IOnlineSessionPtr testvar = OnlineSubsystem ->GetSessionInterface();
		//UE_LOG(LogTemp, Error, TEXT("The Session interface is %d"), testvar);

	}
	
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

void UEOSGameInstance::OnOnlineStatusChange(int Success)
{
	bIsLoggedIn = Success;
	UE_LOG(LogTemp, Warning, TEXT("Online status changed, they IsLoggedIn is now: %d"), bIsLoggedIn);
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

			Identity->OnLoginChangedDelegates.AddUObject(this,&UEOSGameInstance::OnOnlineStatusChange);
			Identity->OnLoginCompleteDelegates->AddUObject(this,&UEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID,
	const FString& Error)
{
	bIsLoggedIn = bWasSuccessful;
	UE_LOG(LogTemp, Warning, TEXT("Running Login Complete Delegate"));
	UE_LOG(LogTemp, Warning, TEXT("LoggedIn: %d"), bWasSuccessful);
	UE_LOG(LogTemp, Warning, TEXT("LoggedIn internal value: %d"), bIsLoggedIn);


	//LocalPlayerName = BasicProfile
	if(OnlineSubsystem)
	{
		if(IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			LoggedIntoEOS();
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
		//CreateSession("Just Waiting");
	}
	
}



void UEOSGameInstance::OnCreateSessionsComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Sessuibs Cinokete Success: %d"), bWasSuccessful);

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

void UEOSGameInstance::EOS_Sessions_UpdateSessionModification(EOS_HSessionModification Handle)
{
	
}

void UEOSGameInstance::CloseSession()
{
	//todo: close session for other players
	//FOnlineSessionSettings SessionSettings;
	//EOS_Sessions_UpdateSessionModification(*new EOS_HSessionModification);
}

void UEOSGameInstance::FindSessions()
{
	if(bIsLoggedIn)
	{
		if(OnlineSubsystem)
		{
			if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
			{
				UE_LOG(LogTemp, Warning, TEXT("Finding Sessions"));
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->QuerySettings.SearchParams.Empty();
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS,FString(MySessionName.ToString()), EOnlineComparisonOp::Equals);
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
	UE_LOG(LogTemp, Warning, TEXT("Finding Sessions has finished, results were: %d"), bWasSuccessful);

	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d  Lobbie(s)"), SearchSettings->SearchResults.Num());
		if(OnlineSubsystem)
		{
			if(IOnlineSessionPtr SessionPtr = OnlineSubsystem-> GetSessionInterface())
			{
				if(SearchSettings->SearchResults.Num())
				{
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this,&UEOSGameInstance::OnJoinSessionComplete);
					SessionPtr->JoinSession(0,MySessionName,SearchSettings->SearchResults[0]);

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