#pragma once

#include "../vbasiccore.h"

#include <functional>
#include <map>

#pragma comment(lib, "Imm32.lib")

VLIB_BEGIN_NAMESPACE

namespace Core {

struct VMainWindowConfig {
    int IMEX = -1;
    int IMEY = -2 ;

    LOGFONT  IMEFontStyle;

    bool      InFrameless    = false;
    bool      UseMaxMinSize  = false;
    bool      Sizable        = false;

    VGeomtery WindowMaxSize;
    VGeomtery WindowMiniSize;

    std::wstring GlobalFocusID;

    bool LockFocus = false;
    bool IMEInput  = false;

    bool   EnableRadius = false;
    VPoint BorderRadius = { 0, 0 };

    std::function<void()>         WinRepaintMessage;
    std::function<void()>         StartIMEInput;
    std::function<void()>         EndIMEInput;
    std::function<void()>         LosedUserFocus;
    std::function<void(int, int)> WindowOnSize;

    WNDPROC OriginWindowProcess;
};

extern std::map<HWND, VMainWindowConfig*> _VMainConfigs;

void ExResize(const int& Width, const int& Height);

LRESULT _VWidgetWNDPROC(HWND Handle, UINT Message, WPARAM wParameter, LPARAM lParameter);

};

VLIB_END_NAMESPACE