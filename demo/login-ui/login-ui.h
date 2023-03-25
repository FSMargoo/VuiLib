#pragma once

#include "../../include/vml/vmlwidget.h"
#include "login-ui.h"

class LoginUI : public VML::VMLMainWindow
{
public:
	LoginUI(Core::VApplication *App);

private:
	void ValidCheck(const wchar_t &Characeter, bool *AllowFlag);

private:
	void LoginInterface();
	void VertifyInterface();
	void SetupVertifyInterface();
	void WelcomeInterface();
	void SetupWelcomeInterface();

	void UIFadeIn();
	void UIFadeInWithoutButton();
	void UIFadeOut();
	void UIFadeOutWithoutButton();

private:
	VString UserName;
	VString VertifyCode;
};