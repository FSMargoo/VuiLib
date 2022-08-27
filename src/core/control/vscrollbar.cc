#include "../../../include/core/control/vscrollbar.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VCircleScrollBarButton::VCircleScrollBarButton(const VDragTowardsMode& DragTowards, VUIObject* Parent)
    : VDragControlBaseOnPushButton(Parent) {
    DragTowardsMode = DragTowards;
    MouseDragged.Connect(this, &VCircleScrollBarButton::UserOnDrag);

    delete Theme;
    Theme = new VCircleScrollBarTheme();
}
VCircleScrollBarButton::VCircleScrollBarButton(const VDragTowardsMode& DragTowards, const int& Width, const int& Height, VUIObject* Parent)
    : VDragControlBaseOnPushButton(Width, Height, Parent) {
    DragTowardsMode = DragTowards;
    MouseDragged.Connect(this, &VCircleScrollBarButton::UserOnDrag);

    delete Theme;
    Theme = new VCircleScrollBarTheme();
}
void VCircleScrollBarButton::OnPaint(Core::VCanvasPainter *Painter) {
    VSolidBrush BackgroundBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                                Theme->LocalTheme.BackgroundColor);
    VPenBrush   PenBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                         Theme->LocalTheme.BorderColor, Theme->LocalTheme.BorderThickness);
    VSolidBrush TextBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(),
                          Theme->LocalTheme.TextColor);

    Painter->BeginDraw();

    Painter->FillEllipse({
                            static_cast<int>(Theme->LocalTheme.BorderThickness),
                            static_cast<int>(Theme->LocalTheme.BorderThickness),
                            GetWidth(), GetHeight()
                        }, &PenBrush, &BackgroundBrush);

    Painter->EndDraw();
}
void VCircleScrollBarButton::SetDragRange(const VRect& Range) {
    DraggedRange = Range;
}
void VCircleScrollBarButton::LosedMouseFocus() {
    if (!UserInDrag) {
        VPushButton::LosedMouseFocus();
    }
}
void VCircleScrollBarButton::UserOnDrag(const int &MouseX, const int &MouseY) {
    switch (DragTowardsMode) {
        case VDragTowardsMode::Vertical: {
            if (MouseY >= DraggedRange.Top &&
                MouseY <= DraggedRange.Bottom) {
                Move(GetX(), MouseY);
            }

            break;
        }
        case VDragTowardsMode::Horizontal: {
            if (MouseX >= DraggedRange.Left &&
                MouseX <= DraggedRange.Right) {
                Move(MouseX, GetY());
            }

            break;
        }
    }
}

VSliderHorizontal::VSliderHorizontal(VUIObject* Parent)
    : VUIObject(Parent) {
    Theme = new VSliderTheme(*(static_cast<VSliderTheme*>(GetTargetTheme(VUIThemeType::VSlider))));

    SliderButton = new VCircleScrollBarButton(VDragTowardsMode::Horizontal, Parent);
    SliderButton->Resize(18, 18);
    SliderButton->Move(GetSliderButtonX(), GetSliderButtonY());
    SliderButton->SetDragRange( { GetX(), GetY(), GetWidth(), GetHeight() } );

    SliderButton->MouseDragged.Connect(this, &VSliderHorizontal::SliderButtonDraged);
}
VSliderHorizontal::VSliderHorizontal(const int& Width, VUIObject* Parent)
    : VUIObject(Parent) {
    Theme = new VSliderTheme(*(static_cast<VSliderTheme*>(GetTargetTheme(VUIThemeType::VSlider))));

    SliderButton = new VCircleScrollBarButton(VDragTowardsMode::Horizontal, Parent);
    SliderButton->Resize(18, 18);
    SliderButton->Move(GetSliderButtonX(), GetSliderButtonY());
    SliderButton->SetDragRange( { GetX(), GetY(), GetWidth(), GetHeight() } );

    SliderButton->MouseDragged.Connect(this, &VSliderHorizontal::SliderButtonDraged);

    Resize(Width, 4);
}

int VSliderHorizontal::GetSliderButtonX() const {
    return SliderPercent * GetWidth() + GetX();
}
int VSliderHorizontal::GetSliderButtonY() const {
    return GetY() - 6;
}
VSliderTheme* VSliderHorizontal::GetTheme() {
    return Theme;
}
int VSliderHorizontal::GetSelectedAreaWidth() const {
    return GetWidth() * SliderPercent;
}
int VSliderHorizontal::GetUnselectedAreaWidth() const {
    return GetWidth() - GetSelectedAreaWidth();
}
int VSliderHorizontal::GetUnselectedAreaX() const {
    return GetSelectedAreaWidth();
}
void VSliderHorizontal::OnPaint(VCanvasPainter* Painter) {
    Painter->BeginDraw();

    // Draw Selected area
    {
        VSolidBrush SelectBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->SelectedArea.LocalTheme.BackgroundColor);
        VPenBrush   SelectBorder(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->SelectedArea.LocalTheme.BorderColor,
                                   Theme->SelectedArea.LocalTheme.BorderThickness);

        Painter->FillRoundedRectangle( { 0, 0, GetSelectedAreaWidth(), 4 },
                                       VPoint{ 4, 4 }, &SelectBorder, &SelectBrush);
    }

    // Draw Unselected area
    {
        VSolidBrush UnselectBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->UnselectedArea.LocalTheme.BackgroundColor);
        VPenBrush   UnselectBorder(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->UnselectedArea.LocalTheme.BorderColor,
                                   Theme->UnselectedArea.LocalTheme.BorderThickness);

        Painter->FillRoundedRectangle( { GetUnselectedAreaX(), 0, GetUnselectedAreaX() + GetUnselectedAreaWidth(), 4 },
                                           VPoint{ 4, 4 }, &UnselectBorder, &UnselectBrush);
    }

    Painter->EndDraw();
}
void VSliderHorizontal::Move(const int& X, const int& Y) {
    VUIObject::Move(X, Y);
    SliderButton->Move(GetSliderButtonX(), GetSliderButtonY());
    SliderButton->SetDragRange( { GetX(), GetY(), GetX() + GetWidth() - 14, GetY() + GetHeight() } );
}
void VSliderHorizontal::SliderButtonDraged(const int&, const int&) {
    SliderPercent = double(SliderButton->GetX() - GetX()) / (GetWidth() - 14);

    ValueChanged.Emit(SliderPercent);

    Update();
}
void VSliderHorizontal::Resize(const int &Width, const int &Height) {
    VUIObject::Resize(Width, 4);
}
double VSliderHorizontal::GetValue() const {
    return SliderPercent;
}
void VSliderHorizontal::SetValue(const double& Value) {
    SliderPercent = Value;

    Update();
}
VCircleScrollBarButton* VSliderHorizontal::GetScrollBarButtonInstance() {
    return SliderButton;
}

