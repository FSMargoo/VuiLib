/*
 * VLayout.hpp
 *	@description  : Provide a Layout Helper Control
*/

#pragma once

#include "vuiobject.hpp"
#include "vwidget.hpp"

VLIB_BEGIN_NAMESPACE

/*
 * VLayoutMode enum:
 *	@description : The Mode of Layout
*/
enum class VLayoutMode {
	LayoutModeCenter, LayoutModeFar, LayoutModePercent,
	LayoutModeRelative, LayoutModeRelativeBottom, LayoutModeRelativeTop,
	LayoutModeMiddleOffset
};

/*
 * VLayout class:
 *	@description : Provide a Layout Helper
*/
class VLayout : public VUIObject {
private:
	VUIObject* TargetWindow;

	VLayoutMode  VerticalLayoutMode;
	VLayoutMode  HorizontalLayoutMode;

	double       VerticalLayoutPercent = 0.f;
	double       HorziontalLayoutPercent = 0.f;

	int          RelativeX = 0;
	int          RelativeY = 0;

	int          XMiddleOffset = 0;
	int          YMiddleOffset = 0;

private:
	void TargetWindowSizeChanged(int Width, int Height) {
		int NewX = 0;
		int NewY = 0;

		switch (HorizontalLayoutMode) {
		case VLayoutMode::LayoutModeCenter: {
			NewX = Width / 2 - Parent()->GetRegoin().GetWidth() / 2;

			break;
		}
		case VLayoutMode::LayoutModeFar: {
			NewX = Width - Parent()->GetRegoin().GetWidth();

			break;
		}
		case VLayoutMode::LayoutModePercent: {
			NewX = Width * VerticalLayoutPercent;

			break;
		}
		case VLayoutMode::LayoutModeRelative: {
			NewX = Width - RelativeX;

			break;
		}
		case VLayoutMode::LayoutModeRelativeTop: {
			NewX = RelativeX;

			break;
		}
		case VLayoutMode::LayoutModeRelativeBottom: {
			NewX = Width - RelativeX;

			break;
		}
		case VLayoutMode::LayoutModeMiddleOffset: {
			NewX = Width / 2 - Parent()->GetRegoin().GetWidth() / 2 + XMiddleOffset;

			break;
		}
		}
		switch (VerticalLayoutMode) {
		case VLayoutMode::LayoutModeCenter: {
			NewY = Height / 2 - Parent()->GetRegoin().GetHeight() / 2;

			break;
		}
		case VLayoutMode::LayoutModeFar: {
			NewY = Height - Parent()->GetRegoin().GetHeight();

			break;
		}
		case VLayoutMode::LayoutModePercent: {
			NewY = Height * VerticalLayoutPercent;

			break;
		}
		case VLayoutMode::LayoutModeRelative: {
			NewY = RelativeY;

			break;
		}
		case VLayoutMode::LayoutModeRelativeTop: {
			NewY = RelativeY;

			break;
		}
		case VLayoutMode::LayoutModeRelativeBottom: {
			NewY = Height - RelativeY;

			break;
		}
		case VLayoutMode::LayoutModeMiddleOffset: {
			NewY = Height / 2 - Parent()->GetRegoin().GetHeight() / 2 + YMiddleOffset;

			break;
		}
		}

		Parent()->Move(NewX, NewY);
	}
	void ParentSizeChanged(int Width, int Height) {
		int NewX = 0;
		int NewY = 0;

		switch (HorizontalLayoutMode) {
		case VLayoutMode::LayoutModeCenter: {
			NewX = TargetWindow->GetWidth() / 2 - Width / 2;

			break;
		}
		case VLayoutMode::LayoutModeFar: {
			NewX = TargetWindow->GetWidth() - Width;

			break;
		}
		case VLayoutMode::LayoutModePercent: {
			NewX = TargetWindow->GetWidth() * VerticalLayoutPercent;

			break;
		}
		}
		switch (VerticalLayoutMode) {
		case VLayoutMode::LayoutModeCenter: {
			NewY = TargetWindow->GetHeight() / 2 - Height / 2;

			break;
		}
		case VLayoutMode::LayoutModeFar: {
			NewY = TargetWindow->GetHeight() - Height;

			break;
		}
		case VLayoutMode::LayoutModePercent: {
			NewY = TargetWindow->GetHeight() * VerticalLayoutPercent;

			break;
		}
		}

		Parent()->Move(NewX, NewY);
	}

public:
	/*
	 * Build up Functional
	*/

	VLayout(VUIObject* Parent, VUIObject* TargetWidget) : VUIObject(Parent) {
		TargetWindow = TargetWidget;

		TargetWidget->SizeChanged.Connect(this, &VLayout::TargetWindowSizeChanged);
		Parent->SizeChanged.Connect(this, &VLayout::ParentSizeChanged);

		VerticalLayoutMode = VLayoutMode::LayoutModeCenter;
		HorizontalLayoutMode = VLayoutMode::LayoutModeCenter;
	}

	void SetVerticalLayoutMode(VLayoutMode Mode) {
		VerticalLayoutMode = Mode;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}
	void SetHorizontalLayoutMode(VLayoutMode Mode) {
		HorizontalLayoutMode = Mode;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}

	void SetVerticalLayoutPercent(double Percent) {
		VerticalLayoutPercent = Percent;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}
	void SetHorziontalLayoutPercent(double Percent) {
		HorziontalLayoutPercent = Percent;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}

	void SetRelativeX(int X) {
		RelativeX = X;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}
	void SetRelativeY(int Y) {
		RelativeY = Y;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}

	void SetXMiddleOffset(int XOffset) {
		XMiddleOffset = XOffset;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}
	void SetYMiddleOffset(int YOffset) {
		YMiddleOffset = YOffset;

		TargetWindowSizeChanged(TargetWindow->GetWidth(), TargetWindow->GetHeight());
	}

	int GetRelativeX() const {
		return RelativeX;
	}
	int GetRelativeY() const {
		return RelativeY;
	}
	int GetXMiddleOffset() const {
		return XMiddleOffset;
	}
	int GetYMiddleOffset() const {
		return YMiddleOffset;
	}
};

VLIB_END_NAMESPACE