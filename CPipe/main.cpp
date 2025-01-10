
#include "Headers.h"
bool status = FALSE;



#define BUFFER_SIZE 512

HANDLE CreateNamedPipeG(LPCSTR lpName, int size) {
	return CreateNamedPipeA(lpName,
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		0,
		255,
		BUFFER_SIZE,
		BUFFER_SIZE,
		NULL,
		NULL);
}

void CloseHandleG(HANDLE clientHandle) {
	DisconnectNamedPipe(clientHandle);
	CloseHandle(clientHandle);
}

DWORD WINAPI InstanceThread(HANDLE hPipe) {
	if (hPipe == NULL)
		return -1;
	HANDLE hHeap = GetProcessHeap();
	TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFFER_SIZE * sizeof(TCHAR));
	TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFFER_SIZE * sizeof(TCHAR));
	BOOL fSuccess = FALSE;
	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
	while (true)
	{
		fSuccess = ReadFile(
			hPipe,
			pchRequest,
			BUFFER_SIZE * sizeof(TCHAR),
			&cbBytesRead,
			NULL);
		if (!fSuccess || cbBytesRead == 0) {
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				cout << "InstanceThread: client disconnected.\n" << endl;
			}
			else
			{
				cout << TEXT("InstanceThread ReadFile failed, GLE=") << GetLastError() << endl;

			}
			cout << "InstanceThread: client disconnected.\n" << endl;

			CloseHandleG(hPipe);
			
			break;
		}

		pchRequest[cbBytesRead] = '\0';
		cout << pchRequest << endl;

		Sleep(100);
	}
	return 1;


}

DWORD WINAPI _init(void* data)
{

	while (true)
	{
		HANDLE clientHandle = CreateNamedPipeG("\\\\.\\pipe\\my_pipe_server", BUFFER_SIZE);
		if (clientHandle == INVALID_HANDLE_VALUE)
			continue;


		status = ConnectNamedPipe(clientHandle, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (status) {

			std::cout << clientHandle << std::endl;

			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)InstanceThread, clientHandle, NULL, NULL);
		}
		Sleep(100);
	}


}



int main(const char* args, int size) {




	CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)_init, NULL, NULL, NULL);


	int c = 0;
	std::cin >> c;
	return 0;
}



