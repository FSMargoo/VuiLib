﻿/*
 * VUIObject.hpp
 *	@description : VLib's UI Object
 *	@birth		 : 2022/6.4
*/

#pragma once

#include "../../../render/vrender/vpainter.hpp"
#include "../../../basic/vbasic/vmessage.hpp"
#include "../../../basic/vbasic/vsignal.hpp"

#include "vtheme.hpp"

VLIB_BEGIN_NAMESPACE

#include <vector>

/* Pre-Declared VUIObject FOr VUIObjectKernel */
class VUIObject;

/*
 * VUIObjectKernel class:
 *	@description  : This Class Describe a Object Kernel Attribute
*/
struct VUIObjectKernel {
	/* Object's Parent */
	VUIObject* Parent = nullptr;

	/* Object's Parent ID ( Vector Position ) */
	size_t	     ParentID = 0;
	/* Object's Global ID */
	std::wstring GlobalID = L"VO";

	/* Object's Child Object */
	std::vector<VUIObject* > ChildObjectContainer;
};

/*
 * VUIObjectUIStats
*/
enum VUIObjectUIStats {
	Hidden, OnClicked, OnFocus, Normal
};

/*
 * VUIObjectSurface class:
 *	@description  : This Class Desribe A Object's Surface Stats
*/
struct VUIObjectSurface {
	VRect			 Rect;

	int              Transparency = 255;

	VUIObjectUIStats UIStats = VUIObjectUIStats::Normal;
};

/*
 * VUIObject class :
 *	@description  : The Basic UI Object In VLib
*/
class VUIObject {
protected:
	/*
	 * GetThemeListFromParent virtual Functional:
	 *	@description  : Get the Theme List From Application Parent
	*/
	virtual std::vector<VTheme*> GetThemeListFromParent() {
		if (VUnlikely(Parent() != nullptr)) {
			return Parent()->GetThemeListFromParent();
		}
		else {
			VAssertBox(L"Parent() Should Not Be Nullptr!");

			return std::vector<VTheme*>(0);
		}
	}
	/*
	 * GetRegoin Functional:
	 *	@description  : Get the Regoin of Object
	*/
	VRect GetRegoin() {
		return SurfaceRegion();
	}

protected:
	/*
	 * GetParentWindowHandle Functional:
	 *	@description : Get the Parent Window's Handle
	*/
	virtual HWND GetParentWindowHandle() {
		if (Parent() != nullptr) {
			return Parent()->GetParentWindowHandle();
		}

		return NULL;
	}

public:
	virtual bool IsAnimation() {
		return false;
	}

public:
	/*
	 * ObjectCanvas Variable:
	 *	@description  : Provide a Canvas For Object & Children Object to Paint Control
	*/
	VCanvas* ObjectCanvas = nullptr;

	/*
	 * GetParentCanvas Functional:
	 *	@description  : Get Parent Canvas
	*/
	VCanvas* GetParentCanvas() {
		return Parent()->ObjectCanvas;
	}

