// License(MIT)
// Athuor: Margoo
// The widget (VMainWindow)
#pragma once

#include "../object/vapplication.h"
#include "../control/vswitchgroup.h"
#include "vprivatewidget.h"

#include <map>

VLIB_BEGIN_NAMESPACE

namespace Core {

struct VWin32StatusCache {
    int UserSetWidth  = 0;
    int UserSetHeight = 0;

    bool UserSetGeomtery = false;
    bool Repaint         = false;
};

class VMainWindow : public VUIObject {
 private:
     std::vector<VRepaintMessage*> RepaintMessages;
     VPainter*                     BufferPainter;

 private:
     VBasicTimer       FpsTimer;

     VMainWindowTheme* Theme;
     VMainWindowConfig WindowConfig;
     VWin32StatusCache Win32Cache;

     VHWNDRender*      Direct2DRender;

 public:
     bool IsWidget() override {
         return true;
     }

 private:
     void InitKernel();
     void InitWindow();
     void InitIME();

 private:
     void Win32ThreadResized(int Width, int Height);
     void Win32ThreadRepaint();
     void Win32LoseFocus();

 public:
     VMainWindow(const int& Width, const int& Height, VApplication* Parent, const bool& Sizble = true);
     ~VMainWindow();

 public:
     void CallWidgetSetIME(const int& X, const int& Y) override;
     void CallWidgetSetFocusID(const std::wstring& ObjectID) override;
     std::wstring CallWidgetGetFocusID() override;
     void CallWidgetLockFocusID() override;
     void CallWidgetUnlockFocusID() override;
     bool CallWidgetGetLockingStatus() override;
     void CallWidgetSendMessage(VMessage *Message) override;
     VCanvasPainter* CallWidgetGetCanvas() override;
     Core::VHWNDRender* CallWidgetGetDCRenderTarget() override;

public:
    HWND GetLocalWinId() override;

 public:
     void Update(VRect UpdateRect) override;

 public:
     void OnPaint(VCanvasPainter* Canvas, const VRect& Rect);

 public:
     void SetTitle(const std::wstring& WindowText);
     void SetBackgroundColor(const VColor& Color);
     void SetMaxSize(const VGeomtery& MaxSize);
     void SetMiniSize(const VGeomtery& MiniSize);
     void SetSizble(const bool& Sizble);

     void SetRadius(VPoint Radius = { 0, 0 });

     void SetFrameless(const bool& FramelessStatus);

     void Resize(const int& Width, const int& Height) override;

 public:
     void CheckFrame() override;
};

}

VLIB_END_NAMESPACE
