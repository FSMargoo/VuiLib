#include "../../../include/core/object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VUIObject::~VUIObject() {
    SetParent(nullptr);

    for (auto& ChildObject : ObjectKernel.ChildObjectContainer) {
        delete (VUIObject*)ChildObject;
    }

    ObjectKernel.ChildObjectContainer.clear();
}
std::vector<VBasicUITheme *> VUIObject::GetApplicationTheme() {
    if (GetParent() != nullptr) {
        return GetParent()->GetApplicationTheme();
    } else {
        VLIB_REPORT_ERROR(L"Get theme list from applicaiton failed!");

        return {nullptr};
    }
}

VBasicUITheme* VUIObject::GetTargetTheme(VUIThemeType ThemeType) {
    auto ThemeList = GetApplicationTheme();

    for (auto &Theme: ThemeList) {
        if (Theme->GetThemeType() == ThemeType) {
            return Theme;
        }
    }

    return nullptr;
}

HWND VUIObject::GetLocalWinId() {
    if (GetParent() != nullptr) {
        return GetParent()->GetLocalWinId();
    } else {
        VLIB_REPORT_ERROR(L"Get local winid from widget failed!");

        return NULL;
    }
}

VUIObject *VUIObject::GetParent() {
    return ObjectKernel.Parent;
}

VRect VUIObject::GetRegion() {
    return ObjectVisual.Rectangle;
}

VCanvasPainter *VUIObject::GetParentCanvas() {
    return GetParent()->Canvas;
}

VRect VUIObject::HelperGetSourceRect() {
    return VRect{0, 0, ObjectVisual.Rectangle.GetWidth(), ObjectVisual.Rectangle.GetHeight()};
}

std::wstring VUIObject::GeneratedGlobalID(std::wstring TargetID) {
    if (!GetParent()->IsApplication()) {
        TargetID += std::to_wstring(ObjectKernel.ParentID);

        return GetParent()->GeneratedGlobalID(TargetID);
    }

    return TargetID;
}

void VUIObject::CallWidgetSetIME(const int& X, const int& Y) {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetSetIME(X, Y);
    }
}

void VUIObject::CallWidgetSetFocusID(const std::wstring& ObjectID) {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetSetFocusID(ObjectID);
    }
}

std::wstring VUIObject::CallWidgetGetFocusID() {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetGetFocusID();
    }

    return L"";
}

void VUIObject::CallWidgetLockFocusID() {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetLockFocusID();
    }
}

void VUIObject::CallWidgetUnlockFocusID() {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetUnlockFocusID();
    }
}

bool VUIObject::CallWidgetGetLockingStatus() {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetGetLockingStatus();
    }

    return false;
}

void VUIObject::CallWidgetSendMessage(VMessage *Message) {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetSendMessage(Message);
    }
}

Core::VDCRender *VUIObject::CallWidgetGetDCRenderTarget() {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetGetDCRenderTarget();
    }

    return nullptr;
}
Core::VCanvasPainter *VUIObject::CallWidgetGetCanvas() {
    if (GetParent() != nullptr) {
        return GetParent()->CallWidgetGetCanvas();
    }

    return nullptr;
}


void VUIObject::Update(VRect UpdateRect) {
    if (GetParent() != nullptr) {
        auto ParentRectangle = GetParent()->GetRegion().Clone();
        ParentRectangle.Move(0, 0);

        if (UpdateRect.Overlap(ParentRectangle)) {
            if (GetParent()->IsWidget()) {
                UpdateRect = GetParent()->GetRegion();
            }

            return GetParent()->Update(UpdateRect);
        }
    }
}

void VUIObject::Update() {
    Update(GetRegion());
}

void VUIObject::CheckAllFrame(bool RecursionChildren) {
    CheckFrame();

    if (!RecursionChildren) {
        for (auto &Object: ObjectKernel.ChildObjectContainer) {
            Object->CheckFrame();
        }
    } else {
        for (auto &Object: ObjectKernel.ChildObjectContainer) {
            Object->CheckAllFrame(RecursionChildren);
        }
    }
}

int VUIObject::GetX() const {
    return ObjectVisual.Rectangle.Left;
}

int VUIObject::GetY() const {
    return ObjectVisual.Rectangle.Top;
}

