#include "../../../include/core/widget/vprivatewidget.h"

#include <graphics.h>

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
            case WM_SIZING: {
                VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                RECT *Rect = (RECT *) lParameter;

                WindowConfig->WindowOnSize(Rect->right - Rect->left - 18, Rect->bottom - Rect->top - 39);

                return 0;
            }
            case WM_SIZE: {
                if (wParameter != SIZE_MINIMIZED) {
                    VMainWindowConfig *WindowConfig = _VMainConfigs.find(Handle)->second;

                    if (wParameter != SIZE_RESTORED) {
                        RECT Rect;
                        GetWindowRect(Handle, &Rect);
                        WindowConfig->WindowOnSize(Rect.right - Rect.left - 18, Rect.bottom - Rect.top - 39);
                    } else {
                        WindowConfig->WindowOnSize(LOWORD(lParameter), HIWORD(lParameter));
                    }
                }

                return 0;
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