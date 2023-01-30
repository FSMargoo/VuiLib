#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "../../../include/vml/vmlwidget.h"
#include <comutil.h>
#include <regex>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "comsuppw")
#pragma comment(lib, "ws2_32")

#define _CHECK_RETURN_VALUE_(Exp)                                                                                      \
	if (Exp)                                                                                                           \
	{                                                                                                                  \
	}

#define LOCAL_CLIENT_VERSION 0x10100
#define SERVER_PORT			 8008

struct ChatPackHeader
{
	int ChatVersion;
	int UserNameLength;
};

#define SUCCESS				0
#define ERROR_VERSION		1
#define USER_ALREADY_EXISTS 3

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

struct ChatMessage
{
	std::wstring TimeString;
	std::wstring UserNameString;
	std::wstring MessageString;
};

class ChatRoomClient : public VML::VMLMainWindow
{
private:
	SOCKET ClientSocket;

	std::string UserName;

	bool MessageNeedSend;
	bool ResetMessageLabel;

	Core::VSolidBrush UserBrush;
	Core::VSolidBrush TimeBrush;
	Core::VSolidBrush MessageBrush;

	std::vector<ChatMessage> Messages;

private:
	bool JudgetEmpty()
	{
		auto EdtiorInstance = this->operator[](L"main-widget")[L"main-ui"][L"editor"].Get<Core::VEditor>();
		auto						PlaneText = EdtiorInstance->GetPlaneText();

		if (PlaneText.empty())
		{
			return true;
		}

		bool Flag = true;
		for (auto Character : PlaneText)
		{
			if (Character != L'\n' && Character != L'\r' && Character != L'\t' && Character != L' ')
			{
				Flag = false;
			}
		}

		return Flag;
	}
	void FirstContact()
	{
		auto		   UserNameLength = UserName.size() + 1;
		ChatPackHeader PackHeader{LOCAL_CLIENT_VERSION, UserNameLength};

		SendData<ChatPackHeader>(ClientSocket, &PackHeader, 0);
		SendData<char>(ClientSocket, (char *)UserName.c_str(), UserNameLength, 0);
		auto BackHeader = RecData<ChatBackPack>(ClientSocket, 0);
		if (!std::get<1>(BackHeader))
		{
			shutdown(ClientSocket, 2);

			exit(-1);
		}

		auto BackPack = std::get<0>(BackHeader);

		if (BackPack->ServerCheckCode == SUCCESS)
		{
			this->operator[](L"main-widget")[L"login-interface"].Get()->Hide();
			this->operator[](L"main-widget")[L"main-ui"].Get()->Show();

			std::thread WebSocketThread([&]() -> void {
				int TimeOut = 400;
				setsockopt(ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(int));

				while (true)
				{
					auto ServerContactPack = RecData<ServerContact>(ClientSocket, 0);
					if (std::get<1>(ServerContactPack))
					{
						auto Contact = std::get<0>(ServerContactPack);

						if (Contact->Operation == NEWMESSAGE)
						{
							auto Message = RecData<char>(ClientSocket, Contact->MessageLength, 0);

							if (std::get<1>(Message))
							{
								auto MessageString = std::get<0>(Message);
								auto UserName	   = RecData<char>(ClientSocket, Contact->UserNameLength, 0);

								if (std::get<1>(UserName))
								{
									auto UserNameString = std::get<0>(UserName);
									printf("<%s> : %s\n", UserNameString, MessageString);

									auto LocalTime = std::chrono::system_clock::now();

									auto	CTimeStructure = std::chrono::system_clock::to_time_t(LocalTime);
									tm		TimeData;
									wchar_t TimeFormat[1024];
									localtime_s(&TimeData, &CTimeStructure);
									wsprintf(TimeFormat, L"%d-%02d-%02d %02d:%02d:%02d", TimeData.tm_year + 1900,
											 TimeData.tm_mon + 1, TimeData.tm_mday, TimeData.tm_hour, TimeData.tm_min,
											 TimeData.tm_sec);

									_bstr_t	 ConvertString1	  = UserNameString;
									wchar_t *UserNameWideChar = ConvertString1;
									_bstr_t	 ConvertString2	  = MessageString;
									wchar_t *MessageWideChar  = ConvertString2;

									Messages.push_back(ChatMessage{TimeFormat, UserNameWideChar, MessageWideChar});

									ResetMessageLabel = true;
								}
							}
						}
					}

					if (MessageNeedSend)
					{
						MessageNeedSend = false;

						if (JudgetEmpty())
						{
							continue;
						}

						auto	  EdtiorInstance =
							this->operator[](L"main-widget")[L"main-ui"][L"editor"].Get<Core::VEditor>();

						_bstr_t		MessageConvertor = EdtiorInstance->GetPlaneText().c_str();
						std::string MessageCppObject = (const char *)MessageConvertor;

						ClientOperationPack OperationPack{CLIENT_NEWMESSAGE, MessageCppObject.size() + 1};

						char *MessageString = new char[MessageCppObject.size() + 1];

						for (int Count = 0; Count < MessageCppObject.size() + 1; ++Count)
						{
							MessageString[Count] = MessageCppObject[Count];
						}

						MessageString[MessageCppObject.size()] = '\0';

						SendData<ClientOperationPack>(ClientSocket, &OperationPack, 0);
						SendData<char>(ClientSocket, MessageString, MessageCppObject.size() + 1, 0);

						EdtiorInstance->SetPlaneText(L"");
					}
				}
			});

			WebSocketThread.detach();
		}
		else if (BackPack->ServerCheckCode == ERROR_VERSION)
		{
			this->operator[](L"main-widget")[L"login-interface"][L"login-block"][L"error-text"]
				.Get<Core::VTextLabel>()
				->SetPlaneText(L"The version of client is not compatible with the server's");
		}
		else if (BackPack->ServerCheckCode == USER_ALREADY_EXISTS)
		{
			this->operator[](L"main-widget")[L"login-interface"][L"login-block"][L"error-text"]
				.Get<Core::VTextLabel>()
				->SetPlaneText(L"The user name already exists!");
		}
	}
	void LoginToServer()
	{
		_bstr_t IPAddress = this->operator[](L"main-widget")[L"login-interface"][L"login-block"][L"server-ip-editor"]
								.Get<Core::VLineEditor>()
								->GetPlaneText()
								.c_str();
		_bstr_t UserString = this->operator[](L"main-widget")[L"login-interface"][L"login-block"][L"user-name-editor"]
								 .Get<Core::VLineEditor>()
								 ->GetPlaneText()
								 .c_str();

		UserName = (const char *)UserString;

		this->operator[](L"main-widget")[L"login-interface"][L"login-block"][L"error-text"]
			.Get<Core::VTextLabel>()
			->SetPlaneText(L"");

		std::string IPString = (const char *)IPAddress;

		auto SplitSymbol = IPString.find(":");
		auto Port		 = IPString.substr(SplitSymbol + 1, IPString.size() - SplitSymbol - 1);
		auto IP			 = IPString.substr(0, SplitSymbol);

		std::regex IPRegx("(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])[.](25[0-5]|2[0-4]["
						  "0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.]"
						  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-4]|2[0-4]["
						  "0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])");
		if (!std::regex_match(IP, IPRegx))
		{
			auto Host = gethostbyname(IP.c_str());
			IP		  = inet_ntoa(*(struct in_addr *)Host->h_addr_list[0]);
		}

		IN_ADDR InAddress;
		inet_pton(AF_INET, IP.c_str(), (PVOID)&InAddress);
		SOCKADDR_IN Address;
		Address.sin_family = AF_INET;
		Address.sin_port   = atoi(Port.c_str());
		Address.sin_addr   = InAddress;
		if (connect(ClientSocket, (PSOCKADDR)&Address, sizeof(SOCKADDR_IN)) != 0)
		{
			this->operator[](L"main-widget")[L"login-interface"][L"login-block"][L"error-text"]
				.Get<Core::VTextLabel>()
				->SetPlaneText(L"Failed to connect to server!");

			return;
		}

		FirstContact();
	}
	void SendChatMessage()
	{
		MessageNeedSend = true;
	}
	void CheckFrame() override
	{
		VMLMainWindow::CheckFrame();

		if (ResetMessageLabel)
		{
			ResetMessageLabel = false;

			for (auto &Message : Messages)
			{
				auto EdtiorInstance = this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>();
				auto						PlaneText = EdtiorInstance->GetPlaneText();

				auto LocalTime = std::chrono::system_clock::now();

				auto	CTimeStructure = std::chrono::system_clock::to_time_t(LocalTime);
				tm		TimeData;
				wchar_t TimeFormat[1024];
				localtime_s(&TimeData, &CTimeStructure);
				wsprintf(TimeFormat, L"%d-%02d-%02d %02d:%02d:%02d", TimeData.tm_year + 1900, TimeData.tm_mon + 1,
						 TimeData.tm_mday, TimeData.tm_hour, TimeData.tm_min, TimeData.tm_sec);

				this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>()->TextEffect.push_back(
					{(ID2D1Effect *)UserBrush.GetDxBrush(),
					 DWRITE_TEXT_RANGE{(UINT32)PlaneText.size(), (UINT32)Message.UserNameString.size() + 1}});

				PlaneText = PlaneText + L"\n" + Message.UserNameString;

				this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>()->TextEffect.push_back(
					{(ID2D1Effect *)TimeBrush.GetDxBrush(),
					 DWRITE_TEXT_RANGE{(UINT32)PlaneText.size(), (UINT32)lstrlen(TimeFormat) + 1}});

				PlaneText = PlaneText + L"\t" + TimeFormat;

				this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>()->TextEffect.push_back(
					{(ID2D1Effect *)MessageBrush.GetDxBrush(),
					 DWRITE_TEXT_RANGE{(UINT32)PlaneText.size(), (UINT32)Message.MessageString.size() + 1}});

				PlaneText = PlaneText + L"\n" + Message.MessageString;

				this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>()->SetPlaneText(
					PlaneText);
				this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>()->ScrollToEnd();
				this->operator[](L"main-widget")[L"main-ui"][L"chat-editor"].Get<Core::VEditor>()->ResetTextLayout();
			}

			Messages.clear();
		}
	}

public:
	ChatRoomClient(Core::VApplication *App)
		: VMLMainWindow(App), UserBrush(Core::VColor::White, CallWidgetGetStaticRenderHandle()),
		  TimeBrush(Core::VColor(1.f, 1.f, 1.f, 0.31f), CallWidgetGetStaticRenderHandle()),
		  MessageBrush(Core::VColor(1.f, 1.f, 1.f, 0.52f), CallWidgetGetStaticRenderHandle()), ResetMessageLabel(false)
	{
		static WSADATA WSAData;
		_CHECK_RETURN_VALUE_(WSAStartup(MAKEWORD(2, 2), &WSAData));

		MessageNeedSend = false;

		RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &ChatRoomClient::LoginToServer, LoginToServer));
		RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &ChatRoomClient::SendChatMessage, SendChatMessage));

		LoadVML(L"./mainui.xml", VML::VMLParserParseMode::FromFile);

		ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (!ClientSocket)
		{
			exit(-1);
		}
	}
};

int main()
{
	Core::VApplication App;
	ChatRoomClient	   Client(&App);

	Client.Show();

	return App.Exec();
}