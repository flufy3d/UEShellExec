// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UEShellExecPrivatePCH.h"


class FUEShellExec : public IUEShellExec
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FUEShellExec, UEShellExec )



void FUEShellExec::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FUEShellExec::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



