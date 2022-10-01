#include "../../../include/core/widget/vwidget.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

void VMainWindow::Win32ThreadResized(int Width, int Height) {
     Win32Cache.UserSetGeomtery = true;
     Win32Cache.UserSetWidth    = Width;
     Win32Cache.UserSetHeight   = Height;
}
void VMainWindow::Win32ThreadRepaint() {
    Win32Cache.Repaint = true;
}
void VMainWindow::Win32LoseFocus() {
    CallWidgetUnlockFocusID();

    VKillFocusMessage* KillFocusMessage = new VKillFocusMessage;
    SendMessageToChild(KillFocusMessage, false);

    delete KillFocusMessage;
}

void VMainWindow::InitKernel() {
    FpsTimer.Start(16);
    _VMainConfigs.insert(std::make_pair<HWND, VMainWindowConfig*>(GetHWnd(), &WindowConfig));
}
void VMainWindow::InitWindow() {
    initgraph(GetWidth(), GetHeight());

    if (Theme->WindowSizble) {
        SetWindowLong(GetHWnd(), GWL_STYLE, GetWindowLong(GetHWnd(), GWL_STYLE) | (WS_MAXIMIZEBOX));
        SetWindowLong(GetHWnd(), GWL_STYLE, (GetWindowLong(GetHWnd(), GWL_STYLE) | WS_THICKFRAME));
    }

    _VExWindowProcess = reinterpret_cast<WNDPROC>(GetWindowLongPtr(GetHWnd(), GWLP_WNDPROC));
    SetWindowLongPtr(GetHWnd(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_VWidgetWNDPROC));

    BeginBatchDraw();

    Direct2DRender = new VDCRender(VDirectXD2DFactory.GetInstance(), GetDC(GetHWnd()),
                                   { 0, 0, GetWidth(), GetHeight() }, true);
    BufferPainter  = new VPainter(Direct2DRender->GetDirectXRenderTarget());

    WindowConfig.WindowOnSize = [this](auto PH1, auto PH2)
            { Win32ThreadResized(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); };
    WindowConfig.LosedUserFocus = [this]() -> void
            { Win32LoseFocus(); };
    WindowConfig.WinRepaintMessage = [this]() { Win32ThreadRepaint(); };
}
void VMainWindow::InitIME() {
    WindowConfig.IMEX = -1;
    WindowConfig.IMEY = -1;
}

VMainWindow::VMainWindow(const int& Width, const int& Height, VApplication* Parent, const bool& Sizble)
        : VUIObject(Parent) {
    Theme = static_cast<VMainWindowTheme*>(GetTargetTheme(VUIThemeType::VMainWindow));
    VLIB_CHECK_REPORT(Theme == nullptr, L"Object [VMainWindow] losed!");

    Theme->WindowSizble = Sizble;

    if (Parent == nullptr) {
        VLIB_REPORT_ERROR(L"VMainWindow should be init after VApplication!");

        exit(-1);
    }

    InitWindow();
    InitKernel();
    InitIME();

    Resize(Width, Height);

    Update( { 0, 0, GetWidth(), GetHeight() } );
}

void VMainWindow::CallWidgetSetIME(const int& X, const int& Y) {
    WindowConfig.IMEX = X;
    WindowConfig.IMEY = Y;
}
void VMainWindow::CallWidgetSetFocusID(const std::wstring& ObjectID) {
    WindowConfig.GlobalFocusID = ObjectID;
}
std::wstring VMainWindow::CallWidgetGetFocusID() {
    return WindowConfig.GlobalFocusID;
}
void VMainWindow::CallWidgetLockFocusID() {
    WindowConfig.LockFocus = true;
}
void VMainWindow::CallWidgetUnlockFocusID() {
    WindowConfig.LockFocus = false;
}
bool VMainWindow::CallWidgetGetLockingStatus() {
    return WindowConfig.LockFocus;
}
void VMainWindow::CallWidgetSendMessage(VMessage *Message) {
    SendMessageToChild(Message);
}
VCanvasPainter* VMainWindow::CallWidgetGetCanvas() {
    return Canvas;
}
Core::VDCRender* VMainWindow::CallWidgetGetDCRenderTarget() {
    return Direct2DRender;
}

void VMainWindow::OnPaint(VCanvasPainter* Canvas, const VRect& Rect) {
    VSolidBrush BackgroundColor(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->BackgroundColor);

    Canvas->SolidRectangle(Rect, &BackgroundColor);
}

