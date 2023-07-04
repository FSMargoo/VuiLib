#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
class VImageLabel : public VUIObject
{
public:
	VImageLabel(VUIObject *Parent);
	VImageLabel(const int &Width, const int &Height, VUIObject *Parent);
	VImageLabel(const int &Width, const int &Height, VImage *Image, VUIObject *Parent);
	VImageLabel(const int &Width, const int &Height, const VString &ImagePath, VUIObject *Parent);
	~VImageLabel();

public:
	void	SetImage(VImage *Image);
	VImage *GetImage();
	void	Resize(const int &Width, const int &Height) override;
	void	SetLockStatus(const bool &Status);

public:
	void ResizeByImage();
	void SetBorderRadius(const VPoint &Radius);

public:
	VImageLabelTheme *GetTheme();

public:
	void OnPaint(VCanvasPainter *Painter) override;

private:
	VImageLabelTheme *Theme;
	bool			  LockHeight = false;
};
} // namespace Core

VLIB_END_NAMESPACE