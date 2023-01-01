#include "../../../include/core/object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
	VUIObjectKernel::VUIObjectKernel() {
		Parent = nullptr;
		ParentID = 0;

		GlobalID = L"[VObject]";
	}
	VUIObject::VUIObject(VUIObject* Parent) {
		Canvas = nullptr;

		SetParent(Parent);
	}
	VUIObjectShadowProperty::VUIObjectShadowProperty() {
		Color = VColor(0.f, 0.f, 0.f, 1.f);
		Offset = { 0, 0 };

		Radius = 2;

		EnableStatus = false;
	}
	VUIObjectSurface::VUIObjectSurface() {
		Stats = VUIObjectUIStats::Normal;
		Transparency = 1.f;
	}
	VUIObject::~VUIObject() {
		SetParent(nullptr);

		for (auto& ChildObject : ObjectKernel.ChildObjectContainer) {
			ChildObject->~VUIObject();
		}

		ObjectKernel.ChildObjectContainer.clear();
	}
	bool VUIObject::IsAnimation() {
		return false;
	}
	bool VUIObject::IsApplication() {
		return false;
	}
	bool VUIObject::IsWidget() const {
		return false;
	}
	VUIObject* VUIObject::GetChildObjectByPosition(const int& Position) {
		return ObjectKernel.ChildObjectContainer[Position];
	}
	std::vector<VBasicUITheme*> VUIObject::GetApplicationTheme() {
		if (GetParent() != nullptr) {
			return GetParent()->GetApplicationTheme();
		}
		else {
			VLIB_REPORT_ERROR(L"Get theme list from application failed!");

			return { nullptr };
		}
	}
	VBasicUITheme* VUIObject::GetTargetTheme(VUIThemeType ThemeType) {
		auto ThemeList = GetApplicationTheme();

		for (auto& Theme : ThemeList) {
			if (Theme->GetThemeType() == ThemeType) {
				return Theme;
			}
		}

		return nullptr;
	}
	HWND VUIObject::GetLocalWinId() {
		if (GetParent() != nullptr) {
			return GetParent()->GetLocalWinId();
		}
		else {
			VLIB_REPORT_ERROR(L"Get local window's HWND from widget failed!");

			return NULL;
		}
	}
	VUIObject* VUIObject::GetParent() const {
		return ObjectKernel.Parent;
	}

	VRect VUIObject::GetRegion() {
		return ObjectVisual.Rectangle;
	}
	VRect VUIObject::GetChildrenVisualRegion() {
		return { 0, 0, GetWidth(), GetHeight() };
	}
	VCanvasPainter* VUIObject::GetParentCanvas() {
		return GetParent()->Canvas;
	}
	VKits::VAllocator* VUIObject::GetWidgetAllocator() const {
		if (GetParent() != nullptr) {
			return GetParent()->GetWidgetAllocator();
		}
	}
	VCanvasPainter* VUIObject::GetWidgetCanvas() {
		if (IsWidget() == false) {
			return GetParent()->GetWidgetCanvas();
		}

		return Canvas;
	}
	int VUIObject::GetOriginX(const int& X) {
		if (IsWidget() == false) {
			return GetParent()->GetOriginX(GetX() + X);
		}

		return X;
	}
	int VUIObject::GetOriginY(const int& Y) {
		if (IsWidget() == false) {
			return GetParent()->GetOriginX(GetY() + Y);
		}

		return Y;
	}
	float VUIObject::GetTransparency() const {
		return ObjectVisual.Transparency;
	}
	VRect VUIObject::HelperGetSourceRect() {
		return VRect{ 0, 0, ObjectVisual.Rectangle.GetWidth(), ObjectVisual.Rectangle.GetHeight() };
	}
	std::wstring VUIObject::GeneratedGlobalID(std::wstring TargetID) {
		if (!GetParent()->IsApplication()) {
			TargetID += std::to_wstring(ObjectKernel.ParentID);

			return GetParent()->GeneratedGlobalID(TargetID);
		}

		return TargetID;
	}
	void VUIObject::CallWidgetSetIME(const int& X, const int& Y) {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetSetIME(X, Y);
		}
	}
	void VUIObject::CallWidgetSetFocusID(const std::wstring& ObjectID) {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetSetFocusID(ObjectID);
		}
	}
	void VUIObject::RestoreMousePosition(VPoint* MousePosition) {
		MousePosition->X += GetX();
		MousePosition->Y += GetY();
	}
	std::wstring VUIObject::CallWidgetGetFocusID() const {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetGetFocusID();
		}

		return L"";
	}
	void VUIObject::CallWidgetLockFocusID() {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetLockFocusID();
		}
	}
	void VUIObject::CallWidgetUnlockFocusID() {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetUnlockFocusID();
		}
	}
	bool VUIObject::CallWidgetGetLockingStatus() {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetGetLockingStatus();
		}

		return false;
	}
	void VUIObject::CallWidgetSendMessage(VMessage* Message) {
		if (GetParent() != nullptr) {
			switch (Message->GetType()) {
			case VMessageType::CheckLocalFocusMessage: {
				GetParent()->RestoreMousePosition(&static_cast<VCheckFocusMessage*>(Message)->FocusPoint);

				break;
			}
			case VMessageType::MouseMoveMessage: {
				GetParent()->RestoreMousePosition(&static_cast<VMouseMoveMessage*>(Message)->MousePosition);

				break;
			}
			case VMessageType::MouseClickedMessage: {
				GetParent()->RestoreMousePosition(&static_cast<VMouseClickedMessage*>(Message)->MousePosition);

				break;
			}
			default: {
				break;
			}
			}

			return GetParent()->CallWidgetSendMessage(Message);
		}
	}
	HWND VUIObject::CallWidgetGetHWND() {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetGetHWND();
		}

		return NULL;
	}

	void VUIObject::CallWidgetSetIMEFontStyle(const LOGFONT& LogfontStyle) {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetSetIMEFontStyle(LogfontStyle);
		}

		return;
	}
	LOGFONT VUIObject::GetWidgetIMEFontStlye() const {
		if (GetParent() != nullptr) {
			return GetParent()->GetWidgetIMEFontStlye();
		}

		return LOGFONT();
	}

	Core::VRenderHandle VUIObject::CallWidgetGetRenderHandle()  const {
		VRenderHandle RenderHandle;

		if (IsWidget() == true) {
			RenderHandle._IRenderTarget = Canvas->GetDXObject();
		}
		else {
			RenderHandle._IRenderTarget = GetParent()->CallWidgetGetRenderHandle()._IRenderTarget;
		}

		RenderHandle._IDWriteFactory = VDirectXWriteFactory.GetInstance();

		RenderHandle.Allocator	    = GetWidgetAllocator();

		return RenderHandle;
	}
	Core::VRenderHandle VUIObject::CallWidgetGetDCRenderTarget() const {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetGetDCRenderTarget();
		}

		return NULL;
	}
	Core::VCanvasPainter* VUIObject::CallWidgetGetCanvas() {
		if (GetParent() != nullptr) {
			return GetParent()->CallWidgetGetCanvas();
		}

		return nullptr;
	}
	void VUIObject::Update(VRect UpdateRect) {
		if (ObjectVisual.Shadow.EnableStatus) {
			auto ShadowOffset = ObjectVisual.Shadow.Radius * 3;
			UpdateRect.Extended(ShadowOffset, ShadowOffset, ShadowOffset, ShadowOffset);
		}

		if (GetParent() != nullptr) {
			return GetParent()->Update(UpdateRect);
		}
	}
	void VUIObject::Update() {
		Update(GetRegion());
	}
	void VUIObject::CheckAllFrame(bool RecursionChildren) {
		CheckFrame();

		if (!RecursionChildren) {
			for (auto& Object : ObjectKernel.ChildObjectContainer) {
				Object->CheckFrame();
			}
		}
		else {
			for (auto& Object : ObjectKernel.ChildObjectContainer) {
				Object->CheckAllFrame(RecursionChildren);
			}
		}
	}
	int VUIObject::GetX() const {
		return ObjectVisual.Rectangle.Left;
	}
	int VUIObject::GetY() const {
		return ObjectVisual.Rectangle.Top;
	}
	int VUIObject::GetWidth() const {
		return ObjectVisual.Rectangle.GetWidth();
	}
	int VUIObject::GetHeight() const {
		return ObjectVisual.Rectangle.GetHeight();
	}
	void VUIObject::Resize(const int& Width, const int& Height) {
		auto OldRect = ObjectVisual.Rectangle.Clone();

		ObjectVisual.Rectangle.Right = ObjectVisual.Rectangle.Left + Width;
		ObjectVisual.Rectangle.Bottom = ObjectVisual.Rectangle.Top + Height;

		OldRect.FusionRect(ObjectVisual.Rectangle);

		Update(OldRect);

		Resized.Emit(Width, Height);
	}
	void VUIObject::Move(const int& X, const int& Y) {
		auto OldRect = ObjectVisual.Rectangle.Clone();
		auto FlushRect = GetRegion().Clone();

		auto Width = GetWidth();
		auto Height = GetHeight();

		ObjectVisual.Rectangle.Left = X;
		ObjectVisual.Rectangle.Top = Y;
		ObjectVisual.Rectangle.Right = X + Width;
		ObjectVisual.Rectangle.Bottom = Y + Height;

		OldRect.Extended(1, 1, 1, 1);

		Update(GetRegion());
		Update(FlushRect);

		Moved.Emit(X, Y);
	}
	void VUIObject::Resize(const Core::VGeometry& Size) {
		Resize(Size.Width, Size.Height);
	}
	void VUIObject::Move(const VPoint& Point) {
		Move(Point.X, Point.Y);
	}
	void VUIObject::SetParent(VUIObject* Parent) {
		if (ObjectKernel.Parent != nullptr) {
			for (auto Object = GetParent()->ObjectKernel.ChildObjectContainer.begin() + (ObjectKernel.ParentID + 1);
				Object != GetParent()->ObjectKernel.ChildObjectContainer.end(); ++Object) {
				--(Object.operator*()->ObjectKernel.ParentID);
			}

			GetParent()->ObjectKernel.ChildObjectContainer.erase(GetParent()->ObjectKernel.ChildObjectContainer.begin()
				+ ObjectKernel.ParentID);
		}

		ObjectKernel.Parent = Parent;

		if (Parent != nullptr) {
			GetParent()->ObjectKernel.ChildObjectContainer.push_back(this);

			ObjectKernel.ParentID = GetParent()->ObjectKernel.ChildObjectContainer.size() - 1;
			ObjectKernel.GlobalID = GeneratedGlobalID();
		}

		Update();
	}
	void VUIObject::SetTransparency(const float& Transparency) {
		ObjectVisual.Transparency = Transparency;

		Update();
	}
	bool VUIObject::CheckUIFocusStatus(const VPoint& MousePosition, VMessage* SourceMessage) {
		if (ObjectVisual.Stats == Hidden) {
			return false;
		}

		if ((IsAnimation() || !GetWidth() || !GetHeight()) ||
			(CallWidgetGetLockingStatus() && CallWidgetGetFocusID() != ObjectKernel.GlobalID)) {
			return false;
		}

		if (MousePosition.InsideRectangle(GetRegion()) && CheckMousePosition(MousePosition)) {
			if (SourceMessage->GetType() == VMessageType::CheckLocalFocusMessage) {
				if (CallWidgetGetFocusID() != ObjectKernel.GlobalID) {
					ObjectVisual.Stats = VUIObjectUIStats::Normal;

					Update();
					LostMouseFocus();

					LostFocus.Emit();

					return false;
				}

				return false;
			}
			if (SourceMessage->GetType() == VMessageType::MouseClickedMessage) {
				auto MouseClickedMessage = static_cast<VMouseClickedMessage*>(SourceMessage);

				switch (MouseClickedMessage->ClickedKey) {
				case VMouseKeyFlag::Right: {
					MouseRightClicked(MouseClickedMessage->ClickedMethod);

					break;
				}
				case VMouseKeyFlag::Left: {
					MouseLeftClicked(MouseClickedMessage->ClickedMethod);

					break;
				}
				case VMouseKeyFlag::Middle: {
					MouseMiddleClicked(MouseClickedMessage->ClickedMethod);

					break;
				}
				}
			}

			if (ObjectVisual.Stats != VUIObjectUIStats::OnFocus && ObjectVisual.Stats != Hidden) {
				ObjectVisual.Stats = VUIObjectUIStats::OnFocus;

				Update();

				CallWidgetSetFocusID(ObjectKernel.GlobalID);

				GotMouseFocus();

				InFocus.Emit();

				VCheckFocusMessage* CheckFocus = new VCheckFocusMessage(CallWidgetGetHWND(), MousePosition);

				CallWidgetSendMessage(CheckFocus);
				delete CheckFocus;
			}

			return true;
		}
		else if (ObjectVisual.Stats == VUIObjectUIStats::OnFocus) {
			ObjectVisual.Stats = VUIObjectUIStats::Normal;

			Update();

			CallWidgetSetFocusID(L"");

			LostMouseFocus();
			LostFocus.Emit();

			return false;
		}

		return false;
	}
	void VUIObject::SetShadowStats(const bool& Stats) {
		ObjectVisual.Shadow.EnableStatus = Stats;

		Update();
	}
	void VUIObject::SetShadowColor(const VColor& Color) {
		ObjectVisual.Shadow.Color = Color;

		Update();
	}
	void VUIObject::SetShadowRadius(const float& Radius) {
		ObjectVisual.Shadow.Radius = Radius;

		Update();
	}
	bool VUIObject::OnMessageTrigger(Core::VRepaintMessage* RepaintMessage) {
		if (RepaintMessage->DirtyRectangle.Overlap(GetRegion()) &&
			(GetParent()->IsApplication() || GetParent()->GetChildrenVisualRegion().Overlap(GetRegion()))) {
			VRepaintMessage* ChildRepaintMessage = RepaintMessage;

			if (Canvas != nullptr) {
				delete Canvas;

				Canvas = nullptr;
			}

			if (ObjectVisual.Transparency != 0) {
				Canvas = new VCanvasPainter(GetRegion().GetWidth(), GetRegion().GetHeight(), CallWidgetGetRenderHandle());
				Canvas->BeginDraw();
				Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
				Canvas->EndDraw();

				OnPaint(Canvas);

				if (!IsWidget() && !IsApplication()) {
					ChildRepaintMessage = new VRepaintMessage(*RepaintMessage);

					ChildRepaintMessage->DirtyRectangle = *(GetRegion().Clone().MoveRV(0, 0));
				}

				Canvas->BeginDraw();
				SendMessageToChild(ChildRepaintMessage, false);
				Canvas->EndDraw();

				if (ObjectVisual.Shadow.EnableStatus) {
					D2D1_POINT_2U OriginPoint = { 0, 0 };
					D2D1_RECT_U   CopyRect = { static_cast<unsigned int>(GetWidth()), static_cast<unsigned int>(GetHeight()) };

					Microsoft::WRL::ComPtr<ID2D1Bitmap> CanvasSurface;

					Canvas->GetDXObject()->GetBitmap(&CanvasSurface);

					VImage ShadowImage((ID2D1Bitmap1*)CanvasSurface.Get());
					ShadowImage.ApplyShadowEffect(ObjectVisual.Shadow.Radius, ObjectVisual.Shadow.Color, CallWidgetGetRenderHandle(), &ObjectVisual.Shadow.Offset);

					GetParentCanvas()->DrawImage({ static_cast<int>(GetX() + ObjectVisual.Shadow.Offset.X),
												   static_cast<int>(GetY() + ObjectVisual.Shadow.Offset.Y),
												   static_cast<int>(GetX() + ObjectVisual.Shadow.Offset.X + ShadowImage.GetWidth()),
												   static_cast<int>(GetY() + ObjectVisual.Shadow.Offset.Y + ShadowImage.GetHeight()) }, &ShadowImage,
						{ 0, 0, ShadowImage.GetWidth(), ShadowImage.GetHeight() }, ObjectVisual.Transparency);

					CanvasSurface.ReleaseAndGetAddressOf();
				}

				if (ChildRepaintMessage != RepaintMessage) {
					delete ChildRepaintMessage;
				}

				EditCanvas(Canvas);

				GetParentCanvas()->DrawCanvas(GetRegion(), Canvas, { 0, 0, GetRegion().GetWidth(), GetRegion().GetHeight() }, ObjectVisual.Transparency);

				delete Canvas;
				Canvas = nullptr;
			}

			return true;
		}

		return false;
	}
	bool VUIObject::SysProcessMessage(Core::VMessage* Message) {
		if (ObjectVisual.Stats == VUIObjectUIStats::Hidden) {
			return false;
		}

		if (IsApplication()) {
			if (Message->GetType() != VMessageType::QuitWindowMessage) {
				return SendMessageToChild(Message, false);
			}
			else {
				return SendMessageToChild(Message, true);
			}
		}

		if (Message->MessageTriggerWidget != CallWidgetGetHWND()) {
			if (Message->GetType() != VMessageType::QuitWindowMessage) {
				return SendMessageToChild(Message, false);
			}
			else {
				return SendMessageToChild(Message, true);
			}
		}

		if (ObjectVisual.Stats == VUIObjectUIStats::Hidden) {
			return false;
		}

		OnMessage(Message);

		switch (Message->GetType()) {
		case VMessageType::QuitWindowMessage: {
			return CheckQuitWindowMessage(Message);
		}
		case VMessageType::GetRepaintAeraMessage: {
			VGetRepaintAeraMessage* RepaintMessage = static_cast<VGetRepaintAeraMessage*>(Message);
			if (*(RepaintMessage->RepaintAera) != GetRegion() &&
				RepaintMessage->RepaintAera->Overlap(GetRegion())) {
				(*(RepaintMessage->RepaintAera)).FusionRect(GetRegion());

				return true;
			}

			return false;
		}
		case VMessageType::MouseMoveMessage: {
			return CheckElementUIStatus(Message);
		}
		case VMessageType::MouseClickedMessage: {
			return CheckElementUIStatus(Message);
		}
		case VMessageType::IMECharMessage: {
			auto IMEMessage = static_cast<VIMECharMessage*>(Message);

			if (!IMECharInputed(IMEMessage->IMEChar)) {
				SendMessageToChild(IMEMessage);
			}

			break;
		}
		case VMessageType::RepaintMessage: {
			auto RepaintMessage = static_cast<VRepaintMessage*>(Message);

			return OnMessageTrigger(RepaintMessage);
		}
		case VMessageType::MouseWheelMessage: {
			auto WheelMessage = static_cast<VMouseWheelMessage*>(Message);

			MouseMiddleDragged(WheelMessage->WheelValue);

			return SendMessageToChild(WheelMessage, false);
		}
		case VMessageType::KeyClickedMessage: {
			auto KeyMessage = static_cast<VKeyClickedMessage*>(Message);

			if (!CheckDown(KeyMessage)) {
				SendMessageToChild(KeyMessage);
			}

			break;
		}
		case VMessageType::FreeResourceMessage: {
			if (Canvas != nullptr) {
				delete Canvas;

				Canvas = nullptr;
			}

			return true;
		}
		case VMessageType::CheckLocalFocusMessage: {
			auto FocusMessage = static_cast<VCheckFocusMessage*>(Message);

			CheckUIFocusStatus(FocusMessage->FocusPoint, FocusMessage);

			SendMessageToChild(Message, false);

			return false;
		}
		case VMessageType::KillFocusMessage: {
			if (CallWidgetGetFocusID() == ObjectKernel.GlobalID) {
				LostMouseFocus();
				LostFocus.Emit();
			}

			SendMessageToChild(Message, false);

			return true;
		}
		default: {
			return false;
		}
		}

		return false;
	}
	bool VUIObject::SendMessageToChild(Core::VMessage* Message, bool BreakWhenMeetTrue) {
		EditChildMessage(Message);

		VMessage* FocusMessage = Message;

		switch (Message->GetType()) {
		case VMessageType::CheckLocalFocusMessage: {
			FocusMessage = new VCheckFocusMessage(*(static_cast<VCheckFocusMessage*>(Message)));
				static_cast<VCheckFocusMessage*>(FocusMessage)->FocusPoint.Offset(-GetRegion().Left, -GetRegion().Top);

			break;
		}
		case VMessageType::MouseMoveMessage: {
			FocusMessage = new VMouseMoveMessage(*(static_cast<VMouseMoveMessage*>(Message)));
				static_cast<VMouseMoveMessage*>(FocusMessage)->MousePosition.Offset(-GetRegion().Left, -GetRegion().Top);

			break;
		}
		case VMessageType::MouseClickedMessage: {
			FocusMessage = new VMouseClickedMessage(*(static_cast<VMouseClickedMessage*>(Message)));
				static_cast<VMouseClickedMessage*>(FocusMessage)->MousePosition.Offset(-GetRegion().Left, -GetRegion().Top);

			break;
		}
		default: {
			break;
		}
		}

		bool Flag = false;

		if (Message->GetType() != VMessageType::RepaintMessage) {
			for (auto Object = ObjectKernel.ChildObjectContainer.rbegin();
				 Object != ObjectKernel.ChildObjectContainer.rend(); ++Object) {
				if (Object.operator*()->SysProcessMessage(FocusMessage)) {
					if (BreakWhenMeetTrue) {
						if (FocusMessage != Message) {
							delete FocusMessage;
						}

						return true;
					}
					else {
						Flag = true;
					}
				}
			}
		}
		else {
			for (auto Object = ObjectKernel.ChildObjectContainer.begin();
				 Object != ObjectKernel.ChildObjectContainer.end(); ++Object) {
				if (Object.operator*()->SysProcessMessage(FocusMessage)) {
					if (BreakWhenMeetTrue) {
						if (FocusMessage != Message) {
							delete FocusMessage;
						}

						return true;
					}
					else {
						Flag = true;
					}
				}
			}
		}

		if (FocusMessage != Message) {
			delete FocusMessage;
		}

		return Flag;
	}
	bool VUIObject::CheckElementUIStatus(VMessage* SourceMessage) {
		if (ObjectVisual.Stats == Hidden) {
			return false;
		}

		VPoint MousePosition;

		switch (SourceMessage->GetType()) {
		case VMessageType::MouseMoveMessage: {
			MousePosition = static_cast<VMouseMoveMessage*>(SourceMessage)->MousePosition;

			break;
		}
		case VMessageType::MouseClickedMessage: {
			MousePosition = static_cast<VMouseClickedMessage*>(SourceMessage)->MousePosition;

			break;
		}

		default: {
			return false;
		}
		}

		if (SendMessageToChild(SourceMessage, true)) {
			if (!CheckMousePosition(MousePosition) && MousePosition.InsideRectangle(GetRegion())) {
				VKillFocusMessage KillFocus(CallWidgetGetHWND());

				SendMessageToChild(&KillFocus, true);
			}

			return true;
		}
		else {
			auto Result = CheckUIFocusStatus(MousePosition, SourceMessage);

			return Result;
		}

		return false;
	}
	const std::vector<VUIObject* > VUIObject::GetChildLayout() {
		return ObjectKernel.ChildObjectContainer;
	}
	VCanvasPainter* VUIObject::GetCanvas() {
		return Canvas;
	}
	void VUIObject::Hide() {
		ObjectVisual.Stats = VUIObjectUIStats::Hidden;

		Update();
	}
	void VUIObject::Show() {
		ObjectVisual.Stats = VUIObjectUIStats::Normal;

		Update();
	}

}

VLIB_END_NAMESPACE