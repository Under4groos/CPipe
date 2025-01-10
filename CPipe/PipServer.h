#pragma once
#include <Windows.h>
#include <string>
#include <thread>



typedef void (*EventMessageErrors)(std::string msg);
typedef void (*EventMessageReceived)(HANDLE hwnd, std::string msg);
typedef void (*EventMessageConnected)(HANDLE hwnd);
class PipServer
{
public:
	int BUFFER_SIZE = 4096;

	int SLEEP = 1000;

	std::string NamePipe = "\\\\.\\pipe\\my_pipe_server";
	EventMessageErrors MessageErrors = NULL;
	EventMessageReceived MessageReceived = NULL;
	EventMessageConnected MessageConnected = NULL;

	PipServer() {

		
	}

	void Init() {
		_init();
	}



	 
	void InstanceThread(HANDLE hPipe , EventMessageReceived messageReceived) {
		if (hPipe == NULL)
			return;
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

					MessageErrors("InstanceThread: client disconnected.");
				}
				else
				{

					cout << TEXT("InstanceThread ReadFile failed, GLE=") << GetLastError() << endl;
				}
				cout << "InstanceThread: client " << hPipe << " disconnected.\n" << endl;
				CloseHandleG(hPipe);
				break;
			}
			pchRequest[cbBytesRead] = '\0';



			if (messageReceived)
				messageReceived(hPipe, pchRequest);

			pchReply = pchRequest;
			fSuccess = WriteFile(
				hPipe,
				pchReply,
				sizeof(pchReply),
				&cbWritten,
				NULL);

			if (!fSuccess || cbReplyBytes != cbWritten)
			{
				 
				//cout << "InstanceThread WriteFile failed, GLE=%d" << GetLastError() << endl;
				continue;
			}


			Sleep(100);
		}
		return;
	}
	void _init() {
		bool status = FALSE;
		while (true)
		{
			HANDLE clientHandle = CreateNamedPipeG(NamePipe.c_str(), BUFFER_SIZE);
			if (clientHandle == INVALID_HANDLE_VALUE)
				continue;
			status = ConnectNamedPipe(clientHandle, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
			if (status) {

				if (MessageConnected)
					MessageConnected(clientHandle);

				auto functor =
					[this](HANDLE clientHandle , EventMessageReceived messageReceived) ->void
					{
						InstanceThread(clientHandle , messageReceived);
					
					};
				std::thread(functor , clientHandle , MessageReceived).detach();

			 
			}
			Sleep(SLEEP);
		}
	}

private:
	void CloseHandleG(HANDLE clientHandle) {
		DisconnectNamedPipe(clientHandle);
		CloseHandle(clientHandle);
	}




	HANDLE CreateNamedPipeG(LPCSTR lpName, int size) {
		return CreateNamedPipeA(lpName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			0,
			255,
			size,
			size,
			NULL,
			NULL);
	}

};
