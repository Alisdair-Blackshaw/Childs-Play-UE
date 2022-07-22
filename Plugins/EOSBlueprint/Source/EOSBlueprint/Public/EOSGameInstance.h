// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EOSBLUEPRINT_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEOSGameInstance();
	virtual void Init() override;
	UFUNCTION(BlueprintCallable, DisplayName="Initalise Online Session", meta=(Keywords="EOSBlueprint sample test testing"), Category="EOSBlueprintTesting")
	void CreateSession();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(Keywords="EOSBlueprint sample test testing"), Category="EOSBlueprintTesting")
	void FinishedLoggingIn();

	UFUNCTION(BlueprintCallable, DisplayName="Check if logged in")
	void LoginCheck(int& isLoggedIn);
	
	UFUNCTION(BlueprintCallable, DisplayName="Destroy Session", Category="EOSBlueprintTesting")
	void DestroySession();

	UFUNCTION(BlueprintCallable, DisplayName="Try logging in again")
	void LonginReattempt();

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnOnlineStatusChange(int Success);
	
	UFUNCTION(BlueprintCallable, DisplayName="Login", meta=(Keywords="EOSBlueprint sample test testing"), Category="EOSBlueprintTesting")
	void Login();
	
	void OnCreateSessionsComplete(FName SessionName, bool bWasSuccessful);

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID, const FString& Error);
	
	UFUNCTION(BlueprintPure, meta=(Keywords="EOSBlueprint sample test testing"), Category="EOSBlueprintTesting")
	FString LocalPlayerName();


	UFUNCTION(BlueprintImplementableEvent,DisplayName="Logged into EOS", meta=(Keywords="EOSBlueprint sample test testing"), Category="EOSBlueprintTesting")
	void LoggedIntoEOS();
	
UFUNCTION(BlueprintCallable, Category="EOSBlueprintTesting")
	void FindSessions();
	void OnFindSessionsComplete(bool bWasSuccessful);
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	int bIsLoggedIn;
};
