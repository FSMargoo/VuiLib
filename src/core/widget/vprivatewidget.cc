#include "../../../include/core/widget/vprivatewidget.h"

#include "../../../include/core/render/vdirectxfactory.h"
#include <uxtheme.h>
#include <vssym32.h>

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "uxtheme.lib")

VLIB_BEGIN_NAMESPACE

namespace Core
{
std::map<HWND, VWin32ThreadPipe *> _VMainConfigs;
bool							   MainThreadEnd = false;

LRESULT _VWidgetWNDPROC(HWND Handle, UINT Message, WPARAM wParameter, LPARAM lParameter)
{
	if (MainThreadEnd)
	{
		return DefWindowProc(Handle, Message, wParameter, lParameter);
	}

	if (!(Message == WM_PAINT || Message == WM_NCHITTEST || Message == WM_SETCURSOR || Message == WM_NCLBUTTONDOWN ||
		  Message == WM_SIZING || Message == WM_SIZE || Message == WM_GETMINMAXINFO || Message == WM_IME_COMPOSITION ||
		  Message == WM_CHAR || Message == WM_KILLFOCUS || Message == WM_NCMOUSELEAVE || Message == WM_CLOSE ||
		  Message == WM_LBUTTONDOWN || Message == WM_LBUTTONUP || Message == WM_MBUTTONDOWN ||
		  Message == WM_MBUTTONUP || Message == WM_RBUTTONUP || Message == WM_RBUTTONDOWN || Message == WM_MOUSEMOVE ||
		  Message == WM_IME_CHAR || Message == WM_KEYDOWN || Message == WM_KEYUP || Message == WM_MOUSEWHEEL ||
		  Message == WM_CLOSE || Message == WM_NCCALCSIZE || Message == WM_DROPFILES))
	{
		return DefWindowProc(Handle, Message, wParameter, lParameter);
	}

	if (_VMainConfigs.find(Handle) == _VMainConfigs.end() || _VMainConfigs[Handle] == nullptr)
	{
		return DefWindowProc(Handle, Message, wParameter, lParameter);
	}

	WNDPROC _VExWindowProcess = _VMainConfigs[Handle]->OriginWindowProcess;

	if (_VMainConfigs.find(Handle) != _VMainConfigs.end())
	{
		switch (Message)
		{
		case WM_DROPFILES: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			wchar_t FileName[MAX_PATH];

			HDROP DropInstance = (HDROP)wParameter;
			UINT  FileCount	   = DragQueryFile(DropInstance, -1, NULL, 0);

			std::vector<VString> FileSets;

			for (; FileCount > 0; --FileCount)
			{
				if (DragQueryFile(DropInstance, FileCount - 1, FileName, sizeof(FileName)))
				{
					FileSets.push_back(FileName);
				}
			}

			DragFinish(DropInstance);

			WindowConfig->WindowOnFileDrag(FileSets);

			break;
		}
		case WM_PAINT: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;
			WindowConfig->WinRepaintMessage();

			break;
		}
		case WM_NCHITTEST: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (WindowConfig->InFrameless)
			{
				POINT MousePoint = {GET_X_LPARAM(lParameter), GET_Y_LPARAM(lParameter)};

				RECT WindowRect;
				GetWindowRect(Handle, &WindowRect);

				if (MousePoint.x <= WindowRect.left + 10 && MousePoint.y <= WindowRect.top + 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTTOPLEFT;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.x >= WindowRect.right - 10 && MousePoint.y <= WindowRect.top + 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTTOPRIGHT;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.x <= WindowRect.left + 10 && MousePoint.y >= WindowRect.bottom - 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTBOTTOMLEFT;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.x >= WindowRect.right - 10 && MousePoint.y >= WindowRect.bottom - 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTBOTTOMRIGHT;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.x <= WindowRect.left + 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTLEFT;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.x >= WindowRect.right - 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTRIGHT;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.y <= WindowRect.top + 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTTOP;
					}
					else
					{
						return HTCLIENT;
					}
				}
				else if (MousePoint.y >= WindowRect.bottom - 10)
				{
					if (WindowConfig->Sizable)
					{
						return HTBOTTOM;
					}
					else
					{
						return HTCLIENT;
					}
				}
			}

			if (WindowConfig->BorderLess && WindowConfig->Sizable)
			{
				POINT MousePoint = {GET_X_LPARAM(lParameter), GET_Y_LPARAM(lParameter)};
				RECT  WindowRect;
				ScreenToClient(Handle, &MousePoint);
				GetClientRect(Handle, &WindowRect);

				if (MousePoint.y > 0 && MousePoint.y < 5)
				{
					return HTTOP;
				}

				WindowRect.bottom -= WindowRect.top;
				WindowRect.bottom -= GetSystemMetrics(SM_CYCAPTION);
				WindowRect.bottom += 5;

				if (MousePoint.y <= WindowRect.bottom && MousePoint.y >= WindowRect.bottom - 5)
				{
					return HTBOTTOM;
				}
			}

