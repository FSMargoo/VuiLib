#pragma once

#include "vscroller.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VViewLabelVerticalAlign {
    Left, Right
};
enum class VViewLabelHorizontalAlign {
    Top, Bottom
};

class VViewLabel : public VUIObject {
 private:
    VRect ViewArea;

    VScrollerHorizontal* HorizontalScroller;
    VScrollerVertical* VerticalScroller;

    VViewLabelTheme* Theme;

    VViewLabelVerticalAlign VerticalAlign = VViewLabelVerticalAlign::Left;
    VViewLabelHorizontalAlign HorizontalAlign  = VViewLabelHorizontalAlign::Top;

    bool TieWithMouseWheel = false;

 public:
    VScrollerHorizontal* GetHorizontalScrollerInstance();
    VScrollerVertical* GetVerticalScrollerInstance();

 private:
    void InitScroller(VUIObject* Parent);
    void ResetScroller();

 private:
    bool OnMessageTrigger(VRepaintMessage *RepaintMessage) override;

 public:
    void OnMessage(VMessage* Message) override;

 private:
    void ViewValueOnChange(const double&);
    void EditChildMessage(VMessage* Message) override;

 protected:
    void RestoreMousePosition(VPoint* MousePoint) override;

 public:
    VRect GetChildrenVisualRegion() override;
    void Update(VRect UpdateRect) override;

 public:
    void SetViewRegion(const VPoint& ViewRegion);

 public:
    void SetVerticalAlign(const VViewLabelVerticalAlign& Align);
    void SetHorizontalAlign(const VViewLabelHorizontalAlign& Align);

 public:
    void SetTieWithWheel(const bool& TieStatus);

 public:
    void Move(const int& X, const int& Y) override;
    void Resize(const int& Width, const int& Height) override;

 public:
    VViewLabelTheme* GetTheme();

 public:
    VViewLabel(VUIObject* Parent);
    VViewLabel(const int& Width, const int& Height, VUIObject* Parent);
    VViewLabel(const int& Width, const int& Height, const int& ViewWidth, const int& ViewHeight,VUIObject* Parent);
};

}

VLIB_END_NAMESPACE