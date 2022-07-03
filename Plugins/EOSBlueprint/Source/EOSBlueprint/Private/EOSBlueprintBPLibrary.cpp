// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOSBlueprintBPLibrary.h"

#include <eos_userinfo.h>

#include "G:\Unreal Engine\Engine\Source\ThirdParty\EOSSDK\SDK\Include\eos_sdk.h"
#include "G:\Unreal Engine\Engine\Source\ThirdParty\EOSSDK\SDK\Include\eos_common.h"
#include "EOSBlueprint.h"
#include "eos_auth.h"
#include "eos_init.h"
#include "assert.h"

UEOSBlueprintBPLibrary::UEOSBlueprintBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}
 bool EOSConfigured = false;
 bool FullyLoggedIn = false;
 EOS_InitializeOptions PlatformInitOptions;
 EOS_Platform_Options PlatformOptions;
 UEOSBlueprintBPLibrary::keys GameKeys;
 UEOSBlueprintBPLibrary::localUserInfo GUserInfo;
 EOS_HAuth AuthHandle;
 EOS_HPlatform PlatformHandler = nullptr;
 EOS_UserInfo_QueryUserInfoOptions QueryUserInfoOptions;
 


float UEOSBlueprintBPLibrary::EOSBlueprintSampleFunction(float Param)
{
	return -1;
}

void PlayerLoginCallback(const EOS_Auth_LoginCallbackInfo* Data);

void UEOSBlueprintBPLibrary::TestOnlineInterface	(FString ProductName, FString ProductVersion, FString ProductID, FString ClientID, FString ClientSecret, FString
                                                     ApplicationID, FString SandboxID, FString DeploymentID, FString EncryptionKey, bool& IsFunctioning, FString& ErrorOut)
{
	IsFunctioning = true;
	GameKeys.ProductName = TCHAR_TO_UTF8(*ProductName);
	if(GameKeys.ProductName == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your ProductName";
		return;
	}
	GameKeys.ProductVersion = TCHAR_TO_UTF8(*ProductVersion);
	if(GameKeys.ProductVersion == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your ProductVersion";
		return;
	}
	GameKeys.ProductID = TCHAR_TO_UTF8(*ProductID);
	if(GameKeys.ProductID == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your ProductID";
		return;
	}
	GameKeys.SandboxID = TCHAR_TO_UTF8(*SandboxID);
	if(GameKeys.SandboxID == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your SandboxID";
		return;
	}
	GameKeys.DeploymentID = TCHAR_TO_UTF8(*DeploymentID);
	if(GameKeys.DeploymentID == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your DeploymentID";
		return;
	}
	GameKeys.ClientID = TCHAR_TO_UTF8(*ClientID);
	if(GameKeys.ClientID == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your ClientCredentialsID";
		return;
	}
	GameKeys.EncryptionKey = TCHAR_TO_UTF8(*EncryptionKey);
	if(GameKeys.EncryptionKey == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your EncryptionKey";
		return;
	}
	GameKeys.ApplicationID = TCHAR_TO_UTF8(*ApplicationID);
	if(GameKeys.EncryptionKey == "")
	{
		IsFunctioning = false;
		ErrorOut = "Please set your ApplicationID";
		return;
	}

	PlatformInitOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	PlatformInitOptions.ProductName = TCHAR_TO_UTF8(*ProductName);
	PlatformInitOptions.ProductVersion = TCHAR_TO_UTF8(*ProductVersion);
	PlatformInitOptions.AllocateMemoryFunction = NULL;
	PlatformInitOptions.ReallocateMemoryFunction = NULL;

	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.ProductId = TCHAR_TO_UTF8(*ProductID);
	PlatformOptions.SandboxId = TCHAR_TO_UTF8(*SandboxID);
	PlatformOptions.EncryptionKey = TCHAR_TO_UTF8(*EncryptionKey);
	PlatformOptions.DeploymentId = TCHAR_TO_UTF8(*DeploymentID);
	PlatformOptions.bIsServer = false;

	QueryUserInfoOptions.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	
	PlatformHandler = EOS_Platform_Create(&PlatformOptions);
	EOS_EResult InitResult = EOS_Initialize(&PlatformInitOptions);
	if (InitResult != EOS_EResult::EOS_Success && InitResult != EOS_EResult::EOS_AlreadyConfigured)
	{
		IsFunctioning = false;
		ErrorOut =  EOS_EResult_ToString(InitResult);
		
		
	}
	else
	{
		IsFunctioning = true;
		ErrorOut = EOS_EResult_ToString(InitResult);
		return;
	}
}




