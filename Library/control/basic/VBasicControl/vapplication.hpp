/*
 * VUIObject.hpp
 *	@description : VLib's UI Object
 *	@birth		 : 2022/6.8
*/

#pragma once

#include "vuiobject.hpp"

VLIB_BEGIN_NAMESPACE

class VCoreApplication;

VCoreApplication* _VL_Core_Application = nullptr;

/*
 * VCoreApplication class:
 *	@description  : The Basic Application In VLib
*/
class VCoreApplication : public VUIObject {
public:
	/*
	 * PatchEvent Functional:
	 *	@description : Peek a Message From Win32
	*/
	VMessage* PatchEvent() {
		VMessage* ResultEvent = nullptr;

		ExMessage EasyxMessage;
		peekmessage(&EasyxMessage);

		/* Convert EasyX Message To VMessage */
		switch (EasyxMessage.message) {
		case WM_LBUTTONUP: {
			ResultEvent = new VMouseClickedMessage(EasyxMessage.x, EasyxMessage.y,
				VMouseClickedFlag::Up, VMouseKeyFlag::Left);

			return ResultEvent;
		}
		case WM_LBUTTONDOWN: {
			ResultEvent = new VMouseClickedMessage(EasyxMessage.x, EasyxMessage.y,
				VMouseClickedFlag::Down, VMouseKeyFlag::Left);

			return ResultEvent;
		}
		case WM_RBUTTONUP: {
			ResultEvent = new VMouseClickedMessage(EasyxMessage.x, EasyxMessage.y,
				VMouseClickedFlag::Up, VMouseKeyFlag::Right);

			return ResultEvent;
		}
		case WM_RBUTTONDOWN: {
			ResultEvent = new VMouseClickedMessage(EasyxMessage.x, EasyxMessage.y,
				VMouseClickedFlag::Down, VMouseKeyFlag::Right);

			return ResultEvent;
		}
		case WM_MBUTTONUP: {
			ResultEvent = new VMouseClickedMessage(EasyxMessage.x, EasyxMessage.y,
				VMouseClickedFlag::Up, VMouseKeyFlag::Middle);

			return ResultEvent;
		}
		case WM_MBUTTONDOWN: {
			ResultEvent = new VMouseClickedMessage(EasyxMessage.x, EasyxMessage.y,
				VMouseClickedFlag::Down, VMouseKeyFlag::Middle);

			return ResultEvent;
		}
		case WM_MOUSEMOVE: {
			ResultEvent = new VMouseMoveMessage(EasyxMessage.x, EasyxMessage.y);

			return ResultEvent;
		}
		case WM_MOUSEWHEEL: {
			ResultEvent = new VMouseWheelMessage(EasyxMessage.x, EasyxMessage.y, EasyxMessage.wheel);

			return ResultEvent;
		}
		case WM_KEYUP: {
			ResultEvent = new VKeyClickedMessage(EasyxMessage.vkcode, EasyxMessage.prevdown, EasyxMessage.extended, VkeyClickedFlag::Up);

			return ResultEvent;
		}
		case WM_KEYDOWN: {
			ResultEvent = new VKeyClickedMessage(EasyxMessage.vkcode, EasyxMessage.prevdown, EasyxMessage.extended, VkeyClickedFlag::Down);

			return ResultEvent;
		}
		case WM_CHAR: {
			ResultEvent = new VIMECharMessage(EasyxMessage.ch);

			return ResultEvent;
		}
		}

		return nullptr;
	}

	/*
	 * IsApplication override Functional:
	 *	@description  : Is this Object a Application
	*/
	bool IsApplication() override {
		return true;
	}

public:
	/*
	 * Build up Functional
	*/

	VCoreApplication() : VUIObject(nullptr) {

	}

	/*
	 * Instance Functional:
	 *	@description  : Get the Local Program's Instance Object
	*/
	static VCoreApplication* Instance() {
		return _VL_Core_Application;
	}
};

/*
 * VApplication class:
 *	@description  : The Application Class In VLib
*/
class VApplication : public VCoreApplication {
private:
	std::vector<VTheme*> ThemeList;

private:
	/*
	 * InitTheme class:
	 *	@description  : Init the Native Theme
	*/
	void InitTheme() {
		ThemeList.push_back(new VPushButtonTheme());
		ThemeList.push_back(new VTextLabelTheme());
		ThemeList.push_back(new VWidgetTheme());
		ThemeList.push_back(new VBlurLabelTheme());
		ThemeList.push_back(new VImageLabelTheme());
		ThemeList.push_back(new VIconButtonTheme());
		ThemeList.push_back(new VScrollBarTheme());
		ThemeList.push_back(new VScrollButtonTheme());
		ThemeList.push_back(new VInteractiveTextLabelTheme());
		ThemeList.push_back(new VLineEditorTheme());
		ThemeList.push_back(new VViewLabelTheme());
	}

protected:
	/*
	 * GetThemeListFromParent override Functional:
	 *	@description  : Get the Theme List
	*/
	std::vector<VTheme*> GetThemeListFromParent() override {
		return ThemeList;
	}

private:
	/*
	 * ProcessEvent Functional:
	 *	@description  : Dealy the Pathced Message
	*/
	void ProcessEvent(VMessage* PatchedMessage) {
		PatchedMessage = PatchEvent();

		while (PatchedMessage != nullptr) {
			SysDealyMessage(PatchedMessage);

			delete PatchedMessage;

			PatchedMessage = PatchEvent();
		}
	}

public:
	/*
	 * Build up Functional
	*/

	VApplication() {
		InitTheme();

		if (VUnlikely(VCoreApplication::Instance() == nullptr)) {
			_VL_Core_Application = this;
		}
		else {
			VAssertBox(L"VApplication Should Be Only Init At Once !");
		}
	}

	/*
	 * Exec Functional:
	 *	@descipriton  : The Event Loop Of Application
	*/
	int Exec() {
		VMessage* PatchedMessage = nullptr;

		while (true) {
			/* Dealy the Event */
			ProcessEvent(PatchedMessage);

			CheckAllFrame();

			Sleep(1);
		}

		/* Exit Not Normaly */
		return -1;
	}
};

VLIB_END_NAMESPACE