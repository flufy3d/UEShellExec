#include "UEShellExecPrivatePCH.h"
#include "WindowsSpec.h"

#if PLATFORM_WINDOWS

#include "AllowWindowsPlatformTypes.h"

int SystemWrap(const FString& cmd)
{

	FString _str("cmd.exe /c ");
	_str += *cmd;
	const TCHAR  *cmdArgs = *_str;

    PROCESS_INFORMATION pinfo;
    STARTUPINFO sinfo;

    memset(&sinfo, 0, sizeof(sinfo));
    sinfo.cb = sizeof(sinfo);
	GetStartupInfo(&sinfo);
    sinfo.wShowWindow = SW_HIDE;
    sinfo.dwFlags = STARTF_USESHOWWINDOW;

    
    CreateProcess(NULL, (TCHAR*)cmdArgs,
        NULL, NULL, false,
        0,
        NULL, NULL, &sinfo, &pinfo);
    DWORD ret;
    while (1)
    {
        HANDLE array[1];
        array[0] = pinfo.hProcess;
        ret = MsgWaitForMultipleObjects(1, array, false, INFINITE,
            QS_ALLPOSTMESSAGE);
        if ((ret == WAIT_FAILED) || (ret == WAIT_OBJECT_0))
            break;

        //Don't block message loop

        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    DWORD pret = 0; 
    GetExitCodeProcess(pinfo.hProcess, &pret);
    
    return pret;
}

int Unicode2Utf8(const wchar_t* unicode, char* utf8, int nBuffSize)
{
	if (!unicode || !wcslen(unicode))
	{
		return 0;
	}
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	if (len > nBuffSize)
	{
		return 0;
	}
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, len, NULL, NULL);
	return len;
}

FString PopenWrap(const FString& cmd)
{
	FString retStr;

	FString _str("cmd.exe /c ");
	_str += *cmd;
    const TCHAR  *cmdArgs = *_str;

    //create pipe
    HANDLE hRead,hWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = true;
	CreatePipe(&hRead, &hWrite, &sa, 0);

    //start process

    PROCESS_INFORMATION pinfo;
    STARTUPINFO sinfo;

    memset(&sinfo, 0, sizeof(sinfo));
    sinfo.cb = sizeof(sinfo);
	GetStartupInfo(&sinfo);
    sinfo.wShowWindow = SW_HIDE;
    sinfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    sinfo.hStdError = hWrite;
    sinfo.hStdOutput = hWrite;

    
    CreateProcess(NULL, (TCHAR*)cmdArgs,
        NULL, NULL, true,
        0,
        NULL, NULL, &sinfo, &pinfo);

	CloseHandle(hWrite);

    //read process output
    const int buffer_size = 1024;
    TCHAR buffer[buffer_size] = {0};
    DWORD bytesRead = 0;

    retStr = "";

    while(true)
    {
        if (ReadFile(hRead,buffer,buffer_size,&bytesRead,NULL) == NULL)
        {
            break;
        }
		retStr += UTF8_TO_TCHAR(buffer);
    }

    CloseHandle(hRead);
    


    //wait process end
    
    while (true)
    {
        DWORD ret;
        HANDLE array[1];
        array[0] = pinfo.hProcess;
        ret = MsgWaitForMultipleObjects(1, array, false, INFINITE,
            QS_ALLPOSTMESSAGE);
        if ((ret == WAIT_FAILED) || (ret == WAIT_OBJECT_0))
            break;

        //Don't block message loop

        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    DWORD pret = 0; 
    GetExitCodeProcess(pinfo.hProcess, &pret);

	return retStr;
}

#include "HideWindowsPlatformTypes.h"

#endif