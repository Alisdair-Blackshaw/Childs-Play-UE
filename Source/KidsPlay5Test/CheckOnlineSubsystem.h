#pragma once

class CheckOnlineSubsystem
{
public:
	CheckOnlineSubsystem();
	UFUNCTION(BlueprintPure, Category="Test")
	bool ReturnAValue();

	UFUNCTION(BlueprintCallable, Category="Test")
	void ToggleReturnedValue(bool bShouldBeActive);




protected:
	bool bReturnAValue;
};