VSliderVertical::VSliderVertical(VUIObject* Parent)
        : VUIObject(Parent) {
    Theme = new VSliderTheme(*(static_cast<VSliderTheme*>(GetTargetTheme(VUIThemeType::VSlider))));

    SliderButton = new VCircleScrollBarButton(VDragTowardsMode::Vertical, Parent);
    SliderButton->Resize(18, 18);
    SliderButton->Move(GetSliderButtonX(), GetSliderButtonY());
    SliderButton->SetDragRange( { GetX(), GetY(), GetWidth(), GetHeight() } );

    SliderButton->MouseDragged.Connect(this, &VSliderVertical::SliderButtonDraged);
}
VSliderVertical::VSliderVertical(const int& Height, VUIObject* Parent)
            : VUIObject(Parent) {
    Theme = new VSliderTheme(*(static_cast<VSliderTheme*>(GetTargetTheme(VUIThemeType::VSlider))));

    SliderButton = new VCircleScrollBarButton(VDragTowardsMode::Vertical, Parent);
    SliderButton->Resize(18, 18);
    SliderButton->Move(GetSliderButtonX(), GetSliderButtonY());
    SliderButton->SetDragRange( { GetX(), GetY(), GetWidth(), GetHeight() } );

    SliderButton->MouseDragged.Connect(this, &VSliderVertical::SliderButtonDraged);

    Resize(4, Height);
}

int VSliderVertical::GetSliderButtonX() const {
    return GetX() - 6;
}
int VSliderVertical::GetSliderButtonY() const {
    return SliderPercent * GetHeight() + GetY();
}
VSliderTheme* VSliderVertical::GetTheme() {
    return Theme;
}
int VSliderVertical::GetSelectedAreaHeight() const {
    return GetHeight() * SliderPercent;
}
int VSliderVertical::GetUnselectedAreaHeight() const {
    return GetHeight() - GetSelectedAreaHeight();
}
int VSliderVertical::GetUnselectedAreaY() const {
    return GetSelectedAreaHeight();
}
void VSliderVertical::OnPaint(VCanvasPainter* Painter) {
    Painter->BeginDraw();

    // Draw Selected area
    {
        VSolidBrush SelectBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->SelectedArea.LocalTheme.BackgroundColor);
        VPenBrush   SelectBorder(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->SelectedArea.LocalTheme.BorderColor,
                                 Theme->SelectedArea.LocalTheme.BorderThickness);

        Painter->FillRoundedRectangle( { 0, 0, 4, GetSelectedAreaHeight() },
                                       VPoint{ 4, 4 }, &SelectBorder, &SelectBrush);
    }

    // Draw Unselected area
    {
        VSolidBrush UnselectBrush(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->UnselectedArea.LocalTheme.BackgroundColor);
        VPenBrush   UnselectBorder(CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget(), Theme->UnselectedArea.LocalTheme.BorderColor,
                                   Theme->UnselectedArea.LocalTheme.BorderThickness);

        Painter->FillRoundedRectangle( { 0, GetUnselectedAreaY(), 4, GetUnselectedAreaY() + GetUnselectedAreaHeight() },
                                       VPoint{ 4, 4 }, &UnselectBorder, &UnselectBrush);
    }

    Painter->EndDraw();
}
void VSliderVertical::Move(const int& X, const int& Y) {
    VUIObject::Move(X, Y);
    SliderButton->Move(GetSliderButtonX(), GetSliderButtonY());
    SliderButton->SetDragRange( { GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight() - 14 } );
}
void VSliderVertical::SliderButtonDraged(const int&, const int&) {
    SliderPercent = double(SliderButton->GetY() - GetY()) / (GetHeight() - 14);

    ValueChanged.Emit(SliderPercent);

    Update();
}
void VSliderVertical::Resize(const int &Width, const int &Height) {
    VUIObject::Resize(4, Height);
}
double VSliderVertical::GetValue() const {
    return SliderPercent;
}
void VSliderVertical::SetValue(const double& Value) {
    SliderPercent = Value;

    Update();
}
VCircleScrollBarButton* VSliderVertical::GetScrollBarButtonInstance() {
    return SliderButton;
}

}

VLIB_END_NAMESPACE