int VUIObject::GetWidth() const {
    return ObjectVisual.Rectangle.GetWidth();
}

int VUIObject::GetHeight() const {
    return ObjectVisual.Rectangle.GetHeight();
}

void VUIObject::Resize(const int &Width, const int &Height) {
    auto OldRect = ObjectVisual.Rectangle.Clone();

    ObjectVisual.Rectangle.Right = ObjectVisual.Rectangle.Left + Width;
    ObjectVisual.Rectangle.Bottom = ObjectVisual.Rectangle.Top + Height;

    OldRect.FusionRect(ObjectVisual.Rectangle);

    Update(OldRect);

    Resized.Emit(Width, Height);
}

void VUIObject::Move(const int &X, const int &Y) {
    auto OldRect = ObjectVisual.Rectangle.Clone();

    auto Width = GetWidth();
    auto Height = GetHeight();

    ObjectVisual.Rectangle.Left = X;
    ObjectVisual.Rectangle.Top = Y;
    ObjectVisual.Rectangle.Right = X + Width;
    ObjectVisual.Rectangle.Bottom = Y + Height;

    OldRect.Extended(1, 1, 1, 1);

    Update();
    Update(OldRect);

    Moved.Emit(X, Y);
}

void VUIObject::Resize(const Core::VGeomtery &Size) {
    Resize(Size.Width, Size.Height);
}

void VUIObject::Move(const VPoint &Point) {
    Move(Point.X, Point.Y);
}

void VUIObject::SetParent(VUIObject *Parent) {
    if (ObjectKernel.Parent != nullptr) {
        for (auto Object = GetParent()->ObjectKernel.ChildObjectContainer.begin() + (ObjectKernel.ParentID + 1);
             Object != GetParent()->ObjectKernel.ChildObjectContainer.end(); ++Object) {
            --(Object.operator*()->ObjectKernel.ParentID);
        }

        GetParent()->ObjectKernel.ChildObjectContainer.erase(GetParent()->ObjectKernel.ChildObjectContainer.begin()
                                                             + ObjectKernel.ParentID);
    }

    ObjectKernel.Parent = Parent;

    if (Parent != nullptr) {
        GetParent()->ObjectKernel.ChildObjectContainer.push_back(this);

        ObjectKernel.ParentID = GetParent()->ObjectKernel.ChildObjectContainer.size() - 1;
        ObjectKernel.GlobalID = GeneratedGlobalID();
    }

    Update();
}

void VUIObject::SetTransparency(const float &Transparency) {
    ObjectVisual.Transparency = Transparency;

    Update();
}

float VUIObject::GetTransparency() const {
    return ObjectVisual.Transparency;
}

bool VUIObject::CheckUIFocusStatus(const VPoint &MousePosition, VMessage *SourceMessage) {
    if ((IsAnimation() || !GetWidth() || !GetHeight()) ||
        (CallWidgetGetLockingStatus() && CallWidgetGetFocusID() != ObjectKernel.GlobalID)) {
        return false;
    }

    if (MousePosition.InsideRectangle(GetRegion())) {
        if (SourceMessage->GetType() == VMessageType::CheckLocalFocusMessage) {
            if (CallWidgetGetFocusID() != ObjectKernel.GlobalID) {
                ObjectVisual.Stats = VUIObjectUIStats::Normal;

                Update();
                LosedMouseFocus();

                LosedFocus.Emit();

                return false;
            }

            return false;
        }
        if (SourceMessage->GetType() == VMessageType::MouseClickedMessage) {
            auto MouseClickedMessage = static_cast<VMouseClickedMessage *>(SourceMessage);

            switch (MouseClickedMessage->ClickedKey) {
                case VMouseKeyFlag::Right: {
                    MouseRightClicked(MouseClickedMessage->ClickedMethod);

                    break;
                }
                case VMouseKeyFlag::Left: {
                    MouseLeftClicked(MouseClickedMessage->ClickedMethod);

                    break;
                }
                case VMouseKeyFlag::Middle: {
                    MouseMiddleClicked(MouseClickedMessage->ClickedMethod);

                    break;
                }
            }
        }

        if (ObjectVisual.Stats != VUIObjectUIStats::OnFocus) {
            ObjectVisual.Stats = VUIObjectUIStats::OnFocus;

            Update();

            CallWidgetSetFocusID(ObjectKernel.GlobalID);

            GotMouseFocus();

            InFocus.Emit();

            VCheckFocusMessage *CheckFocus = new VCheckFocusMessage(MousePosition);

            CallWidgetSendMessage(CheckFocus);
            delete CheckFocus;
        }

        return true;
    } else if (ObjectVisual.Stats == VUIObjectUIStats::OnFocus) {
        ObjectVisual.Stats = VUIObjectUIStats::Normal;

        Update();

        CallWidgetSetFocusID(L"");

        LosedMouseFocus();
        LosedFocus.Emit();

        return false;
    }

    return false;
}

