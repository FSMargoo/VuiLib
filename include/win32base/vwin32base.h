/*
 * File name	: vwin32base.h
 * Author		: Margoo
 * Date			: 11/22/2022
 * Description
 * : The basic win32 operation's wrapper in vuilib
 */
#pragma once

#include <thread>
#include <vector>

#include <windows.h>

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

namespace Win32Core
{
struct VWin32Msg
{
	USHORT message;

	bool  ctrl	  : 1;
	bool  shift	  : 1;
	bool  lbutton : 1;
	bool  mbutton : 1;
	bool  rbutton : 1;
	short x;
	short y;
	short wheel;

	BYTE vkcode;
	BYTE scancode;
	bool extended : 1;
	bool prevdown : 1;

	TCHAR ch;

	WPARAM wParam;
	LPARAM lParam;

	HWND wHandle;
};

extern std::vector<VWin32Msg> VWin32MsgStack;

void VWin32MsgThread(const int &Width, const int &Height, LPCWSTR WindowTitle, LPCWSTR ClassName, HWND ParentWindow);

LRESULT VWin32ProcFnc(HWND Handle, UINT MessageType, WPARAM wParameter, LPARAM lParameter);
HWND VWin32CreateWindow(const int &Width, const int &Height, LPCWSTR WindowTitle, LPCWSTR ClassName, HWND ParentWindow);

bool VPeekMessage(VWin32Msg *MsgStructure);

} // namespace Win32Core