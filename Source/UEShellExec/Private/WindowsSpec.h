#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include "windows.h"

int system_hidden(const TCHAR  *cmdArgs)
{
    PROCESS_INFORMATION pinfo;
    STARTUPINFO sinfo;

    memset(&sinfo, 0, sizeof(sinfo));
    sinfo.cb = sizeof(sinfo);
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


#include "HideWindowsPlatformTypes.h"
#endif