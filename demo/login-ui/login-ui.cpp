#include "login-ui.h"

LoginUI::LoginUI(Core::VApplication *App) : VML::VMLMainWindow(App)
{
	RegisterMetaFunction(VML_CLASS_META_FUNCTION(this, &LoginUI::ValidCheck, ValidCheck));

	auto LoadResult = LoadVML(L"./main.xml", VML::VMLParserParseMode::FromFile);

	if (LoadResult.Status != VML::VMLWidgetVMLLoadStats::Ok)
	{
		printf("Failed to load vml from file reason : %ws", LoadResult.FailedMessage.CStyleString());

		exit(-1);
	}

	Show();

	LoginInterface();
}

void LoginUI::ValidCheck(const wchar_t &Characeter, bool *AllowFlag)
{
	*AllowFlag = true;

	if (Characeter == L'\r' || Characeter == L'\n' ||
		Get("mainwindow")["login-card"]["inputbox"].Get<Core::VEditor>()->GetPlainText().size() >= 16)
	{
		*AllowFlag = false;
	}
}

void LoginUI::LoginInterface()
{
	Get("mainwindow")["login-card"]["title-text"].Get<Core::VTextLabel>()->SetPlainText("登录");
	Get("mainwindow")["login-card"]["description-text"].Get<Core::VTextLabel>()->SetPlainText("请输入你的账号");

	Get("mainwindow")["login-card"]["next-stage"].Get<Core::VPushButton>()->ButtonPushed.Connect(
		this, &LoginUI::VertifyInterface);

	UIFadeIn();
}
void LoginUI::VertifyInterface()
{
	UIFadeOutWithoutButton();

	Get("mainwindow")["login-card"]["next-stage"].Get<Core::VPushButton>()->ButtonPushed.Block(
		this, &LoginUI::VertifyInterface, true);

	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->AnimationEnd.Connect(this, &LoginUI::SetupVertifyInterface);
}
void LoginUI::SetupVertifyInterface()
{
	UserName = Get("mainwindow")["login-card"]["inputbox"].Get<Core::VEditor>()->GetPlainText();

	Get("mainwindow")["login-card"]["title-text"].Get<Core::VTextLabel>()->SetPlainText("验证码");
	Get("mainwindow")["login-card"]["description-text"].Get<Core::VTextLabel>()->SetPlainText("请输入你的身份验证码");
	Get("mainwindow")["login-card"]["inputbox"].Get<Core::VEditor>()->SetPlainText("");
	Get("mainwindow")["login-card"]["next-stage"].Get<Core::VPushButton>()->ButtonPushed.Connect(
		this, &LoginUI::SetupWelcomeInterface);

	UIFadeInWithoutButton();
}
void LoginUI::WelcomeInterface()
{
	Get("mainwindow")["login-card"]["title-text"].Get<Core::VTextLabel>()->SetPlainText(VStr("欢迎，") + UserName +
																						VStr('#') + VertifyCode);
	Get("mainwindow")["login-card"]["description-text"].Get<Core::VTextLabel>()->SetPlainText(
		"欢迎来到 VuiLib，开始你的旅途吧");
	Get("mainwindow")["login-card"]["inputbox"].Get<Core::VEditor>()->SetPlainText("");

	Get("mainwindow")["login-card"]["title-text"]->Move(Get("mainwindow")["login-card"]["title-text"]->GetX(),
														Get("mainwindow")["login-card"]["title-text"]->GetY() - 80);
	Get("mainwindow")["login-card"]["description-text"]->Move(
		Get("mainwindow")["login-card"]["description-text"]->GetX(),
		Get("mainwindow")["login-card"]["description-text"]->GetY() - 80);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["title-text"]->GetX(),
										 Get("mainwindow")["login-card"]["title-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["description-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["description-text"]->GetX(),
										 Get("mainwindow")["login-card"]["description-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
}
void LoginUI::SetupWelcomeInterface()
{
	VertifyCode = Get("mainwindow")["login-card"]["inputbox"].Get<Core::VEditor>()->GetPlainText();

	UIFadeOut();

	Get("mainwindow")["login-card"]["next-stage"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->AnimationEnd.Connect(this, &LoginUI::WelcomeInterface);
}
void LoginUI::UIFadeIn()
{
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->AnimationEnd.Block(this, &LoginUI::UIFadeIn, true);

	Get("mainwindow")["login-card"]["title-text"]->Move(Get("mainwindow")["login-card"]["title-text"]->GetX(),
														Get("mainwindow")["login-card"]["title-text"]->GetY() - 40);
	Get("mainwindow")["login-card"]["description-text"]->Move(
		Get("mainwindow")["login-card"]["description-text"]->GetX(),
		Get("mainwindow")["login-card"]["description-text"]->GetY() - 40);
	Get("mainwindow")["login-card"]["inputbox"]->Move(Get("mainwindow")["login-card"]["inputbox"]->GetX(),
													  Get("mainwindow")["login-card"]["inputbox"]->GetY() - 40);
	Get("mainwindow")["login-card"]["next-stage"]->Move(Get("mainwindow")["login-card"]["next-stage"]->GetX(),
														Get("mainwindow")["login-card"]["next-stage"]->GetY() - 40);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["title-text"]->GetX(),
										 Get("mainwindow")["login-card"]["title-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["description-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["description-text"]->GetX(),
										 Get("mainwindow")["login-card"]["description-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["inputbox"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["inputbox"]->GetX(),
										 Get("mainwindow")["login-card"]["inputbox"]->GetY() + 40));
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->SetTargetValue(
		1);

	Get("mainwindow")["login-card"]["next-stage"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["next-stage"]->GetX(),
										 Get("mainwindow")["login-card"]["next-stage"]->GetY() + 40));
	Get("mainwindow")["login-card"]["next-stage"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
	Get("mainwindow")["login-card"]["inputbox"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["next-stage"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["next-stage"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
}
void LoginUI::UIFadeInWithoutButton()
{
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->AnimationEnd.Block(this, &LoginUI::SetupVertifyInterface, true);

	Get("mainwindow")["login-card"]["title-text"]->Move(Get("mainwindow")["login-card"]["title-text"]->GetX(),
														Get("mainwindow")["login-card"]["title-text"]->GetY() - 80);
	Get("mainwindow")["login-card"]["description-text"]->Move(
		Get("mainwindow")["login-card"]["description-text"]->GetX(),
		Get("mainwindow")["login-card"]["description-text"]->GetY() - 80);
	Get("mainwindow")["login-card"]["inputbox"]->Move(Get("mainwindow")["login-card"]["inputbox"]->GetX(),
													  Get("mainwindow")["login-card"]["inputbox"]->GetY() - 80);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["title-text"]->GetX(),
										 Get("mainwindow")["login-card"]["title-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["description-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["description-text"]->GetX(),
										 Get("mainwindow")["login-card"]["description-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(1);

	Get("mainwindow")["login-card"]["inputbox"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["inputbox"]->GetX(),
										 Get("mainwindow")["login-card"]["inputbox"]->GetY() + 40));
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->SetTargetValue(
		1);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
	Get("mainwindow")["login-card"]["inputbox"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
}
void LoginUI::UIFadeOut()
{
	Get("mainwindow")["login-card"]["title-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["title-text"]->GetX(),
										 Get("mainwindow")["login-card"]["title-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(0);

	Get("mainwindow")["login-card"]["description-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["description-text"]->GetX(),
										 Get("mainwindow")["login-card"]["description-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(0);

	Get("mainwindow")["login-card"]["inputbox"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["inputbox"]->GetX(),
										 Get("mainwindow")["login-card"]["inputbox"]->GetY() + 40));
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->SetTargetValue(
		0);

	Get("mainwindow")["login-card"]["next-stage"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["next-stage"]->GetX(),
										 Get("mainwindow")["login-card"]["next-stage"]->GetY() + 40));
	Get("mainwindow")["login-card"]["next-stage"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(0);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
	Get("mainwindow")["login-card"]["inputbox"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["next-stage"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["next-stage"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
}
void LoginUI::UIFadeOutWithoutButton()
{
	Get("mainwindow")["login-card"]["title-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["title-text"]->GetX(),
										 Get("mainwindow")["login-card"]["title-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(0);

	Get("mainwindow")["login-card"]["description-text"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["description-text"]->GetX(),
										 Get("mainwindow")["login-card"]["description-text"]->GetY() + 40));
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->SetTargetValue(0);

	Get("mainwindow")["login-card"]["inputbox"]["position-animation"]
		.Get<Core::VPositionAnimation>()
		->SetTargetPosition(Core::VPoint(Get("mainwindow")["login-card"]["inputbox"]->GetX(),
										 Get("mainwindow")["login-card"]["inputbox"]->GetY() + 40));
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->SetTargetValue(
		0);

	Get("mainwindow")["login-card"]["title-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["title-text"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["description-text"]["transparent-animation"]
		.Get<Core::VOpacityAnimation>()
		->Start();
	Get("mainwindow")["login-card"]["inputbox"]["position-animation"].Get<Core::VPositionAnimation>()->Start();
	Get("mainwindow")["login-card"]["inputbox"]["transparent-animation"].Get<Core::VOpacityAnimation>()->Start();
}