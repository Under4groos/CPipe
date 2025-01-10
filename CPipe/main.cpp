
#include "Headers.h"



//#define PIPE_BUFFER_SIZE 4096
//#define PIPE_SLEEP 1000
//HANDLE CreateNamedPipeG(LPCSTR lpName, int size) {
//	return CreateNamedPipeA(lpName,
//		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
//		0,
//		255,
//		size,
//		size,
//		NULL,
//		NULL);
//}
//
//void CloseHandleG(HANDLE clientHandle) {
//	DisconnectNamedPipe(clientHandle);
//	CloseHandle(clientHandle);
//}

//DWORD WINAPI InstanceThread(HANDLE hPipe) {
//	if (hPipe == NULL)
//		return -1;
//	HANDLE hHeap = GetProcessHeap();
//	TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, PIPE_BUFFER_SIZE * sizeof(TCHAR));
//	TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, PIPE_BUFFER_SIZE * sizeof(TCHAR));
//	BOOL fSuccess = FALSE;
//	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
//	while (true)
//	{
//		fSuccess = ReadFile(
//			hPipe,
//			pchRequest,
//			PIPE_BUFFER_SIZE * sizeof(TCHAR),
//			&cbBytesRead,
//			NULL);
//		if (!fSuccess || cbBytesRead == 0) {
//			if (GetLastError() == ERROR_BROKEN_PIPE)
//			{
//				cout << "InstanceThread: client disconnected.\n" << endl;
//			}
//			else
//			{
//				cout << TEXT("InstanceThread ReadFile failed, GLE=") << GetLastError() << endl;
//			}
//			cout << "InstanceThread: client " << hPipe << " disconnected.\n" << endl;
//			CloseHandleG(hPipe);
//			break;
//		}
//		pchRequest[cbBytesRead] = '\0';
//
//		_message(pchRequest);
//
//		pchReply = pchRequest;
//		fSuccess = WriteFile(
//			hPipe,
//			pchReply,
//			sizeof(pchReply),
//			&cbWritten,
//			NULL);
//
//		if (!fSuccess || cbReplyBytes != cbWritten)
//		{
//			//cout << "InstanceThread WriteFile failed, GLE=%d" << GetLastError() << endl;
//			continue;
//		}
//
//
//		Sleep(100);
//	}
//	return 1;
//}


//DWORD WINAPI _mainPipe(void* data)
//{
//
//	bool status = FALSE;
//	while (true)
//	{
//		HANDLE clientHandle = CreateNamedPipeG("\\\\.\\pipe\\my_pipe_server", PIPE_BUFFER_SIZE);
//		if (clientHandle == INVALID_HANDLE_VALUE)
//			continue;
//		status = ConnectNamedPipe(clientHandle, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
//		if (status) {
//
//			std::cout << "Connected: " << clientHandle << std::endl;
//
//			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)InstanceThread, clientHandle, NULL, NULL);
//		}
//		Sleep(PIPE_SLEEP);
//	}
//}
//
//void CreatePipServer() {
//	CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)_mainPipe, NULL, NULL, NULL);
//}






#include "PipServer.h";


int main(const char* args, int size) {

	PipServer server{};
	server.MessageReceived = (EventMessageReceived)[](HANDLE hwnd, std::string data)
	{
		cout << data << endl;
	};
	server.MessageConnected = (EventMessageConnected)[](HANDLE hwnd)
	{
		cout << "Connect: " << hwnd << endl;
	};
	server.Init();


	int c = 0;
	std::cin >> c;
	return 0;
}



