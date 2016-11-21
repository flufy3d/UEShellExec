#pragma once

#include "ShellExec.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSystemDoneEvent, const FString&, Cmd, int, RetValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPopenDoneEvent, const FString&, Cmd, const FString&, RetValue);


UCLASS(ClassGroup = UEShellExec, Blueprintable)
class UESHELLEXEC_API USystemDoneObject : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
		FSystemDoneEvent SystemDoneEvent;
};

UCLASS(ClassGroup = UEShellExec, Blueprintable)
class UESHELLEXEC_API UPopenDoneObject : public UObject
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(BlueprintAssignable)
		FPopenDoneEvent PopenDoneEvent;
};


UCLASS(ClassGroup = UEShellExec, Blueprintable)
class UESHELLEXEC_API UShellExec : public UBlueprintFunctionLibrary
{

    GENERATED_UCLASS_BODY()


    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call System Function", Keywords = "system shell exec"), Category = UEShellExec)
        static int System(const FString& cmd);

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Call System Function Background", Keywords = "system shell exec background"), Category = UEShellExec)
		static USystemDoneObject* System_Background(UObject* WorldContextObject,const FString& cmd);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call Popen Function", Keywords = "popen shell exec"), Category = UEShellExec)
        static FString Popen(const FString& cmd);

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Call Popen Function Background", Keywords = "popen shell exec background"), Category = UEShellExec)
		static UPopenDoneObject* Popen_Background(UObject* WorldContextObject, const FString& cmd);

};
