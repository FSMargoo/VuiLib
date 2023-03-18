#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
class VImageLabel : public VUIObject
{
private:
	VImageLabelTheme *Theme;

public:
	VImageLabel(const int &Width, const int &Height, VUIObject *Parent);
	VImageLabel(const int &Width, const int &Height, VImage *Image, VUIObject *Parent);
	VImageLabel(const int &Width, const int &Height, const VString &ImagePath, VUIObject *Parent);
	~VImageLabel();

public:
	void SetImage(VImage *Image);

public:
	void ResizeByImage();

public:
	VImageLabelTheme *GetTheme();

public:
	void OnPaint(VCanvasPainter *Painter) override;
};
} // namespace Core

VLIB_END_NAMESPACE