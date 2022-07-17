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
	LayoutModeRelative, LayoutModeRelativeBottom, LayoutModeRelativeTop
};

/*
 * VLayout class:
 *	@description : Provide a Layout Helper
*/
class VLayout : public VUIObject {
private:
	VMainWindow* TargetWindow;

	VLayoutMode  VerticalLayoutMode;
	VLayoutMode  HorizontalLayoutMode;

	double       VerticalLayoutPercent = 0.f;
	double       HorziontalLayoutPercent = 0.f;

	int          RelativeX = 0;
	int          RelativeY = 0;

private:
	void TargetWindowSizeChanged(int Width, int Height) {
		int NewX = 0;
		int NewY = 0;

		switch (HorizontalLayoutMode) {
		case VLayoutMode::LayoutModeCenter: {
			NewX = Width / 2 - Parent()->GetWidth() / 2;

			break;
		}
		case VLayoutMode::LayoutModeFar: {
			NewX = Width - Parent()->GetWidth();

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
		}
		switch (VerticalLayoutMode) {
		case VLayoutMode::LayoutModeCenter: {
			NewY = Height / 2 - Parent()->GetHeight() / 2;

			break;
		}
		case VLayoutMode::LayoutModeFar: {
			NewY = Height - Parent()->GetHeight();

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

	VLayout(VUIObject* Parent, VMainWindow* TargetWidget) : VUIObject(Parent) {
		TargetWindow = TargetWidget;

		TargetWidget->SizeOnChange.Connect(this, &VLayout::TargetWindowSizeChanged);
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
};

VLIB_END_NAMESPACE