#pragma once

#include "vabstractbutton.h"
#include "../uibasic/vtimer.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

	class VIconButton : public VAbstractButton {
	protected:
		VIconButtonTheme* Theme;

	private:
		VBasicTimer             AnimationFrameTimer;
		VAnimationInterpolator* Interpolator;

		VLabelStatusTheme       OldTheme;
		VLabelStatusTheme       TargetTheme;

		bool                    InAnimation = false;

	public:
		VRect GetRegion() override;

	public:
		void LeftClickedDown() override;
		void LeftClickedUp() override;
		void GotMouseFocus() override;
		void LostMouseFocus() override;

	public:
		explicit VIconButton(VUIObject* Parent);
		VIconButton(const int& Width, const int& Height, VUIObject* Parent);
		VIconButton(const int& Width, const int& Height, VImage* Image, VUIObject* Parent);
		~VIconButton() noexcept;

	public:
		void SetIconImage(VImage* IconImage);

	public:
		VIconButtonTheme* GetTheme();
		void SetTheme(VIconButtonTheme* Theme);

	public:
		void OnPaint(VCanvasPainter* Painter) override;
		void CheckFrame() override;
	};

}

VLIB_END_NAMESPACE