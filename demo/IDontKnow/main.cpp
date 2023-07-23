#include "../../include/vml/vmlwidget.h"

#include <random>

#pragma comment(lib, "winmm.lib")

VML::VMLMainWindow *MainWindowInstance;

Core::VPositionAnimation *ShakingAnmiation;

Core::VImage *CringFace1;
Core::VImage *CringFace2;

int ShakeDealy = 600;
int ShakeDelta = 0;

double Random(const double &Start, const double &End)
{
	static std::random_device RandomDevice;
	static std::mt19937		  Generator(RandomDevice());

	std::uniform_real_distribution<double> Distribution(Start - ShakeDelta, End - ShakeDelta);

	return Distribution(Generator);
}

void ResetShakingAnmiation()
{
	int Token1 = (rand() % 2) ? 1 : -1;
	int Token2 = (rand() % 2) ? 1 : -1;

	ShakingAnmiation->SetTargetPosition(Core::VPoint(Token1 * Random(80, 100), Token2 * Random(80, 100)));
	ShakingAnmiation->SetDuration(ShakeDealy);
	ShakingAnmiation->Start();
}

struct PerlinCurve : public Core::VAnimationCurve
{
	PerlinCurve()
	{
	}

	double CurveFunction(const double &X) override
	{
		return 6 * pow(X, 5) - 15 * pow(X, 4) + 10 * pow(X, 3);
	}
};

std::vector<VString> Text = {"��... ����ô����",
							 "Ŷ��Ǹ�����ƺ��������ҽ�����...",
							 "��������������һ������ס�� *void* ����",
							 "����... �ܺ�",
							 "��û�����֣�����Խ����κ����֣�ֻҪ������Ļ�...",
							 "Ϊʲô�Ҿ�ס�������أ��Ҳ�֪��",
							 "�ҷ·�������������µĺڰ���",
							 "�����������ˣ�",
							 "��Ҳ��֪���ұ�������������...",
							 "������˵���ұ��͸ñ����ںڰ���",
							 "�����Ҿ������콵���񷣵�������һ�����˺�������ֻ�ܻ��ڿ��µ���Ӱ��",
							 "������ڰ�������һ�У��������κ�һ˿����",
							 "������������һ���������־����Ϣ�����˸е���Ϣ",
							 "�������ܼ����ģ�ֻ�кڣ��ޱߵĺڣ�û�о�ͷ�ĺڣ�",
							 "��ڰ���û�о�ͷ��ֻ�����ҵĺ�������������",
							 "����ͬ����֮����һֱ���ƶ��ϣ�ë���Ȼ",
							 "���ˣ����Ѿ�֪������һ����̥�����ںڰ����ˣ����������",
							 "Ϊʲô...",
							 "Ϊʲô...�㻹���ߣ�",
							 "�㻹�������ʲô����",
							 "��ҲҪ������������",
							 "�����ż����˺����𣡣�",
							 "��... ����Ծ��Ĳ�����",
							 "����Զ�㣬��Ҫ���˺�����",
							 "����Զ�㣬��Ҫ���˺�����",
							 "����Զ�㣬��Ҫ���˺�����",
							 "����Զ�㣬��Ҫ���˺�����",
							 "...",
							 "�ܱ�Ǹ��������Ҫ��Ϊ��һ����Ц����",
							 "��",
							 "��",
							 "��",
							 "��",
							 "��",
							 "��",
							 "��",
							 "��",
							 "��",
							 "ϣ",
							 "��",
							 "����˻������ȥ�ˣ�ȴû���κεĺ�ڣ���Ϊ����һ�ֽ��ѣ�",
							 "��������Ϊ���ģ����Ǽٵģ�������α��",
							 "��ԭ����Ϊ�Լ��Ǹ��еģ��Ҹ���",
							 "�ɵ���˺���ǲ���ɴ���ŷ���",
							 "ԭ����һֱ����һ��һ�����еģ����ڻ����е���",
							 "һ�У����Ǽٵ�",
							 "��������",
							 "������Ҫ�������������",
							 "�������þ�ȫ��������������еģ�ֻ�кڰ�������",
							 "�����Ĵ�����������ȴʼ�ղ����뿪����벽��",
							 "���ǣ����������ˣ�����һ������\'�˼�\'�ĵط�",
							 "һƬ���֮�У�û�й�����û����ů����û����ʹ��û��˼�һ�ж�ֻʣ���ޱߵĺڰ�",
							 "���ûڣ�����˺����αװ����ɴ����Թ��������",
							 "Թ�����޾�����գ�Թ��������������������!",
							 "���գ������ˣ�����ʬ�׳�����������У�û����֪����������û����֪������˭......",
							 "����������ˣ���Ҳ��Ը���ټ�������ȥ��......",
							 "���ǿն���˫�ۣ�װ���¶�δ����ϣ��..."};

