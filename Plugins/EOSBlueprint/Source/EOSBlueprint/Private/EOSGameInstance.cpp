// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

#include <eos_auth.h>

#include "EOSBlueprintBPLibrary.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"

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


