// SPDX-License-Identifier: MPL-2.0

#if _MSC_VER && !__INTEL_COMPILER
#include <Windows.h>
#include <cstdio>
#endif

namespace dxfeed::perf {

#if 0
  // https://learn.microsoft.com/en-us/windows/win32/secauthz/enabling-and-disabling-privileges-in-c--
  // https://www.installsetupconfig.com/win32programming/accesscontrollistaclexample3.html
  BOOL SetPrivilege(
      HANDLE hToken,          // access token handle
      LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
      BOOL bEnablePrivilege   // to enable or disable privilege
  )
  {
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if ( !LookupPrivilegeValue(
        NULL,      // lookup privilege on local system
        lpszPrivilege,  // privilege to lookup
        &luid ) )        // receives LUID of privilege
    {
      printf("LookupPrivilegeValue error: %u\n", GetLastError() );
      return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege) {
      tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    } else {
      tp.Privileges[0].Attributes = 0;
    }

    // Enable the privilege or disable all privileges.
    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES) NULL,
        (PDWORD) NULL) )
    {
      printf("AdjustTokenPrivileges error: %u\n", GetLastError() );
      return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
      printf("The token does not have the specified privilege. \n");
      return FALSE;
    }

    return TRUE;
  }

  void OpenProcessTokenAndSetPrivilege() {
    // Open a handle to the access token for the calling process
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
      printf("OpenProcessToken() failed, error %u\n", GetLastError());
    } else {
      printf("OpenProcessToken() is OK, got the handle !\n");
    }

    // Call the user defined SetPrivilege() function to
    // enable and set the needed privilege
    if (!SetPrivilege(hToken, SE_INC_BASE_PRIORITY_NAME, true)) {
      printf("Failed to disable the privilege, error %u\n", GetLastError());
    } else {
      printf("The privilege was disabled!\n");
    }
  }
#endif

  void setProcessPriorityClass() {
#if _MSC_VER && !__INTEL_COMPILER
    HANDLE hProcess = GetCurrentProcess();

    if (!SetProcessPriorityBoost(hProcess, true)) {
      auto dwError = GetLastError();
      printf("Can't SetProcessPriorityBoost, hr = %lu\n", dwError);
    }

    printf("Current process: 0x%p\n", hProcess);
    if (!SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS)) {
      auto dwError = GetLastError();
      printf("Can't SetPriorityClass for process to REALTIME_PRIORITY_CLASS, hr = %lu\n", dwError);
    } else {
      printf("Process priority class is: 0x%x\n", GetPriorityClass(hProcess));
    }

    HANDLE hThread = GetCurrentThread();
    if (!SetThreadPriorityBoost(hThread, true)) {
      auto dwError = GetLastError();
      printf("Can't SetProcessPriorityBoost, hr = %lu\n", dwError);
    }
    if (!SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL)) {
      auto dwError = GetLastError();
      printf("Can't SetPriorityClass for process to THREAD_PRIORITY_TIME_CRITICAL, hr = %lu\n", dwError);
    } else {
      printf("Thread priority class is: 0x%x\n", GetThreadPriority(hThread));
    }
#endif
  }
}