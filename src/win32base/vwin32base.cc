#include "../../include/win32base/vwin32base.h"

namespace Win32Core
{
std::vector<VWin32Msg> VWin32MsgStack;

HWND VLastWindowHandle;

LRESULT CALLBACK VWin32ProcFnc(HWND Handle, UINT MessageType, WPARAM wParameter, LPARAM lParameter)
{
	VWin32Msg RawWin32Msg{};
	bool	  flag = false;

	switch (MessageType)
	{
	case WM_ERASEBKGND: {
		return 0;
	}
	case WM_LBUTTONDOWN: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_LBUTTONUP: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_MBUTTONDOWN: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_MBUTTONUP: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_RBUTTONUP: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_RBUTTONDOWN: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_MOUSEMOVE: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;
		RawWin32Msg.x		= GET_X_LPARAM(lParameter);
		RawWin32Msg.y		= GET_Y_LPARAM(lParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_CHAR: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		RawWin32Msg.message = WM_CHAR;

		RawWin32Msg.extended = lParameter >> 24;
		RawWin32Msg.prevdown = lParameter >> 30;

		RawWin32Msg.ch	   = wParameter;
		RawWin32Msg.vkcode = wParameter;

		break;
	}
	case WM_KEYDOWN: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		RawWin32Msg.message = WM_KEYDOWN;

		RawWin32Msg.extended = lParameter >> 24;
		RawWin32Msg.prevdown = lParameter >> 30;

		RawWin32Msg.vkcode = wParameter;

		break;
	}
	case WM_KEYUP: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		RawWin32Msg.message = WM_KEYUP;

		RawWin32Msg.extended = lParameter >> 24;
		RawWin32Msg.prevdown = lParameter >> 30;

		RawWin32Msg.vkcode = wParameter;

		break;
	}
	case WM_MOUSEWHEEL: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.ctrl	= false;
		RawWin32Msg.shift	= false;
		RawWin32Msg.lbutton = false;
		RawWin32Msg.mbutton = false;
		RawWin32Msg.rbutton = false;

		if (wParameter & MK_CONTROL)
		{
			RawWin32Msg.ctrl = true;
		}
		if (wParameter & MK_LBUTTON)
		{
			RawWin32Msg.lbutton = true;
		}
		if (wParameter & MK_MBUTTON)
		{
			RawWin32Msg.mbutton = true;
		}
		if (wParameter & MK_RBUTTON)
		{
			RawWin32Msg.rbutton = true;
		}
		if (wParameter & MK_SHIFT)
		{
			RawWin32Msg.shift = true;
		}

		RawWin32Msg.message = MessageType;

		POINT ScreenPoint = {GET_X_LPARAM(lParameter), GET_Y_LPARAM(lParameter)};

		ScreenToClient(Handle, &ScreenPoint);

		RawWin32Msg.x = ScreenPoint.x;
		RawWin32Msg.y = ScreenPoint.y;

		RawWin32Msg.wheel = GET_WHEEL_DELTA_WPARAM(wParameter);

		RawWin32Msg.wParam = wParameter;
		RawWin32Msg.lParam = lParameter;

		break;
	}
	case WM_CLOSE: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.message = MessageType;

		break;
	}
	default: {
		flag = true;

		RawWin32Msg.wHandle = Handle;
		RawWin32Msg.message = MessageType;
		RawWin32Msg.wParam	= wParameter;
		RawWin32Msg.lParam	= lParameter;

		break;
	}
	}

	if (flag)
	{
		VWin32MsgStack.push_back(RawWin32Msg);
	}

	return DefWindowProc(Handle, MessageType, wParameter, lParameter);
}
void VWin32MsgThread(const int &Width, const int &Height, LPCWSTR WindowTitle, LPCWSTR ClassName, HWND ParentWindow)
{
	auto InstanceHandle = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEX WindowClass;
	WindowClass.cbSize		  = sizeof(WindowClass);
	WindowClass.style		  = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc	  = (WNDPROC)VWin32ProcFnc;
	WindowClass.cbClsExtra	  = 0;
	WindowClass.cbWndExtra	  = 0;
	WindowClass.hInstance	  = InstanceHandle;
	WindowClass.hIcon		  = LoadIcon(nullptr, IDI_APPLICATION);
	WindowClass.hCursor		  = LoadCursor(nullptr, IDC_ARROW);
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName  = NULL;
	WindowClass.lpszClassName = ClassName;
	WindowClass.hIconSm		  = NULL;

	RegisterClassEx(&WindowClass);

	HWND WindowHandle = CreateWindow(ClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Width,
									 Height, ParentWindow, NULL, InstanceHandle, NULL);

	RECT WindowRect;
	RECT ClientRect;

	GetWindowRect(WindowHandle, &WindowRect);
	GetClientRect(WindowHandle, &ClientRect);

	int TitleBarHeight = WindowRect.bottom - WindowRect.top - ClientRect.bottom;

	MoveWindow(WindowHandle, 0, 0, Width, Height + TitleBarHeight, FALSE);

	MSG WinMsg{};

	VLastWindowHandle = WindowHandle;

	while (GetMessage(&WinMsg, 0, 0, 0))
	{
		TranslateMessage(&WinMsg);
		DispatchMessage(&WinMsg);
	}
}
bool VPeekMessage(VWin32Msg *MsgStructure)
{
	if (!VWin32MsgStack.empty())
	{
		auto Win32Msg = VWin32MsgStack.begin().operator*();
		*MsgStructure = Win32Msg;

		VWin32MsgStack.erase(VWin32MsgStack.begin());

		return true;
	}

	return false;
}
HWND VWin32CreateWindow(const int &Width, const int &Height, LPCWSTR WindowTitle, LPCWSTR ClassName, HWND ParentWindow)
{
	std::thread Thread(VWin32MsgThread, Width, Height, WindowTitle, ClassName, ParentWindow);
	Thread.detach();

	while (VLastWindowHandle == NULL)
	{
		Sleep(1);
	}

	auto Handle		  = VLastWindowHandle;
	VLastWindowHandle = NULL;

	return Handle;
}
} // namespace Win32Core