bool VUIObject::SysProcessMessage(Core::VMessage *Message) {
    if (IsApplication()) {
        return SendMessageToChild(Message, false);
    }

    if (ObjectVisual.Stats == VUIObjectUIStats::Hidden) {
        return false;
    }

    OnMessage(Message);

    switch (Message->GetType()) {
        case VMessageType::GetRepaintAeraMessage: {
            VGetRepaintAeraMessage *RepaintMessage = static_cast<VGetRepaintAeraMessage *>(Message);
            if (*(RepaintMessage->RepaintAera) != GetRegion() &&
                RepaintMessage->RepaintAera->Overlap(GetRegion())) {
                (*(RepaintMessage->RepaintAera)).FusionRect(GetRegion());

                return true;
            }

            return false;
        }
        case VMessageType::MouseMoveMessage: {
            return CheckElementUIStatus(Message);
        }
        case VMessageType::MouseClickedMessage: {
            return CheckElementUIStatus(Message);
        }
        case VMessageType::IMECharMessage: {
            auto IMEMessage = static_cast<VIMECharMessage *>(Message);

            if (!IMECharInputed(IMEMessage->IMEChar)) {
                SendMessageToChild(IMEMessage);
            }

            break;
        }
        case VMessageType::RepaintMessage: {
            auto RepaintMesage = static_cast<VRepaintMessage *>(Message);

            if (RepaintMesage->DirtyRectangle.Overlap(GetRegion()) &&
                (GetParent()->IsApplication() || GetParent()->GetRegion().Clone().
                                MoveRV(GetParent()->GetX(), GetParent()->GetY())
                        ->Overlap(GetRegion()))
                    ) {

                VRepaintMessage *ChildRepaintMessage = RepaintMesage;

                if (Canvas != nullptr) {
                    delete Canvas;

                    Canvas = nullptr;
                }

                if (ObjectVisual.Transparency != 0) {
                    Canvas = new VCanvasPainter(GetRegion().GetWidth(),
                                                GetRegion().GetHeight(),
                                                CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());
                    Canvas->BeginDraw();
                    Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
                    Canvas->EndDraw();

                    OnPaint(Canvas);

                    if (!IsWidget() && !IsApplication()) {
                        ChildRepaintMessage = new VRepaintMessage(*RepaintMesage);

                        ChildRepaintMessage->DirtyRectangle = *(GetRegion().Clone().MoveRV(0, 0));
                    }

                    Canvas->BeginDraw();
                    SendMessageToChild(ChildRepaintMessage, false);
                    Canvas->EndDraw();

                    if (ChildRepaintMessage != RepaintMesage) {
                        delete ChildRepaintMessage;
                    }

                    EditCanvas(Canvas);

                    GetParentCanvas()->DrawCanvas(GetRegion(), Canvas, { 0, 0, GetRegion().GetWidth(), GetRegion().GetHeight() }, ObjectVisual.Transparency);

                    delete Canvas;

                    Canvas = nullptr;
                }

                return true;
            }

            return false;
        }
        case VMessageType::MouseWheelMessage: {
            auto WheelMessage = static_cast<VMouseWheelMessage *>(Message);

            return MouseMiddleDraged(WheelMessage->WheelValue);
        }
        case VMessageType::KeyClickedMessage: {
            auto KeyMessage = static_cast<VKeyClickedMessage *>(Message);

            if (!CheckDown(KeyMessage)) {
                SendMessageToChild(KeyMessage);
            }

            break;
        }
        case VMessageType::FreeResourceMessage: {
            if (Canvas != nullptr) {
                delete Canvas;

                Canvas = nullptr;
            }

            return true;
        }
        case VMessageType::CheckLocalFocusMessage: {
            auto FocusMessage = static_cast<VCheckFocusMessage *>(Message);

            CheckUIFocusStatus(FocusMessage->FocusPoint, FocusMessage);

            SendMessageToChild(Message, false);

            return false;
        }
        case VMessageType::KillFocusMessage: {
            if (CallWidgetGetFocusID() == ObjectKernel.GlobalID) {
                LosedMouseFocus();
                LosedFocus.Emit();
            }

            SendMessageToChild(Message, false);

            return true;
        }
        default: {
            return false;
        }
    }

    return false;
}

