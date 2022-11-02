// License(MIT)
// Athuor: Margoo
// This is the basic UI object in vuilib
#pragma once

#include "../vbasiccore.h"
#include "../widget/vmessage.h"
#include "../render/vpainter.h"
#include "../uibasic/vuitheme.h"
#include "../signal/vsignal.h"

#include <vector>

VLIB_BEGIN_NAMESPACE

namespace Core {

class VUIObject;

struct VUIObjectKernel {
    VUIObject *Parent = nullptr;

    size_t ParentID = 0;
    std::wstring GlobalID = L"VO";

    std::vector<VUIObject *> ChildObjectContainer;
};

struct VUIObjectShadowProperty {
    VColor ShadowColor  = VColor(0.f, 0.f, 0.f, 1.f);
    float  ShadowRadius = 2;
    bool   EnableShadow = false;
    VPoint ShadowOffset = { 0, 0 };
};

enum VUIObjectUIStats {
    Hidden, OnClicked, OnFocus, Normal
};

struct VUIObjectSurface {
    VRect  Rectangle;
    float  Transparency = 1.f;
    VUIObjectShadowProperty Shadow;

    VUIObjectUIStats Stats = VUIObjectUIStats::Normal;
};

class VUIObject {
protected:
    VUIObjectKernel  ObjectKernel;
    VUIObjectSurface ObjectVisual;

public:
    VUIObject *GetParent();

    virtual VRect GetRegion();
    virtual VRect GetChildrenVisualRegion();

    virtual HWND GetLocalWinId();

protected:
    virtual std::vector<VBasicUITheme *> GetApplicationTheme();

public:
    virtual bool IsAnimation() { return false; }

    virtual bool IsApplication() { return false; }

    virtual bool IsWidget() { return false; }

protected:
    VCanvasPainter* Canvas = nullptr;

    VCanvasPainter* GetParentCanvas();

    VBasicUITheme* GetTargetTheme(VUIThemeType ThemeType);

public:
    void SetShadowStats(const bool& Stats);
    void SetShadowColor(const VColor& Color);
    void SetShadowRadius(const float& Radius);

public:
    VSignal<> InFocus;
    VSignal<> LosedFocus;
    VSignal<const int &, const int &> Resized;
    VSignal<const int &, const int &> Moved;

private:
    std::wstring GeneratedGlobalID(std::wstring TargetID = L"");

protected:
    virtual void RestoreMousePosition(VPoint* MousePosition);

protected:
    virtual void CallWidgetSetIME(const int& X, const int& Y);

    virtual void CallWidgetSetFocusID(const std::wstring& ObjectID);

    virtual std::wstring CallWidgetGetFocusID();

    virtual void CallWidgetLockFocusID();

    virtual void CallWidgetUnlockFocusID();

    virtual bool CallWidgetGetLockingStatus();

    virtual void CallWidgetSendMessage(VMessage *Message);

public:
    virtual Core::VDCRender* CallWidgetGetDCRenderTarget();

protected:
    virtual Core::VCanvasPainter* CallWidgetGetCanvas();

protected:
    virtual void EditChildMessage(VMessage *Message) {}

protected:
    VRect HelperGetSourceRect();

public:
    virtual void Update(VRect UpdateRect);

    void Update();

protected:
    virtual void OnPaint(VCanvasPainter *) {}

    virtual void EditCanvas(VCanvasPainter *) {}

    virtual bool OnMessageTrigger(VRepaintMessage *Message);

public:
    virtual void GotMouseFocus() {}

    virtual void LosedMouseFocus() {}

    virtual void MouseLeftClicked(VMouseClickedFlag) {}

    virtual void MouseRightClicked(VMouseClickedFlag) {}

    virtual void MouseMiddleClicked(VMouseClickedFlag) {}

    virtual bool MouseMiddleDragged(short) { return false; }

    virtual bool CheckDown(VKeyClickedMessage *) { return false; }

    virtual bool IMECharInputed(wchar_t Message) { return false; }

public:
    virtual void CheckFrame() {}

    void CheckAllFrame(bool RecursionChildren = false);

public:
    int GetX() const;

    int GetY() const;

    int GetWidth() const;

    int GetHeight() const;

public:
    virtual void Resize(const int &Width, const int &Height);

    virtual void Move(const int &X, const int &Y);

    void Resize(const VGeomtery &Size);

    void Move(const VPoint &Point);

public:
    void SetParent(VUIObject *Parent);

public:
    void SetTransparency(const float &Transparency);

    float GetTransparency() const;

public:
    virtual void OnMessage(VMessage *Message) {}

    bool SysProcessMessage(VMessage *Message);

protected:
    bool SendMessageToChild(VMessage *Message, bool BreakWhenMeetTrue = true);

    bool CheckUIFocusStatus(const VPoint &MousePosition, VMessage *SourceMessage);

    bool CheckElementUIStatus(VMessage *SourceMessage);
public:
    void Hide();
    void Show();

public:
    explicit VUIObject(VUIObject *Parent) {
        SetParent(Parent);
    }
    ~VUIObject();
};

}

VLIB_END_NAMESPACE