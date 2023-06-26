#include "../../../include/vml/vmlwidget.h"

Core::VSmartTimer		 *Timer;
Core::VTextLabel		 *Text;
Core::VPositionAnimation *MoveInAnmiaton;
Core::VOpacityAnimation	 *OpacityAnimation;
Core::VPositionAnimation *MoveOutAnmiaton;
Core::VOpacityAnimation	 *FadeOutAnimation;
Core::VPositionAnimation *TextMoveInAnmiaton;
std::wstring			  PlaneText  = L"Tired of life? You can end it. :)";
std::wstring			  LetsGoText =
	L"I am sorry to hear that you are feeling like this way.\n"
	L"However, it's important for you to know that sucide is not the answer to your problems.\n"
	L"And there is always hope and help available.\n"
	L"I can't Be in the same situation just like you now standing.\n"
	L"But I still want to let the things a little bit change.\n"
	L"I believe that if you don't have any excpeted of your life, you won't appear here, and listen my such words\n"
	L"Whether my words can help you or not, I still want to spread my warm to you.\n"
	L"What I actually want to express is\n\t\"Never give up.\"\nI wish my words can make things even a little bit different.\n\t\t\t\tSiYu. Zhang";
std::wstring NoYetText = L"😮 Wow, dude, I am happy that you can still fight against the situation you now facing. \n"
						 L"Now you here, I want to tell you that one thing.\n"
						 L"The world isn't perfect, but still wonderful.\n"
						 L"Please catch your breath, and fight with the fate.\n"
						 L"Now, please close this app, and enjoy the beautiful life you own.\n"
						 L"I am always here, if you need.\n\t\t\t\tSiYu. Zhang";

void OnTime()
{
	Text->SetPlainText(Text->GetPlainText().substr(0, Text->GetPlainText().size() - 1));
	Text->SetPlainText(Text->GetPlainText() + PlaneText[Text->GetPlainText().size()] + L"_");

	if (Text->GetPlainText().size() < PlaneText.size() + 1)
	{
		Timer->Start(rand() % 40 + 210);
	}
	else
	{
		Text->SetPlainText(PlaneText);

		MoveInAnmiaton->Start();
		OpacityAnimation->Start();
	}
}

void NotYetText()
{
	Text->SetPlainText(Text->GetPlainText().substr(0, Text->GetPlainText().size() - 1));
	Text->SetPlainText(Text->GetPlainText() + NoYetText[Text->GetPlainText().size()] + L"_");

	if (Text->GetPlainText().size() < NoYetText.size() + 1)
	{
		Timer->Start(rand() % 40 + 210);
	}
	else
	{
		Text->SetPlainText(NoYetText);
	}
}
void LetGoText()
{
	Text->SetPlainText(Text->GetPlainText().substr(0, Text->GetPlainText().size() - 1));
	Text->SetPlainText(Text->GetPlainText() + LetsGoText[Text->GetPlainText().size()] + L"_");

	if (Text->GetPlainText().size() < LetsGoText.size() + 1)
	{
		Timer->Start(rand() % 40 + 210);
	}
	else
	{
		Text->SetPlainText(LetsGoText);
	}
}

void NoYet()
{
	MoveOutAnmiaton->Start();
	FadeOutAnimation->Start();
	TextMoveInAnmiaton->Start();

	Text->SetPlainText(Text->GetPlainText().substr(0, Text->GetPlainText().size() - 1));
	Text->SetPlainText(Text->GetPlainText() + PlaneText[Text->GetPlainText().size()] + L"_");

	Text->SetPlainText(L"");
	Text->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	Timer->OnTime.Disconnect(OnTime);
	Timer->OnTime.Connect(NotYetText);
	Timer->Start(400);

	Text->Resize(600, 480);
}
void LetsGo()
{
	MoveOutAnmiaton->Start();
	FadeOutAnimation->Start();
	TextMoveInAnmiaton->Start();

	Text->SetPlainText(Text->GetPlainText().substr(0, Text->GetPlainText().size() - 1));
	Text->SetPlainText(Text->GetPlainText() + PlaneText[Text->GetPlainText().size()] + L"_");

	Text->SetPlainText(L"");
	Text->SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	Timer->OnTime.Disconnect(OnTime);
	Timer->OnTime.Connect(LetGoText);
	Timer->Start(400);

	Text->Resize(600, 680);
}

int main()
{
	Core::VApplication App;
	VML::VMLMainWindow MainWindow(640, 480, &App);

	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(NoYet));
	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(LetsGo));

	MainWindow.LoadVML(L"./main.xml", VML::VMLParserParseMode::FromFile);

	Timer = new Core::VSmartTimer(&App);
	Text  = MainWindow[L"mainwindow"][L"main-text"].Get<Core::VTextLabel>();

	MoveInAnmiaton	 = MainWindow[L"mainwindow"][L"selection-box"][L"move-in-animation"].Get<Core::VPositionAnimation>();
	OpacityAnimation =
		MainWindow[L"mainwindow"][L"selection-box"][L"transparent-animation"].Get<Core::VOpacityAnimation>();
	MoveOutAnmiaton =
		MainWindow[L"mainwindow"][L"selection-box"][L"move-out-animation"].Get<Core::VPositionAnimation>();
	FadeOutAnimation =
		MainWindow[L"mainwindow"][L"selection-box"][L"fade-out-animation"].Get<Core::VOpacityAnimation>();
	TextMoveInAnmiaton =
		MainWindow[L"mainwindow"][L"main-text"][L"move-in-animation"].Get<Core::VPositionAnimation>();

	Timer->OnTime.Connect(OnTime);
	Timer->Start(rand() % 50 + 210);

	return App.Exec();
}
