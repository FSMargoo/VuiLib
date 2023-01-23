#include "../../../include/core/render/vcolor.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
VColor::VColor() : DxColorObject(0, 0, 0, 0)
{
}
VColor::VColor(const VColor &ColorObject) : DxColorObject(ColorObject.DxColorObject)
{
}
VColor::VColor(const ColorEnum &Color, const float &AlphaValue) : DxColorObject(Color, AlphaValue)
{
}
VColor::VColor(const UINT32 &RGB, const float &AlphaValue) : DxColorObject(RGB, AlphaValue)
{
}
VColor::VColor(const float &R, const float &G, const float &B, const float &A) : DxColorObject(R, G, B, A)
{
}

D2D1::ColorF VColor::GetDxObject() const
{
	return DxColorObject;
}

float VColor::GetR() const
{
	return DxColorObject.r;
}
float VColor::GetG() const
{
	return DxColorObject.g;
}
float VColor::GetB() const
{
	return DxColorObject.b;
}
float VColor::GetA() const
{
	return DxColorObject.a;
}

void VColor::SetR(const float &R)
{
	DxColorObject.r = R;
}
void VColor::SetG(const float &G)
{
	DxColorObject.g = G;
}
void VColor::SetB(const float &B)
{
	DxColorObject.b = B;
}
void VColor::SetA(const float &A)
{
	DxColorObject.a = A;
}
} // namespace Core

VLIB_END_NAMESPACE