bool VUIObject::SendMessageToChild(Core::VMessage *Message, bool BreakWhenMeetTrue) {
    EditChildMessage(Message);

    VMessage *FocusMessage = Message;

    switch (Message->GetType()) {
        case VMessageType::CheckLocalFocusMessage: {
            FocusMessage = new VCheckFocusMessage(*(static_cast<VCheckFocusMessage *>(Message)));
            static_cast<VCheckFocusMessage *>(FocusMessage)->FocusPoint.Offset(-GetRegion().Left, -GetRegion().Top);

            break;
        }
        case VMessageType::MouseMoveMessage: {
            FocusMessage = new VMouseMoveMessage(*(static_cast<VMouseMoveMessage *>(Message)));
            static_cast<VMouseMoveMessage *>(FocusMessage)->MousePosition.Offset(-GetRegion().Left,
                                                                                 -GetRegion().Top);

            break;
        }
        case VMessageType::MouseClickedMessage: {
            FocusMessage = new VMouseClickedMessage(*(static_cast<VMouseClickedMessage *>(Message)));
            static_cast<VMouseClickedMessage *>(FocusMessage)->MousePosition.Offset(-GetRegion().Left,
                                                                                    -GetRegion().Top);

            break;
        }
        default: {
            break;
        }
    }

    bool Flag = false;

    if (Message->GetType() != VMessageType::RepaintMessage) {
        for (auto Object = ObjectKernel.ChildObjectContainer.rbegin();
             Object != ObjectKernel.ChildObjectContainer.rend(); ++Object) {
            if (Object.operator*()->SysProcessMessage(FocusMessage)) {
                if (BreakWhenMeetTrue) {
                    if (FocusMessage != Message) {
                        delete FocusMessage;
                    }

                    return true;
                } else {
                    Flag = true;
                }
            }
        }
    } else {
        for (auto Object = ObjectKernel.ChildObjectContainer.begin();
             Object != ObjectKernel.ChildObjectContainer.end(); ++Object) {
            if (Object.operator*()->SysProcessMessage(FocusMessage)) {
                if (BreakWhenMeetTrue) {
                    if (FocusMessage != Message) {
                        delete FocusMessage;
                    }

                    return true;
                } else {
                    Flag = true;
                }
            }
        }
    }

    if (FocusMessage != Message) {
        delete FocusMessage;
    }

    return Flag;
}

bool VUIObject::CheckElementUIStatus(VMessage *SourceMessage) {
    VPoint MousePosition;

    switch (SourceMessage->GetType()) {
        case VMessageType::MouseMoveMessage: {
            MousePosition = static_cast<VMouseMoveMessage *>(SourceMessage)->MousePosition;

            break;
        }
        case VMessageType::MouseClickedMessage: {
            MousePosition = static_cast<VMouseClickedMessage *>(SourceMessage)->MousePosition;

            break;
        }

        default: {
            return false;
        }
    }

    if (SendMessageToChild(SourceMessage, true)) {
        return true;
    } else {
        auto Result = CheckUIFocusStatus(MousePosition, SourceMessage);

        return Result;
    }

    return false;
}

void VUIObject::Hide() {
    ObjectVisual.Stats = VUIObjectUIStats::Hidden;

    Update();
}
void VUIObject::Show() {
    ObjectVisual.Stats = VUIObjectUIStats::Normal;

    Update();
}

}

VLIB_END_NAMESPACE