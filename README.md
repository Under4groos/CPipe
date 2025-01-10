# CPipe

Pipe server.
```C++
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
```