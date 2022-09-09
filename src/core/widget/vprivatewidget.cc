#include "../../../include/core/widget/vprivatewidget.h"
#include "../../../include/core/render/vdirectxfactory.h"

#include <graphics.h>
#include <stdio.h>

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

VLIB_BEGIN_NAMESPACE

namespace Core {

WNDPROC _VExWindowProcess;
std::map<HWND, VMainWindowConfig*> _VMainConfigs;

void ExResize(const int& Width, const int& Height) {
    Resize(NULL, Width, Height);
}

LRESULT _VWidgetWNDPROC(HWND Handle, UINT Message, WPARAM wParameter, LPARAM lParameter) {
    if (_VMainConfigs.find(Handle) != _VMainConfigs.end()) {
        switch (Message) {
            case WM_PAINT: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;
                WindowConfig->WinRepaintMessage();

                break;
            }
            case WM_NCHITTEST: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless) {
                    POINT MousePoint = {(int) (short) LOWORD(lParameter), (int) (short) HIWORD(lParameter)};

                    RECT WindowRect;
                    GetWindowRect(Handle, &WindowRect);

                    if (MousePoint.x <= WindowRect.left + 10 && MousePoint.y <= WindowRect.top + 10) {
                        return HTTOPLEFT;
                    } else if (MousePoint.x >= WindowRect.right - 10 && MousePoint.y <= WindowRect.top + 10) {
                        return HTTOPRIGHT;
                    } else if (MousePoint.x <= WindowRect.left + 10 && MousePoint.y >= WindowRect.bottom - 10) {
                        return HTBOTTOMLEFT;
                    } else if (MousePoint.x >= WindowRect.right - 10 && MousePoint.y >= WindowRect.bottom - 10) {
                        return HTBOTTOMRIGHT;
                    } else if (MousePoint.x <= WindowRect.left + 10) {
                        return HTLEFT;
                    } else if (MousePoint.x >= WindowRect.right - 10) {
                        return HTRIGHT;
                    } else if (MousePoint.y <= WindowRect.top + 10) {
                        return HTTOP;
                    } else if (MousePoint.y >= WindowRect.bottom - 10) {
                        return HTBOTTOM;
                    }
                }

                break;
            }
            case WM_SETCURSOR: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless) {
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
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->InFrameless) {
                    POINT MousePoint = {(int) (short) LOWORD(lParameter), (int) (short) HIWORD(lParameter)};
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
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                RECT *Rect = (RECT *) lParameter;

                WindowConfig->WindowOnSize(Rect->right - Rect->left, Rect->bottom - Rect->top);

                break;
            }
            case WM_SIZE: {
                if (wParameter != SIZE_MINIMIZED) {
                    VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                    if (wParameter != SIZE_RESTORED) {
                        RECT Rect;
                        GetWindowRect(Handle, &Rect);

                        WindowConfig->WindowOnSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
                    } else {
                        WindowConfig->WindowOnSize(LOWORD(lParameter), HIWORD(lParameter));
                    }
                }

                break;
            }
            case WM_GETMINMAXINFO: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;
                if (WindowConfig->UseMaxMinSize) {
                    PMINMAXINFO MinMaxInfo = (PMINMAXINFO) lParameter;

                    MinMaxInfo->ptMinTrackSize.x = WindowConfig->WindowMiniSize.Width;
                    MinMaxInfo->ptMinTrackSize.y = WindowConfig->WindowMiniSize.Height;

                    MinMaxInfo->ptMaxTrackSize.x = WindowConfig->WindowMaxSize.Width;
                    MinMaxInfo->ptMaxTrackSize.y = WindowConfig->WindowMaxSize.Height;
                }

                return DefWindowProc(Handle, Message, wParameter, lParameter);
            }
            case WM_IME_COMPOSITION: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

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
                    ImmSetCompositionFont(IMCHandle, WindowConfig->IMEFontStyle);
                    ImmSetCompositionWindow(IMCHandle, &Composition);
                }

                break;
            }
            case WM_IME_CHAR: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                WindowConfig->IMEInput = false;
                if (WindowConfig->EndIMEInput != nullptr) {
                    WindowConfig->EndIMEInput();
                }

                break;
            }
            case WM_KILLFOCUS: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                if (WindowConfig->IMEX == -1 && WindowConfig->IMEY == -1 && !WindowConfig->IMEInput) {
                    WindowConfig->LosedUserFocus();
                }

                break;
            }
        }
    }

    return _VExWindowProcess(Handle, Message, wParameter, lParameter);
}
}

VLIB_END_NAMESPACE