#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VBlurLabel : public VUIObject {
 private:
     VBlurLabelTheme* Theme;

 public:
     VBlurLabel(VUIObject* Parent);
     ~VBlurLabel();
    VBlurLabel(const int& Width, const int &Height, VUIObject* Parent);
     VBlurLabel(const int& Width, const int &Height, const int& Radius, VUIObject* Parent);

     VBlurLabelTheme* GetTheme();

 public:
     void OnPaint(VCanvasPainter* Painter) override;
};

};

VLIB_END_NAMESPACE