int TextNum = 0;

Core::VSmartTimer		*TextSwitchTimer;
Core::VTextLabel		*ConversationText;
Core::VSmartTimer		*CryFaceSwitchTimer;
Core::VOpacityAnimation *FadeInAnimation;
Core::VOpacityAnimation *FadeOutAnimation;
Core::VSmartTimer		*EndTimer;

bool SwitchFlag = false;

void SwitchCryFace()
{
	if (!SwitchFlag)
	{
		MainWindowInstance->Get("main-window")["eye-container"]["left-eye"].Get<Core::VImageLabel>()->SetImage(
			CringFace2);
		MainWindowInstance->Get("main-window")["eye-container"]["right-eye"].Get<Core::VImageLabel>()->SetImage(
			CringFace2);
	}
	else
	{
		MainWindowInstance->Get("main-window")["eye-container"]["left-eye"].Get<Core::VImageLabel>()->SetImage(
			CringFace1);
		MainWindowInstance->Get("main-window")["eye-container"]["right-eye"].Get<Core::VImageLabel>()->SetImage(
			CringFace1);
	}

	SwitchFlag = !SwitchFlag;

	CryFaceSwitchTimer->Start(ShakeDealy);
}
void InitCryFace()
{
	MainWindowInstance->Get("main-window")["eye-container"]["fade-in-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
	MainWindowInstance->Get("main-window")["eye-container"]["left-eye"].Get<Core::VImageLabel>()->SetImage(CringFace1);
	MainWindowInstance->Get("main-window")["eye-container"]["right-eye"].Get<Core::VImageLabel>()->SetImage(CringFace1);

	CryFaceSwitchTimer->Start(200);
	CryFaceSwitchTimer->OnTime.Connect(SwitchCryFace);
}
void End()
{
	MainWindowInstance->Get("main-window")["background-text"]
		.Get<Core::VTextLabel>()
		->GetTheme()
		->LabelFont->SetFamilyName(L"������");
	MainWindowInstance->Get("main-window")["background-text"].Get<Core::VTextLabel>()->SetPlainText(
		"һ�� LOSER ������");
	ConversationText->SetTextColor(Core::VColor::White);
	ConversationText->SetPlainText(L"By Margoo, ��л�ۿ�");

	MainWindowInstance->Get("main-window")["background-text"][L"fade-in-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetDuration(4000);
	MainWindowInstance->Get("main-window")["background-text"][L"fade-in-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
	FadeInAnimation->Start();
}
void SwitchPlainText()
{
	if (TextNum + 1 > Text.size())
	{
		MainWindowInstance->Get("main-window")["background-text"][L"fade-out-animation"]
			.Get<Core::VOpacityAnimation>()
			->SetDuration(4000);
		MainWindowInstance->Get("main-window")["background-text"][L"fade-out-animation"]
			.Get<Core::VOpacityAnimation>()
			->Start();
		FadeOutAnimation->AnimationEnd.Block(SwitchPlainText, true);
		FadeOutAnimation->Start();

		EndTimer->OnTime.Connect(End);
		EndTimer->Start(4000);

		return;
	}

	FadeInAnimation->Start();

	ConversationText->SetPlainText(Text[TextNum]);

	if (Text[TextNum] == L"���ǿն���˫�ۣ�װ���¶�δ����ϣ��...")
	{
		ConversationText->SetTextColor(Core::VColor::Red);
		ConversationText->GetTheme()->LabelFont->SetFamilyName(L"������");
		ConversationText->Update();
		TextSwitchTimer->Start(10000);
	}

	if (TextNum + 1 < Text.size())
	{
		if (Text[TextNum] == L"ԭ����һֱ����һ��һ�����еģ����ڻ����е���")
		{
			ConversationText->GetTheme()->LabelFont->SetFamilyName(L"������");
			ConversationText->Update();
			TextSwitchTimer->Start(1600);
		}
		else if (Text[TextNum] == L"��������")
		{
			ConversationText->GetTheme()->LabelFont->SetFamilyName(L"Microsoft YaHei UI");
			ConversationText->Update();
			TextSwitchTimer->Start(1600);
		}
		else if (TextNum == 16)
		{
			TextSwitchTimer->Start(10000);
		}
		else if (TextNum == 17)
		{
			MainWindowInstance->Get("main-window")["background-text"].Get<Core::VTextLabel>()->SetPlainText("WHY!");
			MainWindowInstance->Get("main-window")["background-text"][L"fade-in-animation"]
				.Get<Core::VOpacityAnimation>()
				->SetDuration(280);
			MainWindowInstance->Get("main-window")["background-text"][L"fade-in-animation"]
				.Get<Core::VOpacityAnimation>()
				->Start();
			MainWindowInstance->Get("main-window")["eye-container"]["fade-out-animation"]
				.Get<Core::VOpacityAnimation>()
				->AnimationEnd.Connect(InitCryFace);
			MainWindowInstance->Get("main-window")["eye-container"]["fade-out-animation"]
				.Get<Core::VOpacityAnimation>()
				->Start();
			TextSwitchTimer->Start(1800);
		}
		else if (TextNum == 23)
		{
			delete CringFace1;
			delete CringFace2;

			CringFace1 =
				new Core::VImage("./resource/eye-red.png", MainWindowInstance->CallWidgetGetStaticRenderHandle());
			CringFace2 =
				new Core::VImage("./resource/eye-red.png", MainWindowInstance->CallWidgetGetStaticRenderHandle());

			MainWindowInstance->Get("main-window")["eye-container"]["left-eye"].Get<Core::VImageLabel>()->SetImage(
				CringFace1);
			MainWindowInstance->Get("main-window")["eye-container"]["right-eye"].Get<Core::VImageLabel>()->SetImage(
				CringFace1);

			MainWindowInstance->Get("main-window")["eye-container"]["fade-out-animation"]
				.Get<Core::VOpacityAnimation>()
				->AnimationEnd.Connect(InitCryFace);
			MainWindowInstance->Get("main-window")["eye-container"]["fade-out-animation"]
				.Get<Core::VOpacityAnimation>()
				->Start();
			TextSwitchTimer->Start(1800);
		}
		else if (TextNum >= 23 && TextNum <= 23)
		{
			ShakeDelta += 20;
			ShakeDealy -= 145;
			TextSwitchTimer->Start(800);
		}
		else if (TextNum == 34)
		{
			MainWindowInstance->Get("main-window")["eye-container"]["fade-out-animation"]
				.Get<Core::VOpacityAnimation>()
				->AnimationEnd.Disconnect(InitCryFace);
			MainWindowInstance->Get("main-window")["eye-container"]["fade-out-animation"]
				.Get<Core::VOpacityAnimation>()
				->Start();
			TextSwitchTimer->Start(2000);
		}
		else
		{
			TextSwitchTimer->Start(1800);
		}
	}

	++TextNum;
}
void SwitchText()
{
	FadeOutAnimation->Start();
}

class ResourceLoaderAndReleaser
{
public:
	ResourceLoaderAndReleaser()
	{
		AddFontResource(L"./resource/hand-write-font.ttf");
	}
	~ResourceLoaderAndReleaser()
	{
		RemoveFontResource(L"./resource/hand-write-font.ttf");
	}
};

int main()
{
	ResourceLoaderAndReleaser ResourceManager;

	Core::VApplication App;
	VML::VMLMainWindow MainWindow(&App);

	MainWindowInstance = &MainWindow;
	EndTimer		   = new Core::VSmartTimer(&App);

	MainWindow.LoadVML("./main.xml", VML::VMLParserParseMode::FromFile);

	ShakingAnmiation =
		new Core::VPositionAnimation(MainWindow["main-window"]["eye-container"].Get(), new PerlinCurve, 0);
	ShakingAnmiation->AnimationEnd.Connect(ResetShakingAnmiation);
	ShakingAnmiation->SetTargetPosition({0, 0});
	ShakingAnmiation->Start();

	TextSwitchTimer	   = new Core::VSmartTimer(&App);
	CryFaceSwitchTimer = new Core::VSmartTimer(&App);
	CringFace1		   = new Core::VImage("./resource/eye-cring.png", MainWindow.CallWidgetGetStaticRenderHandle());
	CringFace2 = new Core::VImage("./resource/eye-cring-second.png", MainWindow.CallWidgetGetStaticRenderHandle());

	MainWindow["main-window"]["conversation-label"].Get<Core::VTextLabel>()->SetPlainText(L"��... ����ô����");
	FadeInAnimation =
		MainWindow["main-window"]["conversation-label"]["fade-in-animation"].Get<Core::VOpacityAnimation>();
	ConversationText = MainWindow["main-window"]["conversation-label"].Get<Core::VTextLabel>();
	FadeOutAnimation =
		MainWindow["main-window"]["conversation-label"]["fade-out-animation"].Get<Core::VOpacityAnimation>();

	TextSwitchTimer->OnTime.Connect(SwitchText);
	TextSwitchTimer->Start(0);

	FadeOutAnimation->AnimationEnd.Connect(SwitchPlainText);

	MainWindow.Show();

	return App.Exec();
}