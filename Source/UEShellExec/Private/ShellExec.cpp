#include "UEShellExecPrivatePCH.h"

#include "WindowsSpec.h"

#include <stdio.h>  
#include <stdlib.h> 

UShellExec::UShellExec(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}


class ShellExecAutoDeleteAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<ShellExecAutoDeleteAsyncTask>;

	FString _cmd;

	ShellExecAutoDeleteAsyncTask(const FString& cmd)
		: _cmd(cmd)
	{
	}

	void DoWork()
	{
		int ret = 0;
#if PLATFORM_WINDOWS
		const TCHAR* _str = *_cmd;
		ret = system_hidden(_str);	
#else
		char* _str = TCHAR_TO_ANSI(*_cmd);
		ret = system(_str);
#endif
		UE_LOG(LogUEShellExec, Log, TEXT("ShellExecAutoDeleteAsyncTask..."));
		UE_LOG(LogUEShellExec, Log, TEXT("UShellExec::System Process returned %d."), ret);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ShellExecAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};


void UShellExec::System_Background(const FString& cmd)
{
	// start an example job
	(new FAutoDeleteAsyncTask<ShellExecAutoDeleteAsyncTask>(cmd))->StartBackgroundTask();

	// do an example job now, on this thread
	//(new FAutoDeleteAsyncTask<ShellExecAutoDeleteAsyncTask>(cmd))->StartSynchronousTask();
}

int UShellExec::System(const FString& cmd)
{
	int ret = 0;
	char* _str = TCHAR_TO_ANSI(*cmd);
	ret = system(_str);
	UE_LOG(LogUEShellExec, Log, TEXT("UShellExec::System Process returned %d."), ret);
	return ret;

}
    
FString UShellExec::Popen(const FString& cmd)
{
	FString ret;
	char* _str = TCHAR_TO_ANSI(*cmd);
	
	const int buffer_size = 1024;
	char psBuffer[buffer_size];
	FILE   *pPipe;


	if ((pPipe = _popen(_str, "rt")) == NULL)
	{
		UE_LOG(LogUEShellExec, Error, TEXT("Error: Failed to open the pipe."));
		return ret;
	}

	/* Read pipe until end of file, or an error occurs. */

	while (fgets(psBuffer, buffer_size, pPipe))
	{
		FString tmp(psBuffer);
		ret += tmp;
	}

	/* Close pipe and print return value of pPipe. */
	if (feof(pPipe))
	{
		UE_LOG(LogUEShellExec, Log, TEXT("UShellExec::Popen Process returned %d."), _pclose(pPipe));
	}
	else
	{
		UE_LOG(LogUEShellExec, Error, TEXT("Error: Failed to read the pipe to the end."));
	}


	return ret;

}