void VMainWindow::Update(VRect UpdateRect) {
    VGetRepaintAeraMessage RepaintMessage(UpdateRect);

    std::map<int, int>     AlreadySendedObject;

    int                    ObjectCount = 0;

    for (auto ChildObject = ObjectKernel.ChildObjectContainer.begin();
         ChildObject != ObjectKernel.ChildObjectContainer.end(); ++ObjectCount) {
        if (AlreadySendedObject.find(ObjectCount) == AlreadySendedObject.end()) {
            if (ChildObject.operator*()->SysProcessMessage(&RepaintMessage)) {
                AlreadySendedObject.insert(std::pair<int, bool>(ObjectCount, 1));

                ChildObject = ObjectKernel.ChildObjectContainer.begin();

                ObjectCount = -1;

                continue;
            }
        }

        ++ChildObject;
    }

    for (auto& AlreadyExsitsMessage : RepaintMessages) {
        if (AlreadyExsitsMessage->DirtyRectangle == *(RepaintMessage.RepaintAera)) {
            return;
        }

        if (AlreadyExsitsMessage->DirtyRectangle.Overlap(*(RepaintMessage.RepaintAera))) {
            AlreadyExsitsMessage->DirtyRectangle.FusionRect(*(RepaintMessage.RepaintAera));
            return;
        }
    }

    VRect RepaintRect = *(RepaintMessage.RepaintAera);

    RepaintMessages.push_back(new VRepaintMessage(RepaintRect));
}

void VMainWindow::CheckFrame() {
    if (FpsTimer.End()) {
        FpsTimer.Start(14);

        if (Win32Cache.Repaint) {
            if (!Win32Cache.UserSetGeomtery) {
                Win32Cache.Repaint = false;

                Update({0, 0, GetWidth(), GetHeight() } );
            } else {
                Win32Cache.Repaint = false;
            }
        }

        if (Win32Cache.UserSetGeomtery) {
            Win32Cache.UserSetGeomtery = false;

            VUIObject::Resize(Win32Cache.UserSetWidth, Win32Cache.UserSetHeight);

            Resized.Emit(Win32Cache.UserSetWidth, Win32Cache.UserSetHeight);

            delete Direct2DRender;
            delete BufferPainter;

            Direct2DRender = new VDCRender(VDirectXD2DFactory.GetInstance(), GetDC(GetHWnd()),
                                           { 0, 0, GetWidth(), GetHeight() }, true);
            BufferPainter  = new VPainter(Direct2DRender->GetDirectXRenderTarget());

            Update( { 0, 0, GetWidth(), GetHeight() } );
        }

        if (!RepaintMessages.empty()) {
            Canvas = new VCanvasPainter(GetWidth(), GetHeight(),
                                                               CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());

            Canvas->BeginDraw();
            for (auto& Message : RepaintMessages) {
                OnPaint(Canvas, Message->DirtyRectangle);
                SendMessageToChild(Message, false);

                delete Message;
            }
            Canvas->EndDraw();

            RepaintMessages.clear();

            if (WindowConfig.EnableRadius) {
                IMAGE BorderImage(GetWidth(), GetHeight());
                auto Property = D2D1::RenderTargetProperties(
                        D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
                        D2D1::PixelFormat(
                                DXGI_FORMAT_B8G8R8A8_UNORM,
                                D2D1_ALPHA_MODE_PREMULTIPLIED
                        ), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
                );
                ID2D1DCRenderTarget* DCRenderTarget;
                HRESULT Result = VDirectXD2DFactory.GetInstance()->CreateDCRenderTarget(
                        &Property,
                        &DCRenderTarget
                );
                RECT WindowRect = { 0, 0, GetWidth(), GetHeight() };

                ID2D1Bitmap *Bitmap;
                Canvas->GetDXObject()->GetBitmap(&Bitmap);
                VBitmapBrush BitmapBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Bitmap);

                DCRenderTarget->BindDC(GetImageHDC(&BorderImage), &WindowRect);

                DCRenderTarget->BeginDraw();

                DCRenderTarget->FillRoundedRectangle(D2D1_ROUNDED_RECT{
                        {0, 0, static_cast<float>(GetWidth() - 1), static_cast<float>(GetHeight() - 1)},
                        static_cast<float>(WindowConfig.BorderRadius.X),
                        static_cast<float>(WindowConfig.BorderRadius.Y)}, BitmapBrush.GetDxBrush());

                DCRenderTarget->EndDraw();

                HDC WindowDC = GetDC(GetHWnd());

                BLENDFUNCTION BlendFN = { 0 };
                BlendFN.BlendOp = AC_SRC_OVER;
                BlendFN.SourceConstantAlpha = 255;
                BlendFN.AlphaFormat = AC_SRC_ALPHA;

                POINT	SourcePoint = { 0, 0 };
                SIZE	WindowSize = { GetWidth(), GetHeight() };
                UpdateLayeredWindow(GetHWnd(), WindowDC, NULL, &WindowSize, GetImageHDC(&BorderImage),
                                    &SourcePoint, NULL, &BlendFN, ULW_ALPHA);

                ReleaseDC(GetHWnd(), WindowDC);

                Bitmap->Release();
                DCRenderTarget->Release();
            } else {
                BufferPainter->BeginDraw();
                BufferPainter->DrawCanvas(GetRegion(), Canvas, GetRegion(), 1.f);
                BufferPainter->EndDraw();
            }

            delete Canvas;
        }
    }
}

