// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "G:\Unreal Engine\Engine\Source\ThirdParty\EOSSDK\SDK\Include\eos_sdk.h"
#include "G:\Unreal Engine\Engine\Source\ThirdParty\EOSSDK\SDK\Include\eos_common.h"
#include "C:\Users\alisd\Desktop\EOS SDK\Samples\Shared\Source\Utils\StringUtils.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EOSBlueprintBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UEOSBlueprintBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "EOSBlueprint sample test testing"), Category = "EOSBlueprintTesting")
	static float EOSBlueprintSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Check Online Plugin", Keywords ="EOSBlueprint sample test testing"), Category = "EOSBlueprintTesting" )
	static void TestOnlineInterface
	(	FString ProductName,FString ProductVersion, FString ProductID, FString ClientID, FString ClientSecret, FString ApplicationID, FString SandboxID, FString DeploymentID, FString EncryptionKey,
		bool& IsFunctioning, FString& ErrorOut);

	void AddConnectAuthExpirationNotification();
	UFUNCTION(BlueprintCallable)
	static void PlayerLogin();

	UFUNCTION(BlueprintCallable, DisplayName="Is Logged In")
	static void IsLoggedIn(bool& isLoggedIn);
	void ClientData_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EOSBlueprintTesting")
	void LoggedIn();

	UFUNCTION(BlueprintCallable, Category="EOSBlueprintTesting")
	void getLocalUsername(FString& username);

	
	

	EOS_NotificationId ConnectAuthExpirationId = EOS_INVALID_NOTIFICATIONID;
	typedef struct EOS_EpicAccountIdDetails* EOS_EpicAccountId;

	
	template<class TAccountType>
struct TEpicAccountId
	{
		/**
		* Construct wrapper from account id.
		*/
		TEpicAccountId(TAccountType InAccountId) : AccountId(InAccountId) {}
		TEpicAccountId() = default;
		TEpicAccountId(const TEpicAccountId&) = default;
		TEpicAccountId& operator=(const TEpicAccountId&) = default;

		bool operator==(const TEpicAccountId& Other) const
		{
			return AccountId == Other.AccountId;
		}

		bool operator!=(const TEpicAccountId& Other) const
		{
			return !(this->operator==(Other));
		}

		bool operator<(const TEpicAccountId& Other) const
		{
			return AccountId < Other.AccountId;
		}

		/**
		* Checks if account ID is valid.
		*/
		operator bool() const { return IsValid(); }

		/**
		* Easy conversion to EOS account ID.
		*/
		operator TAccountType() const
		{
			return AccountId;
		}

		/**
		* Prints out account ID as hex.
		*/
		std::wstring ToString() const
		{
			return TValidateAccount<TAccountType>::ToString(AccountId);
		}

		/**
		* True if account id  is valid
		*/
		bool IsValid() const 
		{ 
			return TValidateAccount<TAccountType>::IsValid(AccountId);
		};

		/** EOS Account Id */
		TAccountType AccountId = nullptr;
	};
	template <typename T>
struct TValidateAccount
	{
	};

	struct keys
	{
		std::string ProductName;
		std::string ProductVersion;
		std::string ProductID;
		std::string SandboxID;
		std::string DeploymentID;
		std::string ClientID;
		std::string ClientCredentialsSecret;
		std::string ApplicationID;
		std::string EncryptionKey;
	};
	struct localUserInfo
	{
		std::string playerName;
	};
public:
	bool Login();

};
