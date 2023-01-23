#include <map>
#include <stdio.h>
#include <string>
#include <thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

#define _CHECK_RETURN_VALUE_(Exp) if (Exp)

#define LOCAL_SERVER_VERSION 0x10100
#define SERVER_PORT			 8008

struct ChatPackHeader
{
	int ChatVersion;
	int UserNameLength;
};

#define SUCCESS				0
#define ERROR_VERSION		1
#define USER_ALREADY_EXSITS 3

struct ChatBackPack
{
	int ServerCheckCode;
	int MaxUserAmount;
};

#define CLIENT_DISCONNECT 0
#define CLIENT_NEWMESSAGE 1

struct ClientOperationPack
{
	int OperationCode;
	int MessageLength;
};

struct ChatUser
{
	SOCKET *UserSock;
};

#define REMOVED	   0
#define NEWMESSAGE 1

struct ServerContact
{
	int Operation;
	int MessageLength;
	int UserNameLength;
};

int ThrowError(const char *ErrorInfo)
{
	printf("%s\n", ErrorInfo);

	WSACleanup();

	return -1;
}

void PrintInfo(const char *FormatString)
{
	printf("[Server info] : %s\n", FormatString);
}
void PrintInfo(const char *FormatString, const char *List, ...)
{
	printf("[Server info] : ");
	printf(FormatString, List);
	printf("\n");
}
void ThreadPrintInfo(int ThreadID, const char *FormatString)
{
	printf("[Server thread %d] : %s\n", ThreadID, FormatString);
}
void ThreadPrintInfo(int ThreadID, const char *FormatString, const char *List, ...)
{
	printf("[Server thread %d] : ", ThreadID);
	printf(FormatString, List);
	printf("\n");
}

#define MAX_USER 40

template <class Type> std::tuple<Type *, bool> RecData(SOCKET Socket, const int &Length, const int &RecvFlag)
{
	char *Data = new char[Length];

	if (recv(Socket, Data, Length, RecvFlag) != Length)
	{
		return {nullptr, false};
	}

	return {(Type *)Data, true};
}
template <class Type> std::tuple<Type *, bool> RecData(SOCKET Socket, const int &RecvFlag)
{
	char *Data = new char[sizeof(Type)];

	if (recv(Socket, Data, sizeof(Type), RecvFlag) != sizeof(Type))
	{
		return {nullptr, false};
	}

	return {(Type *)Data, true};
}
template <class Type> void SendData(SOCKET Socket, Type *Ptr, const int &Flag)
{
	send(Socket, (char *)Ptr, sizeof(Type), Flag);
}
template <class Type> void SendData(SOCKET Socket, Type *Ptr, const int &Length, const int &Flag)
{
	send(Socket, (char *)Ptr, Length, Flag);
}

