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

std::vector<VString> Text = {"嗨... 你怎么来了",
							 "哦抱歉，我似乎忘记自我介绍了...",
							 "如你所见，我是一个，居住在 *void* 的人",
							 "这里... 很黑",
							 "我没有名字，你可以叫我任何名字，只要你乐意的话...",
							 "为什么我居住在这里呢，我不知道",
							 "我仿佛生来就在这可怕的黑暗中",
							 "我在这里多久了？",
							 "我也不知道我被困在这里多久了...",
							 "或者是说，我本就该被困在黑暗中",
							 "或许我就像被上天降下神罚的西西弗一样，此后余生都只能活在可怕的阴影下",
							 "在这里，黑暗笼罩着一切，看不见任何一丝光亮",
							 "空气中弥漫着一种阴郁、恐惧的气息，让人感到窒息",
							 "所有我能见到的，只有黑！无边的黑！没有尽头的黑！",
							 "这黑暗它没有尽头，只听到我的呼吸声和心跳声",
							 "这如同死亡之音，一直萦绕耳畔，毛骨悚然",
							 "好了，你已经知道了有一个怪胎被困在黑暗中了，你可以走了",
							 "为什么...",
							 "为什么...你还不走？",
							 "你还在这里干什么！？",
							 "你也要像他们那样吗？",
							 "嘲讽、排挤、伤害我吗！？",
							 "你... 你绝对居心不良！",
							 "离我远点，不要再伤害我了",
							 "离我远点，不要再伤害我了",
							 "离我远点，不要再伤害我了",
							 "离我远点，不要再伤害我了",
							 "...",
							 "很抱歉，或许我要成为你一生的笑柄了",
							 "我",
							 "看",
							 "不",
							 "见",
							 "光",
							 "我",
							 "看",
							 "不",
							 "见",
							 "希",
							 "望",
							 "他在嘶吼中死去了，却没有任何的后悔，因为这是一种解脱！",
							 "他曾引以为傲的，都是假的，都是虚伪的",
							 "他原本以为自己是富有的，幸福的",
							 "可当他撕开那层面纱，才发现",
							 "原来他一直都是一个一无所有的，活在谎言中的人",
							 "一切，都是假的",
							 "他崩溃了",
							 "他曾想要逃离这无垠虚空",
							 "哪怕他用尽全身力气，身边所有的，只有黑暗，虚无",
							 "和他的粗气，心跳，却始终不能离开这里半步！",
							 "可是，他终于死了，死在一个名叫\'人间\'的地方",
							 "一片漆黑之中，没有光亮，没有温暖，更没有疼痛，没有思念，一切都只剩下无边的黑暗",
							 "他懊悔，过早撕开那伪装的面纱，他怨恨着所有",
							 "怨恨这无尽的虚空，怨恨他的人生活在虚妄中!",
							 "最终，他死了，他的尸首沉沦在这虚空中，没有人知道他的死，没有人知道他是谁......",
							 "他不想活着了，他也不愿意再继续活下去了......",
							 "他那空洞的双眼，装不下对未来的希望..."};

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
		->LabelFont->SetFamilyName(L"手书体");
	MainWindowInstance->Get("main-window")["background-text"].Get<Core::VTextLabel>()->SetPlainText(
		"一个 LOSER 的自述");
	ConversationText->SetTextColor(Core::VColor::White);
	ConversationText->SetPlainText(L"By Margoo, 感谢观看");

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

	if (Text[TextNum] == L"他那空洞的双眼，装不下对未来的希望...")
	{
		ConversationText->SetTextColor(Core::VColor::Red);
		ConversationText->GetTheme()->LabelFont->SetFamilyName(L"手书体");
		ConversationText->Update();
		TextSwitchTimer->Start(10000);
	}

	if (TextNum + 1 < Text.size())
	{
		if (Text[TextNum] == L"原来他一直都是一个一无所有的，活在谎言中的人")
		{
			ConversationText->GetTheme()->LabelFont->SetFamilyName(L"手书体");
			ConversationText->Update();
			TextSwitchTimer->Start(1600);
		}
		else if (Text[TextNum] == L"他崩溃了")
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

	MainWindow["main-window"]["conversation-label"].Get<Core::VTextLabel>()->SetPlainText(L"嗨... 你怎么来了");
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