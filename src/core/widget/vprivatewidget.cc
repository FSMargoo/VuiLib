#include "../../../include/core/widget/vprivatewidget.h"
#include "../../../include/core/render/vdirectxfactory.h"

#include <stdio.h>

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

VLIB_BEGIN_NAMESPACE

namespace Core {
    std::map<HWND, VWin32ThreadPipe*> _VMainConfigs;

    LRESULT _VWidgetWNDPROC(HWND Handle, UINT Message, WPARAM wParameter, LPARAM lParameter) {
        if (Message == WM_ENTERIDLE || Message == WM_GETICON || Message == 28 || Message == 134 || Message == 70 || Message == 144 || Message == 2 || _VMainConfigs.find(Handle) == _VMainConfigs.end() ||
                _VMainConfigs[Handle] == nullptr) {
            return DefWindowProc(Handle, Message, wParameter, lParameter);
        }
        
        if (Message != WM_GETICON) {
            printf("%#X\n", Message);
        }

        WNDPROC _VExWindowProcess = _VMainConfigs[Handle]->OriginWindowProcess;

        if (_VMainConfigs.find(Handle) != _VMainConfigs.end()) {
            switch (Message) {
            case WM_PAINT: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;
                WindowConfig->WinRepaintMessage();

                break;
            }
            case WM_NCHITTEST: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless) {
                    POINT MousePoint = { GET_X_LPARAM(lParameter), GET_Y_LPARAM(lParameter) };

                    RECT WindowRect;
                    GetWindowRect(Handle, &WindowRect);

                    if (MousePoint.x <= WindowRect.left + 10 && MousePoint.y <= WindowRect.top + 10) {
                        if (WindowConfig->Sizable) {
                            return HTTOPLEFT;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.x >= WindowRect.right - 10 && MousePoint.y <= WindowRect.top + 10) {
                        if (WindowConfig->Sizable) {
                            return HTTOPRIGHT;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.x <= WindowRect.left + 10 && MousePoint.y >= WindowRect.bottom - 10) {
                        if (WindowConfig->Sizable) {
                            return HTBOTTOMLEFT;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.x >= WindowRect.right - 10 && MousePoint.y >= WindowRect.bottom - 10) {
                        if (WindowConfig->Sizable) {
                            return HTBOTTOMRIGHT;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.x <= WindowRect.left + 10) {
                        if (WindowConfig->Sizable) {
                            return HTLEFT;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.x >= WindowRect.right - 10) {
                        if (WindowConfig->Sizable) {
                            return HTRIGHT;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.y <= WindowRect.top + 10) {
                        if (WindowConfig->Sizable) {
                            return HTTOP;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                    else if (MousePoint.y >= WindowRect.bottom - 10) {
                        if (WindowConfig->Sizable) {
                            return HTBOTTOM;
                        }
                        else {
                            return HTCLIENT;
                        }
                    }
                }

                break;
            }
            case WM_SETCURSOR: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless && WindowConfig->Sizable) {
                    switch (LOWORD(lParameter)) {
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

                break;
            }
            case WM_NCLBUTTONDOWN: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless && WindowConfig->Sizable) {
                    POINT MousePoint = { GET_X_LPARAM(lParameter), GET_Y_LPARAM(lParameter) };
                    switch (wParameter) {
                    case HTTOP:
                        SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP,
                            MAKELPARAM(MousePoint.x, MousePoint.y));

                        break;
                    case HTBOTTOM:
                        SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM,
                            MAKELPARAM(MousePoint.x, MousePoint.y));

                        break;
                    case HTLEFT:
                        SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT,
                            MAKELPARAM(MousePoint.x, MousePoint.y));

                        break;
                    case HTRIGHT:
                        SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT,
                            MAKELPARAM(MousePoint.x, MousePoint.y));

                        break;
                    case HTTOPLEFT:
                        SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT,
                            MAKELPARAM(MousePoint.x, MousePoint.y));

                        break;
                    case HTTOPRIGHT:
                        SendMessage(Handle, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT,
                            MAKELPARAM(MousePoint.x, MousePoint.y));

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
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                RECT* Rect = (RECT*)lParameter;

                WindowConfig->WindowOnSize(Rect->right - Rect->left, Rect->bottom - Rect->top);

                break;
            }
            case WM_SIZE: {
                if (wParameter != SIZE_MINIMIZED) {
                    VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                    if (wParameter != SIZE_RESTORED) {
                        WindowConfig->WindowOnSize(LOWORD(lParameter), HIWORD(lParameter));
                    }
                    else {
                        RECT Rect;
                        GetWindowRect(Handle, &Rect);

                        WindowConfig->WindowOnSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
                    }
                }

                break;
            }
            case WM_GETMINMAXINFO: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless) {
                    PMINMAXINFO MinMaxInfo = (PMINMAXINFO)lParameter;

                    MinMaxInfo->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
                }
                if (WindowConfig->UseMaxMinSize) {
                    PMINMAXINFO MinMaxInfo = (PMINMAXINFO)lParameter;

                    MinMaxInfo->ptMinTrackSize.x = WindowConfig->WindowMiniSize.Width;
                    MinMaxInfo->ptMinTrackSize.y = WindowConfig->WindowMiniSize.Height;

                    MinMaxInfo->ptMaxTrackSize.x = WindowConfig->WindowMaxSize.Width;
                    MinMaxInfo->ptMaxTrackSize.y = WindowConfig->WindowMaxSize.Height;
                }

                return DefWindowProc(Handle, Message, wParameter, lParameter);
            }
            case WM_IME_COMPOSITION: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                HIMC IMCHandle = ImmGetContext(Handle);
                if (IMCHandle) {
                    WindowConfig->IMEInput = true;

                    if (WindowConfig->StartIMEInput != nullptr) {
                        WindowConfig->StartIMEInput();
                    }

                    COMPOSITIONFORM Composition;
                    Composition.dwStyle = CFS_POINT;
                    Composition.ptCurrentPos.x = WindowConfig->IMEX;
                    Composition.ptCurrentPos.y = WindowConfig->IMEY;

                    ImmAssociateContext(Handle, IMCHandle);
                    ImmSetCompositionFont(IMCHandle, &WindowConfig->IMEFontStyle);
                    ImmSetCompositionWindow(IMCHandle, &Composition);
                }

                break;
            }
            case WM_IME_CHAR: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                WindowConfig->IMEInput = false;
                if (WindowConfig->EndIMEInput != nullptr) {
                    WindowConfig->EndIMEInput();
                }

                break;
            }
            case WM_KILLFOCUS: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                if (!WindowConfig->IMEInput) {
                    WindowConfig->LosedUserFocus();
                }

                break;
            }
            case WM_CLOSE: {
                VWin32ThreadPipe* WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->WindowQuitOnClicked()) {
                    return 0;
                }

                break;
            }
            }
        }

        return _VExWindowProcess(Handle, Message, wParameter, lParameter);
    }
}

VLIB_END_NAMESPACE