int main()
{
	WSAData							 Data;
	std::map<const char *, ChatUser> UserPool;

	_CHECK_RETURN_VALUE_(WSAStartup(MAKEWORD(2, 2), &Data));

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!ServerSocket)
	{
		return ThrowError("Fatal error : Failed to create server web socket!");
	}

	IN_ADDR Address;
	inet_pton(AF_INET, "0.0.0.0", (PVOID)&Address);

	SOCKADDR_IN SocketAddress{AF_INET, 8008, Address};

	if (bind(ServerSocket, (sockaddr *)&SocketAddress, sizeof(SOCKADDR_IN)) != 0)
	{
		return ThrowError("Fatal error : Failed to bind port!");
	}

	if (listen(ServerSocket, MAX_USER) != 0)
	{
		return ThrowError("Fatal error : Failed to listen at socket!");
	}

	PrintInfo("LOCAL MAX USER AMOUNT {%d}", (const char *)MAX_USER);
	PrintInfo("LOCAL SERVER VERISON {0x%x}", (const char *)LOCAL_SERVER_VERSION);
	PrintInfo("SERVER LISTEN AT PORT {%d}", (const char *)SERVER_PORT);

	for (int ThreadCount = 0; ThreadCount < MAX_USER; ++ThreadCount)
	{
		std::thread SockThread(
			[&](const int &ThreadID) -> void {
				while (true)
				{
					ThreadPrintInfo(ThreadID, "Thread started!");

					int			SOCKADDR_StructureLength = sizeof(SOCKADDR);
					SOCKADDR_IN ClientAddress{};
					SOCKET ClientSocket = accept(ServerSocket, (PSOCKADDR)&ClientAddress, &SOCKADDR_StructureLength);

					if (ClientSocket == INVALID_SOCKET)
					{
						ThreadPrintInfo(ThreadID, "Error : Failed to accept a connection!");
					}

					ThreadPrintInfo(ThreadID, "Thread await waiting for connection.");

					ChatPackHeader *PackHeader;
					char		   *Buffer = new char[sizeof(ChatPackHeader)];

					auto RecResult = RecData<ChatPackHeader>(ClientSocket, 0);
					if (!std::get<1>(RecResult))
					{
						ThreadPrintInfo(ThreadID, "Error : Unexpected EOF, the connection with client will shutdown!");
						shutdown(ClientSocket, 2);

						continue;
					}

					auto Header = std::get<0>(RecResult);

					if (Header->ChatVersion != LOCAL_SERVER_VERSION)
					{
						ThreadPrintInfo(ThreadID,
										"Error : Received Incompatible client version {0x%x}, send the error pack to "
										"client, the connection with client will shutdown!",
										(const char *)Header->ChatVersion);

						ChatBackPack ServerBackPack{ERROR_VERSION, -1};
						SendData<ChatBackPack>(ClientSocket, &ServerBackPack, 0);

						shutdown(ClientSocket, 2);

						continue;
					}

					auto UserNameWebPack = RecData<char>(ClientSocket, Header->UserNameLength, 0);
					if (!std::get<1>(UserNameWebPack))
					{
						ThreadPrintInfo(ThreadID, "Error : Unexpected EOF, the connection with client will shutdown!");
						shutdown(ClientSocket, 2);

						continue;
					}

					auto UserName = std::get<0>(UserNameWebPack);
					if (UserPool.find(UserName) != UserPool.end())
					{
						ThreadPrintInfo(ThreadID, "NEW USER {%s} ALREADY EXSITS, DISCONNECT!", (const char *)UserName);

						ChatBackPack ServerBackPack{USER_ALREADY_EXSITS, -1};
						SendData<ChatBackPack>(ClientSocket, &ServerBackPack, 0);
						shutdown(ClientSocket, 2);

						continue;
					}

					UserPool.insert(std::pair<const char *, ChatUser>(UserName, ChatUser{&ClientSocket}));

					ThreadPrintInfo(ThreadID, "NEW USER CONNECTED, NAME : {%s}", (const char *)UserName);

					auto	Port   = ntohs(ClientAddress.sin_port);
					in_addr InAddr = ClientAddress.sin_addr;
					char	IPAddress[INET_ADDRSTRLEN];

					inet_ntop(AF_INET, &InAddr, IPAddress, sizeof(IPAddress));

					ThreadPrintInfo(ThreadID, "USER CONNECTED : {%s:%d}", (const char *)IPAddress, (const char *)Port);

					ChatBackPack ServerBackPack{SUCCESS, MAX_USER};
					SendData<ChatBackPack>(ClientSocket, &ServerBackPack, 0);

					ThreadPrintInfo(ThreadID, "Sent success back pack to user {%s}", (const char *)UserName);

					while (true)
					{
						auto OperatPack = RecData<ClientOperationPack>(ClientSocket, 0);
						if (!std::get<1>(OperatPack))
						{
							continue;
						}

						auto ClientOperation = std::get<0>(OperatPack);
						if (ClientOperation->OperationCode == CLIENT_DISCONNECT)
						{
							ThreadPrintInfo(ThreadID, "User {%s} disconnected", (const char *)UserName);
							shutdown(ClientSocket, 2);

							UserPool.erase(UserName);

							break;
						}

						if (ClientOperation->OperationCode == CLIENT_NEWMESSAGE)
						{
							ThreadPrintInfo(
								ThreadID,
								"User {%s} try to send message, now trying to received message from client...",
								(const char *)UserName);

							auto UserSentMessage = RecData<char>(ClientSocket, ClientOperation->MessageLength, 0);

							if (!std::get<1>(UserSentMessage))
							{
								ThreadPrintInfo(ThreadID,
												"Error : User {%s} tried to send message, but server received a "
												"unexpected web pack, the connection will shutdown!",
												(const char *)UserName);

								UserPool.erase(UserName);

								shutdown(ClientSocket, 2);

								break;
							}

							auto UserMessage = std::get<0>(UserSentMessage);
							printf("[Server thread %d] : <%s> : %s\nPack length : %d\n", ThreadID, UserName,
								   UserMessage, ClientOperation->MessageLength);

							auto UserNameLength = strlen(UserName) + 1;

							ServerContact ServerContactPack{NEWMESSAGE, ClientOperation->MessageLength, UserNameLength};

							for (auto &User : UserPool)
							{
								SendData<ServerContact>(*User.second.UserSock, &ServerContactPack, 0);
								SendData<char>(*User.second.UserSock, UserMessage, ClientOperation->MessageLength, 0);
								SendData<char>(*User.second.UserSock, UserName, UserNameLength, 0);
							}
						}
					}

					shutdown(ClientSocket, 2);
				}
			},
			ThreadCount);
		SockThread.detach();
	}

	getchar();

	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}