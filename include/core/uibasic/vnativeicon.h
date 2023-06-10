#pragma once

#include "../render/vpainter.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
class VNativeIcon
{
public:
	static VCanvasPainter *CheckImage(const VColor &Color, const int &Width, const int &Height,
									  const VRenderHandle &Handle, const int &Thickness);
	static VCanvasPainter *DropDownUpIcon(const VColor &Color, const int &Width, const int &Height,
										  const VRenderHandle &Handle, const int &Thickness);
	static VCanvasPainter *DropDownDownIcon(const VColor &Color, const int &Width, const int &Height,
											const VRenderHandle &Handle, const int &Thickness);
};
} // namespace Core

VLIB_END_NAMESPACE