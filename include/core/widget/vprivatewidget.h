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

    PLOGFONT  IMEFontStyle;

    bool      UseMaxMinSize = false;

    VGeomtery WindowMaxSize;
    VGeomtery WindowMiniSize;

    std::wstring GlobalFocusID;

    bool LockFocus = false;
    bool IMEInput  = false;

    std::function<void()>         StartIMEInput;
    std::function<void()>         EndIMEInput;
    std::function<void()>         LosedUserFocus;
    std::function<void(int, int)> WindowOnSize;
};

extern WNDPROC _VExWindowProcess;
extern std::map<HWND, VMainWindowConfig*> _VMainConfigs;

void ExResize(const int& Width, const int& Height);

LRESULT _VWidgetWNDPROC(HWND Handle, UINT Message, WPARAM wParameter, LPARAM lParameter);

};

VLIB_END_NAMESPACE