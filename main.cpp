//
// Created by chris_000 on 10/31/2015.
//


#include <windows.h>
#include <iostream>
#include <c++/thread>


using namespace std;
BOOL executeCommandLine(LPSTR cmdLine, DWORD & exitCode)
{
    PROCESS_INFORMATION processInformation = {0};
    STARTUPINFO startupInfo;
    //startupInfo.cb                         = sizeof(startupInfo);
    GetStartupInfo(&startupInfo);



    // Create the process
    BOOL result = CreateProcess(NULL, cmdLine,
                                NULL, NULL, TRUE,
                                NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
                                NULL, NULL, &startupInfo, &processInformation);


    if (!result)
    {
        // CreateProcess() failed
        // Get the error from the system
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

        // Display the error
        char* strError = (LPTSTR) lpMsgBuf;
        std::cout << "::executeCommandLine() failed at CreateProcess()"
                "\nCommand="<<cmdLine<<"\n"
                "Message=" << strError <<"\n\n";

        // Free resources created by the system
        LocalFree(lpMsgBuf);

        // We failed.
        return FALSE;
    }
    else
    {
        // Successfully created the process.  Wait for it to finish.
        WaitForSingleObject( processInformation.hProcess, INFINITE );

        // Get the exit code.
        result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

        // Close the handles.
        CloseHandle( processInformation.hProcess );
        CloseHandle( processInformation.hThread );

        if (!result)
        {
            // Could not get exit code.
            std::cout << "Executed command but couldn't get exit code.\nCommand="
            <<cmdLine<<"\n";
            return FALSE;
        }


        return TRUE;
    }
}

int main() {
    DWORD exitCode;
    LPSTR program = (LPSTR) "C:\\Users\\chris_000\\ClionProjects\\MGS_V Patch\\bin\\Debug\\MGS_V_Patch.exe";
    LPWSTR pMessage = L"%1!*.*s! %3 %4!*s!";

    DWORD_PTR args[] = { (DWORD_PTR)4, (DWORD_PTR)2, (DWORD_PTR)L"Bill",  // %1!*.*s! refers back to the first insertion string in pMessage
                          (DWORD_PTR)L"Bob",                                                // %4 refers back to the second insertion string in pMessage
                          (DWORD_PTR)6, (DWORD_PTR)L"Bill"  };
    if(executeCommandLine(program, exitCode)) {
        LPTSTR exitCodeStr;
        HMODULE Hand = LoadLibrary("NTDLL.DLL");
        FormatMessage(
                FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                //FORMAT_MESSAGE_IGNORE_INSERTS,
                Hand,
                exitCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR) &exitCodeStr,
                0,
                (va_list*)&args);
        std::cout<<"asdfasdf"<<exitCodeStr<<std::endl;
        wprintf(L"Formatted message: %s\n", exitCodeStr);
        LocalFree(exitCodeStr);
        FreeLibrary(Hand);

    }
    return 0;
}