void PlayerLoginCallback(const EOS_Auth_LoginCallbackInfo* CliData)
{
	assert(CliData !=NULL);

	//EOS_EResult_ToString(CliData Result)



	
	UEOSBlueprintBPLibrary* ThisAuth = NULL;
	if (CliData -> ClientData)
	{
		ThisAuth = (UEOSBlueprintBPLibrary*)CliData->ClientData;
		if(ThisAuth)
		{
			//TODO Log current login mode (line 375 of Authentication.cpp)
		}
	}
	AuthHandle = EOS_Platform_GetAuthInterface(nullptr);
	assert(AuthHandle != nullptr);
	using FEpicAccountId = UEOSBlueprintBPLibrary::TEpicAccountId<EOS_EpicAccountId>;

	if (CliData->ResultCode == EOS_EResult::EOS_Success)
	{
		const int32_t AccountsCount = EOS_Auth_GetLoggedInAccountsCount(AuthHandle);
		for (int32_t AccountIdx = 0; AccountIdx < AccountsCount; ++AccountIdx)
		{
			FEpicAccountId AccountId = EOS_Auth_GetLoggedInAccountByIndex(AuthHandle, AccountIdx);

			EOS_ELoginStatus LoginStatus = EOS_Auth_GetLoginStatus(AuthHandle, CliData->LocalUserId);

			//FDebugLog::Log(L"[EOS SDK] [%d] - Account ID: %ls, Status: %d", AccountIdx, FEpicAccountId(AccountId).ToString().c_str(), (int32_t)LoginStatus);
		}

		//FGameEvent Event(EGameEventType::UserLoggedIn, CliData->LocalUserId);
		//FGame::Get().OnGameEvent(Event);
	}
}

bool Login()
{
	EOS_Platform_Tick(PlatformHandler);
	//EOS_Auth_Login(PlatformHandler,PlatformOptions,this ,PlayerLoginCallback);
	//UEOSBlueprintBPLibrary::AddConnectAuthExpirationNotification();
	return true;

}

void UEOSBlueprintBPLibrary::AddConnectAuthExpirationNotification()

	{
		if (ConnectAuthExpirationId == EOS_INVALID_NOTIFICATIONID)
		{
			EOS_Connect_AddNotifyAuthExpirationOptions Options{};
			Options.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;

			//ConnectAuthExpirationId = EOS_Connect_AddNotifyAuthExpiration(ConnectHandle, &Options, NULL, ConnectAuthExpirationCb);
		}
	}


void UEOSBlueprintBPLibrary::PlayerLogin()
{

}

void UEOSBlueprintBPLibrary::IsLoggedIn(bool& isLoggedIn)
{
	//EOS_Auth_GetLoginStatus(&PlatformHandler, Data->LocalUserID);
}

void UEOSBlueprintBPLibrary::ClientData_Implementation()
{
	
}

void UEOSBlueprintBPLibrary::getLocalUsername(FString& username)
{
	
}

void UEOSBlueprintBPLibrary::LoggedIn_Implementation()
{
	//EOS_UserInfo_QueryUserInfo(QueryUserInfoOptions&);
	//EOS_EpicAccountId_IsValid(localUserID);
}




