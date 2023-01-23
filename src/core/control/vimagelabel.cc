#include "../../../include/core/control/vimagelabel.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

VImageLabel::VImageLabel(const int &Width, const int &Height, Core::VUIObject *Parent) : VUIObject(Parent)
{
	Theme		 = new VImageLabelTheme;
	Theme->Image = nullptr;

	Resize(Width, Height);
}
VImageLabel::VImageLabel(const int &Width, const int &Height, VImage *Image, VUIObject *Parent) : VUIObject(Parent)
{
	Theme = new VImageLabelTheme;

	if (Image != nullptr)
	{
		Theme->Image = new VImage(*Image);
	}
	else
	{
		Theme->Image = nullptr;
	}

	Resize(Width, Height);
}
VImageLabel::VImageLabel(const int &Width, const int &Height, const std::wstring &ImagePath, VUIObject *Parent)
	: VUIObject(Parent)
{
	Theme		 = new VImageLabelTheme;
	Theme->Image = new VImage(ImagePath.c_str(), CallWidgetGetRenderHandle());

	Resize(Width, Height);
}
VImageLabel::~VImageLabel()
{
	delete Theme;
}

VImageLabelTheme *VImageLabel::GetTheme()
{
	return Theme;
}

void VImageLabel::OnPaint(VCanvasPainter *Painter)
{
	Painter->BeginDraw();

	if (Theme->Image)
	{
		Painter->DrawImage(
			VRect{
				0,
				0,
				static_cast<int>(GetRegion().GetWidth()),
				static_cast<int>(GetRegion().GetHeight()),
			},
			Theme->Image,
			VRect{
				0,
				0,
				static_cast<int>(Theme->Image->GetDirectXObject()->GetSize().width),
				static_cast<int>(Theme->Image->GetDirectXObject()->GetSize().height),
			},
			1.f);
	}

	Painter->EndDraw();
}

void VImageLabel::SetImage(VImage *Image)
{
	if (Image != nullptr)
	{
		if (Theme->Image != nullptr)
		{
			*(Theme->Image) = *Image;
		}
		else
		{
			Theme->Image = new VImage(*Image);
		}
	}
}

} // namespace Core

VLIB_END_NAMESPACE