void VMainWindow::SetTitle(const std::wstring& WindowText) {
    Theme->WindowPlaneText = WindowText;
    SetWindowTextW(GetHWnd(), Theme->WindowPlaneText.c_str());
}
void VMainWindow::SetBackgroundColor(const VColor& Color) {
    Theme->BackgroundColor = Color;

    Update( { 0, 0, GetWidth(), GetHeight() } );
}
void VMainWindow::SetMaxSize(const VGeomtery& MaxSize) {
    WindowConfig.UseMaxMinSize = true;
    WindowConfig.WindowMaxSize = MaxSize;
}
void VMainWindow::SetMiniSize(const VGeomtery& MiniSize) {
    WindowConfig.UseMaxMinSize = true;
    WindowConfig.WindowMiniSize = MiniSize;
}
void VMainWindow::SetFrameless(const bool& FramelessStatus) {
    if (FramelessStatus) {
        WindowConfig.InFrameless = true;

        SetWindowLongPtr(GetLocalWinId(), GWL_STYLE, GetWindowLongPtr(GetLocalWinId(), GWL_STYLE) & ~ (WS_THICKFRAME | WS_SIZEBOX | WS_CAPTION));

        RECT WindowRect;
        GetWindowRect(GetLocalWinId(), &WindowRect);
        MoveWindow(GetLocalWinId(), WindowRect.left, WindowRect.top, GetWidth(), GetHeight(), FALSE);
    } else {
        WindowConfig.InFrameless = false;

        SetWindowLongPtr(GetLocalWinId(), GWL_STYLE, 349110272ll);
    }
}
void VMainWindow::SetSizble(const bool& Sizble) {
    WindowConfig.Sizable = Sizble;

    if (Sizble) {
        SetWindowLong(GetHWnd(), GWL_STYLE, GetWindowLong(GetHWnd(), GWL_STYLE) | (WS_MAXIMIZEBOX));
        SetWindowLong(GetHWnd(), GWL_STYLE, (GetWindowLong(GetHWnd(), GWL_STYLE) | WS_THICKFRAME));
    }
    else {
        SetWindowLong(GetHWnd(), GWL_STYLE, GetWindowLong(GetHWnd(), GWL_STYLE) ^ (WS_MAXIMIZEBOX));
        SetWindowLong(GetHWnd(), GWL_STYLE, (GetWindowLong(GetHWnd(), GWL_STYLE) ^ (WS_THICKFRAME)));
    }
}
void VMainWindow::SetRadius(VPoint Radius) {
    if (Radius == VPoint{ 0, 0 }) {
        WindowConfig.EnableRadius = false;
        WindowConfig.BorderRadius = Radius;

        WindowConfig.InFrameless = false;
    } else {
        SetWindowLongPtr(GetHWnd(), GWL_EXSTYLE, WS_EX_LAYERED);

        WindowConfig.EnableRadius = true;
        WindowConfig.BorderRadius = Radius;

        WindowConfig.InFrameless  = true;
    }

    Update({ 0, 0, GetWidth(), GetHeight() });
}
void VMainWindow::Resize(const int& Width, const int& Height) {
    VUIObject::Resize(Width, Height);

    if (Width > 0 && Height > 0) {
        ExResize(Width, Height);
    }
}

}

VLIB_END_NAMESPACE
