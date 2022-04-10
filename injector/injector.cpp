#include <stdio.h>
#include <Windows.h>

int main() {
	STARTUPINFO si;
	PROCESS_INFORMATION proc_info;
	void *remote_space;
	HANDLE thread_handle;

	wchar_t exe_name[] = TEXT("CeVIO AI.exe\0");
	char dll_name[] = "inject.dll\0";
	size_t dll_len = sizeof(dll_name) / sizeof(char);

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&proc_info, sizeof(proc_info));
	printf("[*] Injector started.\n");

	if (!CreateProcessW(
		exe_name,
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL,
		&si,
		&proc_info
	)) {
		printf("[*] Failed to create a process. error = 0x%08x\n", GetLastError());
		exit(1);
	}

	printf("[*] Created a process.\n");

	remote_space = VirtualAllocEx(
		proc_info.hProcess,
		NULL,
		MAX_PATH,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	);
	if (remote_space == NULL) {
		printf("[*] Failed to allocate a memory.\n");
		exit(1);
	}

	printf("[*] Allocated a memory.\n");

	if (!WriteProcessMemory(
		proc_info.hProcess,
		remote_space,
		dll_name,
		dll_len,
		NULL
	)) {
		printf("[*] Failed to write to memory.\n");
		exit(1);
	}

	printf("[*] Wrote to a memory.\n");

	thread_handle = CreateRemoteThread(
		proc_info.hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) LoadLibraryA,
		remote_space,
		0,
		NULL
	);
	if (thread_handle == NULL) {
		printf("[*] Failed to create a thread.\n");
		exit(1);
	}

	printf("[*] Created a thread.\n");

	if (WaitForSingleObject(thread_handle, INFINITE) == WAIT_FAILED) {
		printf("[*] DLL execution has failed.\n");
		exit(1);
	}

	CloseHandle(thread_handle);

	printf("[*] Successfully executed a thread.\n");

	if (ResumeThread(proc_info.hThread) == -1) {
		printf("[*] ResumeThread failed\n");
		exit(1);
	}

	printf("[*] Resumed a target process.\n");

	VirtualFreeEx(proc_info.hProcess, remote_space, 0, MEM_RELEASE);
	CloseHandle(proc_info.hProcess);

	return 0;
}
