#pragma once

#include "../object/vuiobject.h"
#include "vtextlabel.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

enum class VLayoutMode
{
	LayoutModeCenter,
	LayoutModeFar,
	LayoutModePercent,
	LayoutModeRelative,
	LayoutModeRelativeBottom,
	LayoutModeRelativeTop,
	LayoutModeMiddleOffset,
	LayoutModeDontUse
};

class VLayout : public VUIObject
{
private:
	VUIObject *TargetWindow;

	VLayoutMode VerticalLayoutMode	 = VLayoutMode::LayoutModeDontUse;
	VLayoutMode HorizontalLayoutMode = VLayoutMode::LayoutModeDontUse;

	double VerticalLayoutPercent   = 0.f;
	double HorizontalLayoutPercent = 0.f;

	int RelativeX = 0;
	int RelativeY = 0;

	int XMiddleOffset = 0;
	int YMiddleOffset = 0;

private:
	void TargetWindowSizeChanged(const int &Width, const int &Height);
	void ParentSizeChanged(const int &Width, const int &Height);

public:
	/*
   * Build up Functional
   */

	VLayout(VUIObject *Parent, VUIObject *TargetWidget);
	VLayout(VUIObject *Parent, VUIObject *TargetWidget, const VLayoutMode &VerticalMode,
			const VLayoutMode &HorizontalMode);

	void SetVerticalLayoutMode(const VLayoutMode &Mode);
	void SetHorizontalLayoutMode(const VLayoutMode &Mode);

	void SetVerticalLayoutPercent(const double &Percent);
	void SetHorizontalLayoutPercent(const double &Percent);

	void SetRelativeX(const int &X);
	void SetRelativeY(const int &Y);

	void SetXMiddleOffset(const int &XOffset);
	void SetYMiddleOffset(const int &YOffset);

	int GetRelativeX() const;
	int GetRelativeY() const;
	int GetXMiddleOffset() const;
	int GetYMiddleOffset() const;
};
class VScaleLayout : public VUIObject
{
private:
	VUIObject *TargetWindow;

	double LayoutWidthPercent  = 0.f;
	double LayoutHeightPercent = 0.f;

private:
	void TargetWindowSizeChanged(const int &Width, const int &Height);

public:
	/*
   * Build up Functional
   */

	VScaleLayout(VUIObject *Parent, VUIObject *TargetWidget);
	VScaleLayout(VUIObject *Parent, VUIObject *TargetWidget, const double &WidthPercent, const double &HeightPercent);
	~VScaleLayout();

	void SetWidthScalePercent(const double &Percent);
	void SetHeightScalePercent(const double &Percent);
};

class VTextSizeLayout : public VUIObject
{
private:
	VUIObject *TargetWindow;

	double LayoutHeightPercent = 0.f;

private:
	void TargetWindowSizeChanged(const int &Width, const int &Height);

public:
	/*
   * Build up Functional
   */

	VTextSizeLayout(VTextLabel *Parent, VUIObject *TargetWidget);
	VTextSizeLayout(VTextLabel *Parent, VUIObject *TargetWidget, const double &HeightPercent);
	~VTextSizeLayout();

	void SetScalePercent(const double &Percent);
};

}; // namespace Core

VLIB_END_NAMESPACE