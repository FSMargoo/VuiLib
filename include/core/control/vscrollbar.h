// License(MIT)
// Athuor: Margoo
#pragma once

#include "vdragcontrolbase.h"
#include "vpushbutton.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VDragTowardsMode {
    Vertical, Horizontal
};

class VCircleScrollBarButton : public VDragControlBaseOnPushButton {
private:
    VRect            DraggedRange;
    VDragTowardsMode DragTowardsMode;

private:
    void UserOnDrag(const int& MouseX, const int& MouseY);

public:
    VCircleScrollBarButton(const VDragTowardsMode& DragTowards, VUIObject* Parent);
    VCircleScrollBarButton(const VDragTowardsMode& DragTowards, const int& Width, const int& Height, VUIObject* Parent);

public:
    void LosedMouseFocus() override;

public:
    void OnPaint(VCanvasPainter* Painter) override;

public:
    void SetDragRange(const VRect& Range);
};

class VSliderHorizontal : public VUIObject {
private:
    VSliderTheme* Theme;

    VCircleScrollBarButton*
                        SliderButton;
    double              SliderPercent = 0.f;

private:
    inline int GetSliderButtonX() const;
    inline int GetSliderButtonY() const;

    inline int GetUnselectedAreaX() const;
    inline int GetUnselectedAreaWidth() const;
    inline int GetSelectedAreaWidth() const;

private:
    void SliderButtonDraged(const int&, const int&);

public:
    VSignal<const double&> ValueChanged;

public:
    VSliderHorizontal(VUIObject* Parent);
    VSliderHorizontal(const int& Width, VUIObject* Parent);

public:
    void OnPaint(VCanvasPainter* Painter) override;

public:
    void Move(const int& X, const int& Y) override;
    void Resize(const int& Width, const int& Height) override;

public:
    VSliderTheme* GetTheme();
};
class VSliderVertical : public VUIObject {
private:
    VSliderTheme* Theme;

    VCircleScrollBarButton*
            SliderButton;
    double              SliderPercent = 0.f;

private:
    inline int GetSliderButtonX() const;
    inline int GetSliderButtonY() const;

    inline int GetUnselectedAreaY() const;
    inline int GetUnselectedAreaHeight() const;
    inline int GetSelectedAreaHeight() const;

private:
    void SliderButtonDraged(const int&, const int&);

public:
    VSignal<const double&> ValueChanged;

public:
    VSliderVertical(VUIObject* Parent);
    VSliderVertical(const int& Height, VUIObject* Parent);

public:
    void OnPaint(VCanvasPainter* Painter) override;

public:
    void Move(const int& X, const int& Y) override;
    void Resize(const int& Width, const int& Height) override;

public:
    VSliderTheme* GetTheme();
};

}


VLIB_END_NAMESPACE