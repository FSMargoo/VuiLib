#include "../../../include/core/object/vapplication.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VApplication *VLib_Application = nullptr;

VApplication::~VApplication()
{
	VUIObject::~VUIObject();

	for (auto &ThemeInstance : ThemeList)
	{
		delete ThemeInstance;
	}

	ThemeList.clear();
}
VApplication::VApplication() : VUIObject(nullptr)
{
	InitTheme();
}
VMessage *VApplication::PatchEvent()
{
	VMessage *ResultEvent = nullptr;

	Win32Core::VWin32Msg Win32Message{};
	auto				 Result = Win32Core::VPeekMessage(&Win32Message);

	if (!Result)
	{
		return nullptr;
	}

	switch (Win32Message.message)
	{
	case WM_LBUTTONUP: {
		ResultEvent = new VMouseClickedMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y,
											   VMouseClickedFlag::Up, VMouseKeyFlag::Left);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_LBUTTONDOWN: {
		ResultEvent = new VMouseClickedMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y,
											   VMouseClickedFlag::Down, VMouseKeyFlag::Left);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_RBUTTONUP: {
		ResultEvent = new VMouseClickedMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y,
											   VMouseClickedFlag::Up, VMouseKeyFlag::Right);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_RBUTTONDOWN: {
		ResultEvent = new VMouseClickedMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y,
											   VMouseClickedFlag::Down, VMouseKeyFlag::Right);

		ResultEvent->MessageTriggerWidget = Win32Message.wHandle;

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_MBUTTONUP: {
		ResultEvent = new VMouseClickedMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y,
											   VMouseClickedFlag::Up, VMouseKeyFlag::Middle);

		ResultEvent->MessageTriggerWidget = Win32Message.wHandle;

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_MBUTTONDOWN: {
		ResultEvent = new VMouseClickedMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y,
											   VMouseClickedFlag::Down, VMouseKeyFlag::Middle);

		ResultEvent->MessageTriggerWidget = Win32Message.wHandle;

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_MOUSEMOVE: {
		ResultEvent = new VMouseMoveMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y);

		ResultEvent->MessageTriggerWidget = Win32Message.wHandle;

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_MOUSEWHEEL: {
		ResultEvent = new VMouseWheelMessage(Win32Message.wHandle, Win32Message.x, Win32Message.y, Win32Message.wheel);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_KEYUP: {
		ResultEvent = new VKeyClickedMessage(Win32Message.wHandle, Win32Message.vkcode, Win32Message.prevdown,
											 Win32Message.extended, VkeyClickedFlag::Up);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_KEYDOWN: {
		ResultEvent = new VKeyClickedMessage(Win32Message.wHandle, Win32Message.vkcode, Win32Message.prevdown,
											 Win32Message.extended, VkeyClickedFlag::Down);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}
	case WM_CHAR: {
		ResultEvent = new VIMECharMessage(Win32Message.wHandle, Win32Message.ch);

		ResultEvent->Win32ID = Win32Message.message;
		return ResultEvent;
	}

	case WM_CLOSE: {
		ResultEvent = new VQuitWindowMessage(Win32Message.wHandle);

		ResultEvent->Win32ID = Win32Message.message;

		return ResultEvent;
	}

	default: {
		ResultEvent						  = new VMessage;
		ResultEvent->MessageTriggerWidget = Win32Message.wHandle;

		ResultEvent->Win32ID = Win32Message.message;

		ResultEvent->wParameter = Win32Message.wParam;
		ResultEvent->lParameter = Win32Message.lParam;

		return ResultEvent;
	}
	}

	return nullptr;
}
void VApplication::InitTheme()
{
	ThemeList.push_back(new VPushButtonTheme);
	ThemeList.push_back(new VMainWindowTheme);
	ThemeList.push_back(new VImageLabelTheme);
	ThemeList.push_back(new VTextLabelTheme);
	ThemeList.push_back(new VRadioButtonTheme);
	ThemeList.push_back(new VCircleScrollBarTheme);
	ThemeList.push_back(new VSliderTheme);
	ThemeList.push_back(new VBlurLabelTheme);
	ThemeList.push_back(new VIconButtonTheme);
	ThemeList.push_back(new VTextEditorTheme);
	ThemeList.push_back(new VViewScrollerButtonTheme);
	ThemeList.push_back(new VViewScrollerTheme);
	ThemeList.push_back(new VViewLabelTheme);
}
void VApplication::ProcessEvent(Core::VMessage *PatchedMessage)
{
	PatchedMessage = PatchEvent();

	while (PatchedMessage != nullptr)
	{
		SysProcessMessage(PatchedMessage);

		delete PatchedMessage;

		PatchedMessage = PatchEvent();
	}
}
std::vector<VBasicUITheme *> VApplication::GetApplicationTheme()
{
	return ThemeList;
}
int VApplication::Exec()
{
	VMessage *PatchedMessage = nullptr;

	while (true)
	{
		ProcessEvent(PatchedMessage);
		CheckAllFrame(true);

		VTimeHelper::Sleep(1);
	}

	return -1;
}
bool VApplication::IsApplication()
{
	return true;
}
void VApplication::SetTheme(VBasicUITheme *Theme)
{
	for (int Count = 0; Count < ThemeList.size(); ++Count)
	{
		if (ThemeList[Count]->GetThemeType() == Theme->GetThemeType())
		{
			delete ThemeList[Count];

			ThemeList.erase(ThemeList.begin() + Count);
			ThemeList.insert(ThemeList.begin() + Count, Theme);

			for (auto &Objects : ObjectKernel.ChildObjectContainer)
			{
				Objects->Update();
			}

			break;
		}
	}
}
void VApplication::SetTheme(VThemeAbstract *Theme)
{
	SetTheme(Theme->GetMainWindowTheme());
	SetTheme(Theme->GetTextLabelTheme());
	SetTheme(Theme->GetPushButtonTheme());
	SetTheme(Theme->GetTextEditorTheme());
	SetTheme(Theme->GetSilderTheme());
	SetTheme(Theme->GetScrollBarTheme());
	SetTheme(Theme->GetRadioTheme());
	SetTheme(Theme->GetViewScrollerButtonTheme());
	SetTheme(Theme->GetViewLabelTheme());
}
} // namespace Core
VLIB_END_NAMESPACE