			break;
		}
		case WM_SETCURSOR: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (WindowConfig->InFrameless && WindowConfig->Sizable)
			{
				switch (LOWORD(lParameter))
				{
				case HTTOP:
				case HTBOTTOM: {
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					return TRUE;
				}

				case HTLEFT:
				case HTRIGHT: {
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
					return TRUE;
				}

				case HTTOPLEFT:
				case HTBOTTOMRIGHT: {
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
					return TRUE;
				}

				case HTTOPRIGHT:
				case HTBOTTOMLEFT: {
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
					return TRUE;
				}
				}
			}
			if (WindowConfig->BorderLess)
			{
				switch (LOWORD(lParameter))
				{
				case HTTOP:
				case HTBOTTOM: {
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					return TRUE;
				}
				}
			}

			break;
		}
		case WM_NCLBUTTONDOWN: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (WindowConfig->InFrameless && WindowConfig->Sizable)
			{
				POINT MousePoint = {GET_X_LPARAM(lParameter), GET_Y_LPARAM(lParameter)};
				switch (wParameter)
				{
				case HTTOP:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTBOTTOM:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTLEFT:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTRIGHT:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTTOPLEFT:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTTOPRIGHT:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTBOTTOMLEFT:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT,
								MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				case HTBOTTOMRIGHT:
					SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT,
								MAKELPARAM(MousePoint.x, MousePoint.y));

					break;
				}
			}

			break;
		}
		case WM_SIZING: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			RECT *Rect = (RECT *)lParameter;

			WindowConfig->WindowOnSize(Rect->right - Rect->left, Rect->bottom - Rect->top);

			break;
		}
		case WM_NCCALCSIZE: {
			if (!wParameter)
			{
				break;
			}
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (WindowConfig->BorderLess)
			{
				int OffsetX = GetSystemMetrics(SM_CXFRAME);
				int OffsetY = GetSystemMetrics(SM_CYFRAME);
				int Margin	= GetSystemMetrics(SM_CXPADDEDBORDER);

				NCCALCSIZE_PARAMS *Parameter  = (NCCALCSIZE_PARAMS *)lParameter;
				RECT			  *ClientRect = Parameter->rgrc;

				ClientRect->right -= OffsetY + Margin;
				ClientRect->left += OffsetX + Margin;
				ClientRect->bottom -= OffsetY + Margin - GetSystemMetrics(SM_CYCAPTION);

				return 0;
			}
			else
			{
				break;
			}
		}
		case WM_SIZE: {
			if (wParameter != SIZE_MINIMIZED)
			{
				VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

				if (wParameter != SIZE_RESTORED)
				{
					RECT WindowRect;
					RECT ClientRect;

					GetWindowRect(Handle, &WindowRect);
					GetClientRect(Handle, &ClientRect);

					int TitleBarHeight = WindowRect.bottom - WindowRect.top - ClientRect.bottom;

					WindowConfig->WindowOnSize(LOWORD(lParameter), HIWORD(lParameter) + TitleBarHeight);
				}
				else
				{
					RECT Rect;
					GetWindowRect(Handle, &Rect);

					WindowConfig->WindowOnSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
				}
			}

			break;
		}
		case WM_GETMINMAXINFO: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (WindowConfig->InFrameless)
			{
				PMINMAXINFO MinMaxInfo = (PMINMAXINFO)lParameter;

				MinMaxInfo->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION) +
										  GetSystemMetrics(SM_CYDLGFRAME);
			}
			if (WindowConfig->UseMaxMinSize)
			{
				PMINMAXINFO MinMaxInfo = (PMINMAXINFO)lParameter;

				MinMaxInfo->ptMinTrackSize.x = WindowConfig->WindowMiniSize.Width;
				MinMaxInfo->ptMinTrackSize.y = WindowConfig->WindowMiniSize.Height;

				MinMaxInfo->ptMaxTrackSize.x = WindowConfig->WindowMaxSize.Width;
				MinMaxInfo->ptMaxTrackSize.y = WindowConfig->WindowMaxSize.Height;
			}

			return DefWindowProc(Handle, Message, wParameter, lParameter);
		}
		case WM_IME_COMPOSITION: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			HIMC IMCHandle = ImmGetContext(Handle);
			if (IMCHandle)
			{
				WindowConfig->IMEInput = true;

				if (WindowConfig->StartIMEInput != nullptr)
				{
					WindowConfig->StartIMEInput();
				}

				COMPOSITIONFORM Composition;
				Composition.dwStyle		   = CFS_POINT;
				Composition.ptCurrentPos.x = WindowConfig->IMEX;
				Composition.ptCurrentPos.y = WindowConfig->IMEY;

				ImmAssociateContextEx(Handle, IMCHandle, IACE_CHILDREN);
				ImmSetCompositionFont(IMCHandle, &WindowConfig->IMEFontStyle);
				ImmSetCompositionWindow(IMCHandle, &Composition);
			}

			break;
		}
		case WM_IME_CHAR: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			WindowConfig->IMEInput = false;
			if (WindowConfig->EndIMEInput != nullptr)
			{
				WindowConfig->EndIMEInput();
			}

			break;
		}
		case WM_KILLFOCUS:
		case WM_NCMOUSELEAVE: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (!WindowConfig->IMEInput)
			{
				WindowConfig->LosedUserFocus();
			}

			break;
		}
		case WM_CLOSE: {
			VWin32ThreadPipe *WindowConfig = _VMainConfigs.find(Handle)->second;

			if (WindowConfig->WindowQuitOnClicked())
			{
				MainThreadEnd = true;

				return 0;
			}

			break;
		}
		}
	}

	return _VExWindowProcess(Handle, Message, wParameter, lParameter);
}
} // namespace Core

VLIB_END_NAMESPACE