	/*
	 * SearchThemeFromParent Functional:
	 *	@description  : Search Theme From Parent
	*/
	VTheme* SearchThemeFromParent(int ThemeID) {
		auto ThemeList = GetThemeListFromParent();

		for (auto& ThemeObject : ThemeList) {
			if (ThemeObject->GetThemeType() == ThemeID) {
				return ThemeObject;
			}
		}

		return nullptr;
	}

public:
	VSignal<>         InFocus;
	VSignal<>         LosedFocus;
	VSignal<int, int> SizeChanged;
	VSignal<int, int> PositionChanged;

private:
	/*
	 * Generated Functional:
	 *	@description  : This Function Generated A Global ID For Local Object
	*/
	std::wstring GeneratedGlobalID(std::wstring LocalID = L"") {
		if (ObjectKernel->Parent->IsApplication() == false) {
			LocalID += std::to_wstring(ObjectKernel->ParentID);

			return ObjectKernel->Parent->GeneratedGlobalID(LocalID);
		}

		return LocalID;
	}

protected:
	/*
	 * SetGlobalIMEPosition virtual Functional:
	 *	@description  : Call the Widget to Set Global IME Position
	*/
	virtual void SetGlobalIMEPosition(int X, int Y) {
		if (Parent() != nullptr) {
			return Parent()->SetGlobalIMEPosition(X, Y);
		}
	}

protected:
	/*
	 * SetGlobalFocusID Functional:
	 *	@description  : Set the Global Focus Object's ID
	*/
	virtual void        SetGlobalFocusID(std::wstring ObjectID) {
		if (Parent() != nullptr) {
			Parent()->SetGlobalFocusID(ObjectID);
		}
	}
	/*
	 * GetGlobalFocusID Functional:
	 *	@description  : Get the Global Focus Object's ID
	 *	@return value : ID
	*/
	virtual std::wstring GetGlobalFocusID() {
		if (Parent() != nullptr) {
			return Parent()->GetGlobalFocusID();
		}

		return L"";
	}
	/*
	 * LockGlobalFocusID Functional:
	 *	@description  : Lock the Global Focus Object
	*/
	virtual void LockGlobalFocusID(std::wstring ID) {
		if (Parent() != nullptr) {
			return Parent()->LockGlobalFocusID(ID);
		}
	}
	/*
	 * UnlockGlobalFocusID Functional:
	 *	@description  : Unlock the Global Focus Object
	*/
	virtual void UnlockGlobalFocusID() {
		if (Parent() != nullptr) {
			return Parent()->UnlockGlobalFocusID();
		}
	}
	/*
	 * IsGlobalIDLocking Functional:
	 *	@description  : Get the Lock Stats
	*/
	virtual bool IsGlobalIDLocking() {
		if (Parent() != nullptr) {
			return Parent()->IsGlobalIDLocking();
		}

		return false;
	}

protected:
	/*
	 * EditChildMessage virtual Fucntional:
	 *	@description  : Provide A API to Edit The Message Which Sends To Child Objects
	*/
	virtual void EditChildMessage(VMessage* Message) {  /* Empty */ }

protected:
	/*
	 * Kernel Functional:
	 *	@description  : Get the Kernel Object
	*/
	VUIObjectKernel* Kernel() {
		return ObjectKernel;
	}
	/*
	 * Parent Functional:
	 *	@description  : Get the Parent Object
	*/
	VUIObject* Parent() {
		return ObjectKernel->Parent;
	}
	/* Get UI Surface */
	VUIObjectSurface* Surface() {
		return ObjectSurface;
	}
	/*
	 * SendMessageToChild Functional:
	 *	@description  : Send Target Message For Each Children Object
	 *	@return value : Children Take Over Message Or Not
	*/
	bool              SendMessageToChild(VBasicMessage* Message, bool BreakWhenMeetTrue = true) {
		EditChildMessage(Message);

		VMessage* FocusMessage = Message;

		switch (Message->GetType()) {
		case VMessageType::CheckLocalFocusMessage: {
			FocusMessage = new VCheckFocusMessage(*(static_cast<VCheckFocusMessage*>(Message)));
			static_cast<VCheckFocusMessage*>(FocusMessage)->FocusPoint.Offset(-SurfaceRegion().left, -SurfaceRegion().top);

			break;
		}
		case VMessageType::MouseMoveMessage: {
			FocusMessage = new VMouseMoveMessage(*(static_cast<VMouseMoveMessage*>(Message)));
			static_cast<VMouseMoveMessage*>(FocusMessage)->MousePosition.Offset(-SurfaceRegion().left, -SurfaceRegion().top);

			break;
		}
		case VMessageType::MouseClickedMessage: {
			FocusMessage = new VMouseClickedMessage(*(static_cast<VMouseClickedMessage*>(Message)));
			static_cast<VMouseClickedMessage*>(FocusMessage)->MousePosition.Offset(-SurfaceRegion().left, -SurfaceRegion().top);

			break;
		}
		}

		bool Flag = false;

		if (Message->GetType() != VMessageType::RepaintMessage) {
			for (auto ChildObject = Kernel()->ChildObjectContainer.rbegin();
				ChildObject != Kernel()->ChildObjectContainer.rend(); ++ChildObject) {
				if (ChildObject.operator*()->SysDealyMessage(FocusMessage) == true) {
					if (BreakWhenMeetTrue == true) {
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
			for (auto ChildObject = Kernel()->ChildObjectContainer.begin();
				ChildObject != Kernel()->ChildObjectContainer.end(); ++ChildObject) {
				if (ChildObject.operator*()->SysDealyMessage(FocusMessage) == true) {
					if (BreakWhenMeetTrue == true) {
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

	/*
	 * SendMessageGlobalWidget virtual Functional:
	 *	@description  : Send The Global Widget Message
	*/
	virtual void       SendMessageToGlobalWidget(VMessage* Message) {
		if (Parent() != nullptr) {
			Parent()->SendMessageToGlobalWidget(Message);
		}
	}

protected:
	/*
	 * GetSourceRect Functional:
	 *	@description : Get the Source Rect
	*/
	VRect GetSourceRect() {
		return VRect(0, 0, GetWidth(), GetHeight());
	}

private:
	/*
	 * SurfaceRect Functional:
	 *	@description  : Get the Object's Surface Rect
	*/
	VRect SurfaceRect() {
		return Surface()->Rect;
	}
	/*
	 * SurfaceRect Virtual Functional:
	 *	@description  : Get the Object's Surface Regoin
	*/
	virtual VRect SurfaceRegion() {
		return Surface()->Rect;
	}

public:
	VUIObject* GetParent() {
		return ObjectKernel->Parent;
	}

public:
	void Show() {
		if (Surface()->UIStats == VUIObjectUIStats::Hidden) {
			Surface()->UIStats = VUIObjectUIStats::Normal;

			UpdateObject();
		}
	}
	void Hide() {
		if (Surface()->UIStats != VUIObjectUIStats::Hidden) {
			Surface()->UIStats = VUIObjectUIStats::Hidden;

			UpdateObject();
		}
	}

public:
	/*
	 * Update Functional:
	 *	@description  : Send Repaint Requset To Parent Object
	*/
	virtual void Update(VRect Rect) {
		if (Parent() != nullptr) {
			if (Rect.Overlap(
				*(Parent()->SurfaceRegion().Clone().OffsetRV(0, 0))
			) == true) {
				if (Parent()->IsWidget() == false) {
					Rect = Parent()->SurfaceRegion();
				}

				return Parent()->Update(Rect);
			}
		}
	}
	/*
	 * Update Functional:
	 *	@description  : Update Itself
	*/
	void Update() {
		Update(SurfaceRegion());
	}

	/*
	 * UpdateObject Functional:
	 *	@description  : Use The Object's Region To Update
	*/
	void UpdateObject() {
		Update(SurfaceRegion());
	}

public:
	/*
	 * Virtual Event Functional, Child Object Inherit Required
	*/

	virtual void GotMouseFocus() {  /* Empty */ }
	virtual void LosedMouseFocus() {  /* Empty */ }
	virtual void MouseLeftClicked(VMouseClickedFlag) {  /* Empty */ }
	virtual void MouseRightClicked(VMouseClickedFlag) {  /* Empty */ }
	virtual void MouseMiddleClicked(VMouseClickedFlag) {  /* Empty */ }
	virtual bool MouseMiddleDraged(short) { return false; }

	virtual bool CheckDown(VKeyClickedMessage*) { return false; }

	/*
	 * IMECharInputed virtual Functional:
	 *	@descrpition  : The IME Char Inputed
	 *	@return value : Is Take This IMEChar
	*/
	virtual bool IMECharInputed(wchar_t Message) { return false; }

protected:
	/*
	 * CheckUIFocusStats Functional:
	 *	@description  : Check the UI Focus Stats and Call the Message Functional
	*/
	bool CheckUIFocusStats(VPoint MouseStats, VMessage* ResourceMessage) {
		if (IsAnimation() == true ||
			GetWidth() == 0 || GetHeight() == 0) {
			return false;
		}

		if (IsGlobalIDLocking() == true &&
			GetGlobalFocusID() != Kernel()->GlobalID) {
			return false;
		}

		if (MouseStats.InsideRect(SurfaceRegion())) {
			if (ResourceMessage->GetType() == VMessageType::CheckLocalFocusMessage) {
				if (GetGlobalFocusID() != Kernel()->GlobalID) {
					Surface()->UIStats = VUIObjectUIStats::Normal;

					UpdateObject();
					LosedMouseFocus();

					LosedFocus.Emit();

					return false;
				}

				return false;
			}

			if (Surface()->UIStats != VUIObjectUIStats::OnFocus) {
				VCheckFocusMessage* FocusMessage = new VCheckFocusMessage(MouseStats);

				SendMessageToGlobalWidget(FocusMessage);

				delete FocusMessage;

				Surface()->UIStats = VUIObjectUIStats::OnFocus;
				UpdateObject();

				SetGlobalFocusID(Kernel()->GlobalID);

				GotMouseFocus();

				InFocus.Emit();
			}

			if (ResourceMessage->GetType() == VMessageType::MouseClickedMessage) {
				auto MouseClickedMessage = static_cast<VMouseClickedMessage*>(ResourceMessage);

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

			return true;
		}
		else if (Surface()->UIStats == VUIObjectUIStats::OnFocus) {
			Surface()->UIStats = VUIObjectUIStats::Normal;
			UpdateObject();

			SetGlobalFocusID(L"");

			LosedMouseFocus();

			LosedFocus.Emit();

			return false;
		}


		return false;
	}

	/*
	 * CheckElementUIStats Functional:
	 *	@description  : Cheeck the UI Focus Stats and Call the
	 *					Message Functional ( Including Child Objects )
	*/
	bool CheckElementUIStats(VMessage* ResourceMessage) {
		VPoint MousePosition;

		switch (ResourceMessage->GetType()) {
		case VMessageType::MouseMoveMessage: {
			MousePosition = static_cast<VMouseMoveMessage*>(ResourceMessage)->MousePosition;

			break;
		}
		case VMessageType::MouseClickedMessage: {
			MousePosition = static_cast<VMouseClickedMessage*>(ResourceMessage)->MousePosition;

			break;
		}

		default: {
			return false;
		}
		}

		if (SendMessageToChild(ResourceMessage) == true) {
			return true;
		}
		else {
			auto Result = CheckUIFocusStats(MousePosition, ResourceMessage);

			return Result;
		}

		return false;
	}

	/*
	 * EditCanvas Functional:
	 *	@description  : Edit The Final Canvas
	*/
	virtual void EditCanvas(VCanvas* Canvas) {  /* Empty */ }

public:
	/*
	 * SysDealyMessage Functional:
	 *	@descripition  : Dealy Message System Level Functional
	*/
	bool SysDealyMessage(VMessage* Message) {
		if (IsApplication() == true) {
			return SendMessageToChild(Message, false);
		}

		if (Surface()->UIStats == VUIObjectUIStats::Hidden) {
			return false;
		}

		DealyMessage(Message);

		switch (Message->GetType()) {
		case VMessageType::GetRepaintAeraMessage: {
			/* If Two Region Already Own Across Aera, Fusion It */
			VGetRepaintAeraMessage* RepaintMessage = static_cast<VGetRepaintAeraMessage*>(Message);
			if (*(RepaintMessage->RepaintAera) != SurfaceRegion() &&
				RepaintMessage->RepaintAera->Overlap(SurfaceRegion()) == true) {
				(*(RepaintMessage->RepaintAera)).FusionRect(SurfaceRegion());

				return true;
			}

			return false;
		}
		case VMessageType::MouseMoveMessage: {
			return CheckElementUIStats(Message);
		}
		case VMessageType::MouseClickedMessage: {
			return CheckElementUIStats(Message);
		}
		case VMessageType::IMECharMessage: {
			auto IMEMessage = static_cast<VIMECharMessage*>(Message);

			if (IMECharInputed(IMEMessage->IMEChar) == false) {
				SendMessageToChild(IMEMessage);
			}

			break;
		}
		case VMessageType::RepaintMessage: {
			auto RepaintMesage = static_cast<VRepaintMessage*>(Message);

			if (RepaintMesage->DirtyRectangle.Overlap(SurfaceRegion()) &&
				(Parent()->IsApplication() == true ? true :
					Parent()->GetRegoin().Clone().
					OffsetRV(Parent()->GetX(), Parent()->GetY())
					->Overlap(Parent()->SurfaceRect()))
				) {

				VRepaintMessage* ChildRepaintMessage = RepaintMesage;

				if (ObjectCanvas != nullptr) {
					delete ObjectCanvas;

					ObjectCanvas = nullptr;
				}

				if (Surface()->Transparency != 0) {
					ObjectCanvas = new VCanvas(SurfaceRegion().GetWidth(),
						SurfaceRegion().GetHeight());
					OnPaint(ObjectCanvas);

					if (IsWidget() == false && IsApplication() == false) {
						ChildRepaintMessage = new VRepaintMessage(*RepaintMesage);

						ChildRepaintMessage->DirtyRectangle = *(SurfaceRegion().Clone().OffsetRV(0, 0));
					}
					SendMessageToChild(ChildRepaintMessage, false);

					if (ChildRepaintMessage != RepaintMesage) {
						delete ChildRepaintMessage;
					}

					if (Surface()->Transparency != 255) {
						ObjectCanvas->SetTransparency(Surface()->Transparency);
					}

					EditCanvas(ObjectCanvas);

					GetParentCanvas()->PaintCanvas(Surface()->Rect.left, Surface()->Rect.top, ObjectCanvas);

					delete ObjectCanvas;

					ObjectCanvas = nullptr;
				}

				return true;
			}

			return false;
		}
		case VMessageType::MouseWheelMessage: {
			auto WheelMessage = static_cast<VMouseWheelMessage*>(Message);

			return MouseMiddleDraged(WheelMessage->WheelValue);
		}
		case VMessageType::KeyClickedMessage: {
			auto KeyMessage = static_cast<VKeyClickedMessage*>(Message);

			if (CheckDown(KeyMessage) == false) {
				SendMessageToChild(KeyMessage);
			}

			break;
		}
		case VMessageType::FreeResourceMessage: {
			if (ObjectCanvas != nullptr) {
				delete ObjectCanvas;

				ObjectCanvas = nullptr;
			}

			return true;
		}
		case VMessageType::CheckLocalFocusMessage: {
			auto FocusMessage = static_cast<VCheckFocusMessage*>(Message);

			CheckUIFocusStats(FocusMessage->FocusPoint, FocusMessage);

			SendMessageToChild(Message, false);

			return false;
		}
		case VMessageType::KillFocusMessage: {
			if (GetGlobalFocusID() == Kernel()->GlobalID) {
				LosedMouseFocus();
				LosedFocus.Emit();
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

	/*
	 * SetTransparency Functional:
	 *	@description  : Set the Object's Transparency
	*/
	void SetTransparency(short Transparency) {
		ObjectSurface->Transparency = Transparency;

		Update();
	}
	/*
	 * SetTransparency Functional:
	 *	@description  : Get the Object's Transparency
	*/
	short GetTransparency() {
		return ObjectSurface->Transparency;
	}

protected:
	/* Object Kernel (Attribute) */
	VUIObjectKernel* ObjectKernel;

	/* UI Surface Stats */
	VUIObjectSurface* ObjectSurface;

public:
	/*
	 * GetGlobalFocus Functional:
	 *	@description  : Set the Global Focus
	*/
	void GetGlobalFocus() {
		SetGlobalFocusID(ObjectKernel->GlobalID);
	}

public:
	/*
	 * Build up Functional
	*/

	VUIObject(VUIObject* Parent) {
		/* Init Kernel */
		ObjectKernel = new VUIObjectKernel;
		/* Init Surface */
		ObjectSurface = new VUIObjectSurface;

		/* Set Object's Parent Object */
		SetParent(Parent);
	}

public:
	/*
	 * IsApplicaiton virtual Functional:
	 *	@description  : Describe This Object Is Application Object Or Not
	 *					Default Return false
	*/
	virtual bool IsApplication() {
		return false;
	}
	/*
	 * IsWidget virtual Functional:
	 *	@description  : Describe This Object Is Widget Object Or Not
	 * 					Default Return false
	*/
	virtual bool IsWidget() {
		return false;
	}

public:
	/*
	 * SetParent Functional:
	 *	@description  : This Functional Set Object Parent Into Other Object
	*/
	void SetParent(VUIObject* NewParent) {
		if (ObjectKernel->Parent != nullptr) {
			/* Adjust Child Id For Container's Objects */
			for (auto Object = Parent()->Kernel()->ChildObjectContainer.begin() + (Kernel()->ParentID + 1);
				Object != Parent()->Kernel()->ChildObjectContainer.end(); ++Object) {
				--Object.operator*()->Kernel()->ParentID;
			}

			/* Erase This Object From Parent */
			Parent()->Kernel()->ChildObjectContainer.
				erase(Parent()->Kernel()->ChildObjectContainer.begin()
					+ Kernel()->ParentID);
		}

		ObjectKernel->Parent = NewParent;

		/* If NewParent Isn't nullptr, Init Project Attrubite  */
		if (VUnlikely(NewParent != nullptr)) {
			Parent()->Kernel()->ChildObjectContainer.push_back(this);

			/* Generated Object ID */
			Kernel()->ParentID = Parent()->Kernel()->ChildObjectContainer.size() - 1;
			Kernel()->GlobalID = GeneratedGlobalID();
		}
	}

public:
	/*
	 * DealyMessage virtual functional:
	 *	@description  : For User To Defition The Message Dealy Method
	*/
	virtual void DealyMessage(VMessage* Message) {

	}

public:
	/*
	 * OnPaint virtual Functional:
	 *	@description  : Paint the Control On Canvas
	*/
	virtual void OnPaint(VCanvas* Canvas) {

	}

public:
	/*
	 * Geomtery Functional
	*/

	int  GetX()      const {
		return ObjectSurface->Rect.left;
	}
	int  GetY()      const {
		return ObjectSurface->Rect.top;
	}
	int  GetWidth()  const {
		return ObjectSurface->Rect.right - ObjectSurface->Rect.left;
	}
	int  GetHeight() const {
		return ObjectSurface->Rect.bottom - ObjectSurface->Rect.top;
	}

	virtual void Resize(int Width, int Height) {
		auto OldRect = Surface()->Rect.Clone();

		Surface()->Rect.right = OldRect.left + Width;
		Surface()->Rect.bottom = OldRect.top + Height;

		OldRect.FusionRect(Surface()->Rect);

		Update(OldRect);

		SizeChanged.Emit(Width, Height);
	}
	void Resize(VSize Size) {
		Resize(Size.x, Size.y);
	}
	virtual void Move(int X, int Y) {
		auto OldRect = Surface()->Rect.Clone();

		auto Width = GetWidth();
		auto Height = GetHeight();

		Surface()->Rect.left = X;
		Surface()->Rect.top = Y;
		Surface()->Rect.right = X + Width;
		Surface()->Rect.bottom = Y + Height;

		OldRect.Offset(-1, -1, 1, 1);

		Update();
		Update(OldRect);

		PositionChanged.Emit(X, Y);
	}
	void Move(VPoint Point) {
		Move(Point.x, Point.y);
	}

public:
	/*
	 * CheckFrame virtual Functional:
	 *	@description  : Each Frame Check the Stats
	*/
	virtual void CheckFrame() {  /* Empty */ }

	/*
	 * CheckAllFrame Functional:
	 *	@description  : Check All the Objects Frame
	*/
	void CheckAllFrame(bool CheckChild = false) {
		CheckFrame();

		if (CheckChild == false) {
			for (auto& ChildObject : Kernel()->ChildObjectContainer) {
				ChildObject->CheckFrame();
			}
		}
		else {
			for (auto& ChildObject : Kernel()->ChildObjectContainer) {
				ChildObject->CheckFrame();
				ChildObject->CheckAllFrame(CheckChild);
			}
		}
	}
};

VLIB_END_NAMESPACE
