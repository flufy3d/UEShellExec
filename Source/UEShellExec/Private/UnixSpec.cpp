#include "UEShellExecPrivatePCH.h"
#include "UnixSpec.h"

#if !PLATFORM_WINDOWS
int SystemWrap(const FString& cmd)
{
	int ret = 0;
	char* _str = TCHAR_TO_ANSI(*cmd);
	ret = system(_str);
	return ret;
}


FString PopenWrap(const FString& cmd)
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
#endif