#include "../../../include/core/control/vviewlabel.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

	VScrollerHorizontal* VViewLabel::GetHorizontalScrollerInstance() {
		return HorizontalScroller;
	}
	VScrollerVertical* VViewLabel::GetVerticalScrollerInstance() {
		return VerticalScroller;
	}

	void VViewLabel::InitScroller(VUIObject* Parent) {
		HorizontalScroller = new VScrollerHorizontal(Parent);
		VerticalScroller = new VScrollerVertical(Parent);

		HorizontalScroller->ValueOnChange.Connect(this, &VViewLabel::ViewValueOnChange);
		VerticalScroller->ValueOnChange.Connect(this, &VViewLabel::ViewValueOnChange);
	}
	void VViewLabel::ResetScroller() {
		HorizontalScroller->Resize(GetWidth() - VerticalScroller->GetWidth(), 11);
		VerticalScroller->Resize(11, GetHeight() - HorizontalScroller->GetHeight());
		VerticalScroller->SetViewHeight(ViewArea.GetHeight());
		HorizontalScroller->SetViewWidth(ViewArea.GetWidth());

		if (VerticalAlign == VViewLabelVerticalAlign::Left) {
			VerticalScroller->Move(GetX(), GetY() + HorizontalScroller->GetHeight());
		}
		else {
			VerticalScroller->Move(GetX() + GetWidth() - VerticalScroller->GetWidth(), GetY() + HorizontalScroller->GetHeight());
		}

		if (HorizontalAlign == VViewLabelHorizontalAlign::Top) {
			HorizontalScroller->Move(GetX() + VerticalScroller->GetWidth(), GetY());
		}
		else {
			HorizontalScroller->Move(GetX() + VerticalScroller->GetWidth(), GetY() + GetHeight() - HorizontalScroller->GetHeight());
		}

		if (ViewArea.GetWidth() <= GetWidth()) {
			HorizontalScroller->Hide();
		}
		else {
			HorizontalScroller->Show();
		}
		if (ViewArea.GetHeight() <= GetHeight() - VerticalScroller->GetHeight()) {
			VerticalScroller->Hide();
		}
		else {
			VerticalScroller->Show();
		}
	}

	VRect VViewLabel::GetChildrenVisualRegion() {
		auto OffsetX = static_cast<int>(HorizontalScroller->GetValue() * (ViewArea.GetWidth() - GetWidth()));
		auto OffsetY = static_cast<int>(VerticalScroller->GetValue() * (ViewArea.GetHeight() - GetHeight()));

		return { OffsetX, OffsetY, OffsetX + GetWidth(), OffsetY + GetHeight() };
	}

	void VViewLabel::RestoreMousePosition(VPoint* MousePoint) {
		VUIObject::RestoreMousePosition(MousePoint);

		auto OffsetX = static_cast<int>(HorizontalScroller->GetValue() * (ViewArea.GetWidth() - GetWidth()));
		auto OffsetY = static_cast<int>(VerticalScroller->GetValue() * (ViewArea.GetHeight() - GetHeight()));

		MousePoint->X -= OffsetX;
		MousePoint->Y -= OffsetY;
	}

	void VViewLabel::SetVerticalAlign(const VViewLabelVerticalAlign& Align) {
		VerticalAlign = Align;

		ResetScroller();
	}
	void VViewLabel::SetHorizontalAlign(const VViewLabelHorizontalAlign& Align) {
		HorizontalAlign = Align;

		ResetScroller();
	}

	void VViewLabel::Move(const int& X, const int& Y) {
		VUIObject::Move(X, Y);

		ResetScroller();
	}
	void VViewLabel::Resize(const int& Width, const int& Height) {
		VUIObject::Resize(Width, Height);

		ResetScroller();
	}

	void VViewLabel::EditChildMessage(VMessage* Message) {
		auto OffsetX = static_cast<int>(HorizontalScroller->GetValue() * (ViewArea.GetWidth() - GetWidth()));
		auto OffsetY = static_cast<int>(VerticalScroller->GetValue() * (ViewArea.GetHeight() - GetHeight()));

		switch (Message->GetType()) {
		case VMessageType::CheckLocalFocusMessage: {
			static_cast<VCheckFocusMessage*>(Message)->FocusPoint.Offset(OffsetX, OffsetY);

			break;
		}
		case VMessageType::MouseMoveMessage: {
			static_cast<VMouseMoveMessage*>(Message)->MousePosition.Offset(OffsetX,
				OffsetY);

			break;
		}
		case VMessageType::MouseClickedMessage: {
			static_cast<VMouseClickedMessage*>(Message)->MousePosition.Offset(OffsetX, OffsetY);

			break;
		}
		default: {
			break;
		}
		}
	}

	VViewLabelTheme* VViewLabel::GetTheme() {
		return Theme;
	}

	void VViewLabel::Update(Core::VRect UpdateRect) {
		VUIObject::Update(GetRegion());
	}

	void VViewLabel::SetViewRegion(const VPoint& ViewRegion) {
		ViewArea = { 0, 0, ViewRegion.X, ViewRegion.Y };

		ResetScroller();
	}

	void VViewLabel::ViewValueOnChange(const double&) {
		Update(GetRegion());
	}

	bool VViewLabel::OnMessageTrigger(VRepaintMessage* RepaintMessage) {
		if (RepaintMessage->DirtyRectangle.Overlap(GetRegion()) &&
			(GetParent()->IsApplication() || GetParent()->GetChildrenVisualRegion().Overlap(GetRegion()))) {
			VRepaintMessage* ChildRepaintMessage = RepaintMessage;

			if (Canvas != nullptr) {
				delete Canvas;

				Canvas = nullptr;
			}

			if (ObjectVisual.Transparency != 0) {
				Canvas = new VCanvasPainter(ViewArea.GetWidth(), ViewArea.GetHeight(), CallWidgetGetRenderHandle());

				auto OffsetX = static_cast<int>(HorizontalScroller->GetValue() * (ViewArea.GetWidth() - GetWidth()));
				auto OffsetY = static_cast<int>(VerticalScroller->GetValue() * (ViewArea.GetHeight() - GetHeight()));

				Canvas->BeginDraw();
				Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
				Canvas->EndDraw();

				VSolidBrush BackgroundBrush(Theme->BackgroundColor, CallWidgetGetRenderHandle());
				VPenBrush BorderBrush(Theme->BorderColor, CallWidgetGetRenderHandle());

				Canvas->BeginDraw();

				Canvas->FillRoundedRectangle({ OffsetX, OffsetY, OffsetX + GetWidth(), OffsetY + GetHeight() }, Theme->Radius, &BorderBrush, &BackgroundBrush);

				Canvas->EndDraw();

				if (!IsWidget() && !IsApplication()) {
					ChildRepaintMessage = new VRepaintMessage(*RepaintMessage);

					ChildRepaintMessage->DirtyRectangle = *(GetRegion().Clone().MoveRV(0, 0));
				}

				Canvas->BeginDraw();
				SendMessageToChild(ChildRepaintMessage, false);
				Canvas->EndDraw();

				if (ChildRepaintMessage != RepaintMessage) {
					delete ChildRepaintMessage;
				}

				if (ObjectVisual.Shadow.EnableStatus) {
					int ShadowOffset = ObjectVisual.Shadow.Radius * 3;

					D2D1_POINT_2U OriginPoint = { 0, 0 };
					D2D1_RECT_U   CopyRect = { static_cast<unsigned int>(GetWidth()), static_cast<unsigned int>(GetHeight()) };

					ID2D1Bitmap* CanvasSurface;

					Canvas->GetDXObject()->GetBitmap(&CanvasSurface);

					VImage ShadowImage(CanvasSurface);
					ShadowImage.ApplyShadowEffect(ObjectVisual.Shadow.Radius, ObjectVisual.Shadow.Color, CallWidgetGetRenderHandle());

					GetParentCanvas()->DrawImage({ static_cast<int>(GetX() - ShadowOffset),
												   static_cast<int>(GetY() - ShadowOffset),
												   static_cast<int>(GetX() - ShadowOffset + ShadowImage.GetWidth()),
												   static_cast<int>(GetY() - ShadowOffset + ShadowImage.GetHeight()) }, &ShadowImage,
						{ 0, 0, ShadowImage.GetWidth(), ShadowImage.GetHeight() }, ObjectVisual.Transparency);
				}

				EditCanvas(Canvas);

				GetParentCanvas()->DrawCanvas({ GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight() }, Canvas, {
						OffsetX, OffsetY, OffsetX + GetWidth(), OffsetY + GetHeight()
					}, ObjectVisual.Transparency);

				delete Canvas;

				Canvas = nullptr;
			}

			return true;
		}

		return false;
	}

	void VViewLabel::OnMessage(VMessage* Message) {
		if (Message->GetType() == VMessageType::MouseWheelMessage && TieWithMouseWheel) {
			VMouseWheelMessage* MouseWheel = new VMouseWheelMessage(*static_cast<VMouseWheelMessage*>(Message));
			MouseWheel->MousePosition = { VerticalScroller->GetX(), VerticalScroller->GetY() };

			VerticalScroller->OnMessage(MouseWheel);

			delete MouseWheel;
		}
	}

	void VViewLabel::SetTieWithWheel(const bool& TieStatus) {
		TieWithMouseWheel = TieStatus;
	}

	VViewLabel::VViewLabel(Core::VUIObject* Parent) : VUIObject(Parent) {
		Theme = new VViewLabelTheme(*(static_cast<VViewLabelTheme*>(GetTargetTheme(VUIThemeType::VViewLabel))));

		InitScroller(Parent);

		ResetScroller();
	}
	VViewLabel::VViewLabel(const int& Width, const int& Height, VUIObject* Parent) : VUIObject(Parent) {
		Theme = new VViewLabelTheme(*(static_cast<VViewLabelTheme*>(GetTargetTheme(VUIThemeType::VViewLabel))));

		InitScroller(Parent);

		Resize(Width, Height);

		ResetScroller();
	}
	VViewLabel::VViewLabel(const int& Width, const int& Height, const int& ViewWidth, const int& ViewHeight, VUIObject* Parent)
		: VUIObject(Parent) {
		Theme = new VViewLabelTheme(*(static_cast<VViewLabelTheme*>(GetTargetTheme(VUIThemeType::VViewLabel))));

		InitScroller(Parent);

		Resize(Width, Height);

		ViewArea = { 0, 0, ViewWidth, ViewHeight };

		ResetScroller();
	}
	VViewLabel::~VViewLabel() {
		delete Theme;
	}
}

VLIB_END_NAMESPACE