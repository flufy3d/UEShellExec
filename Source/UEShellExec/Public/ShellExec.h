#pragma once

#include "ShellExec.generated.h"

UCLASS(ClassGroup = UEShellExec, Blueprintable)
class UESHELLEXEC_API UShellExec : public UBlueprintFunctionLibrary
{

    GENERATED_UCLASS_BODY()

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call System Function", Keywords = "system shell exec"), Category = UEShellExec)
        static int System(const FString& cmd);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call System Function Background", Keywords = "system shell exec background"), Category = UEShellExec)
		static void System_Background(const FString& cmd);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call Popen Function", Keywords = "popen shell exec"), Category = UEShellExec)
        static FString Popen(const FString& cmd);

};
