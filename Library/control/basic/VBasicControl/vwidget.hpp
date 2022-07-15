/*
 * VWidget.hpp
 *	@description : VLib's Widget
 *	@birth		 : 2022/6.11
*/
#pragma once

#include "vapplication.hpp"

#include "../../../basic/vbasic/vtimer.hpp"

#include <map>

#pragma comment(lib, "Imm32.lib")

VLIB_BEGIN_NAMESPACE

WNDPROC VOriginWindowProcessFunctional;
std::function<void(HWND, int, int)> VMainWindowResizeProcess;
std::map<HWND, VPoint>              VMainWindowMaxInfoMessage;
std::map<HWND, VPoint>              VMainWindowIMEPositionInfo;

/*
 * VMainWindowProcess Functional:
 *	@desecription  : The VMainWindow Window Event Dealy Function
*/
LRESULT VMainWindowProcess(HWND Handle, UINT Message, WPARAM wParameter, LPARAM lParameter) {
	switch (Message) {
	case WM_SIZING: {
		RECT* Rect = (RECT*)lParameter;

		VMainWindowResizeProcess(Handle, Rect->right - Rect->left - 16, Rect->bottom - Rect->top - 39);

		return 0;
	}
	case WM_SIZE: {
		if (wParameter != SIZE_RESTORED) {
			RECT Rect;

			GetWindowRect(Handle, &Rect);

			VMainWindowResizeProcess(Handle, Rect.right - Rect.left - 18, Rect.bottom - Rect.top - 39);
		}
		else {
			RECT Rect;

			GetWindowRect(Handle, &Rect);

			VMainWindowResizeProcess(Handle, LOWORD(lParameter), HIWORD(lParameter));
		}

		return 0;
	}
	case WM_GETMINMAXINFO: {
		if (VMainWindowMaxInfoMessage.find(Handle) != VMainWindowMaxInfoMessage.end()) {
			MINMAXINFO* MaxMinInfo;
			MaxMinInfo = (PMINMAXINFO)lParameter;

			MaxMinInfo->ptMinTrackSize.x = VMainWindowMaxInfoMessage[Handle].x;
			MaxMinInfo->ptMinTrackSize.y = VMainWindowMaxInfoMessage[Handle].y;
		}

		return DefWindowProc(Handle, Message, wParameter, lParameter);
	}
	case WM_IME_COMPOSITION: {
		if (VMainWindowIMEPositionInfo.find(Handle) != VMainWindowIMEPositionInfo.end()) {
			HIMC IMCHandle = ImmGetContext(Handle);

			if (IMCHandle) {
				COMPOSITIONFORM Composition;
				Composition.dwStyle = CFS_POINT;
				Composition.ptCurrentPos.x = VMainWindowIMEPositionInfo[Handle].x;
				Composition.ptCurrentPos.y = VMainWindowIMEPositionInfo[Handle].y;

				LOGFONT Font;

				gettextstyle(&Font);

				_tcscpy_s(Font.lfFaceName, L"微软雅黑");

				Font.lfQuality = PROOF_QUALITY;

				ImmAssociateContext(Handle, IMCHandle);
				ImmSetCompositionFont(IMCHandle, &Font);
				ImmSetCompositionWindow(IMCHandle, &Composition);
			}
		}
	}
	}

	return VOriginWindowProcessFunctional(Handle, Message, wParameter, lParameter);
}

/*
 * VMainWindow class:
 *	@description  : The Main Window in VLib
*/
class VMainWindow : public VUIObject {
private:
	VWidgetTheme* Theme;

private:
	std::vector<VRepaintMessage*> RepaintMessageStack;

	HWND                          WindowHandle;

private:
	int MinimalWidth = 0;
	int MinimalHeight = 0;

public:
	/*
	 * SetMinimalWidth functional:
	 *	@description  : Set the Minimal Zoom Width
	*/
	void SetMinimalWidth(int Width) {
		VMainWindowMaxInfoMessage.erase(GetWinID());

		MinimalWidth = Width;

		VMainWindowMaxInfoMessage.insert(std::pair<HWND, VPoint>(GetWinID(), { MinimalWidth, MinimalHeight }));
	}
	/*
	 * SetMinimalHeight functional:
	 *	@description  : Set the Minimal Zoom Height
	*/
	void SetMinimalHeight(int Height) {
		VMainWindowMaxInfoMessage.erase(GetWinID());

		MinimalHeight = Height;

		VMainWindowMaxInfoMessage.insert(std::pair<HWND, VPoint>(GetWinID(), { MinimalWidth, MinimalHeight }));
	}

	/*
	 * SetGlobalIMEPosition override Functional
	*/
	void SetGlobalIMEPosition(int X, int Y) {
		if (VMainWindowIMEPositionInfo.find(GetWinID()) != VMainWindowIMEPositionInfo.end()) {
			VMainWindowIMEPositionInfo.erase(GetWinID());
		}

		VMainWindowIMEPositionInfo.insert(std::pair<HWND, VPoint>(GetWinID(), { X, Y }));
	}

	/*
	 * GetParentWindowHandle override Functional:
	*/
	HWND GetParentWindowHandle() override {
		return GetWinID();
	}

private:
	VTimer                        FpsTimer;

private:
	bool                          Win32Resized = false;
	int                           ResizedWidth = 0;
	int                           ResizedHeight = 0;

	std::wstring                  GlobalFocusID;

	bool                          LockFocusID = false;

protected:
	/*
	 * SetGlobalFocusID override Functional
	*/
	void SetGlobalFocusID(std::wstring ID) override {
		if (LockFocusID == false) {
			GlobalFocusID = ID;
		}
	}
	/*
	 * GetGlobalFocusID override Functional
	*/
	std::wstring GetGlobalFocusID() override {
		return GlobalFocusID;
	}
	/*
	 * LockGlobalFocusID override Functional
	*/
	void LockGlobalFocusID(std::wstring ID) override {
		if (LockFocusID == false) {
			LockFocusID = true;

			GlobalFocusID = ID;
		}
	}
	/*
	 * UnlockGlobalFocusID override Functional
	*/
	void UnlockGlobalFocusID() override {
		LockFocusID = false;
	}

	/*
	 * IsGlobalIDLocking override Functional
	*/
	bool IsGlobalIDLocking() override {
		return LockFocusID;
	}

private:
	void VMainWindowResize(HWND Handle, int Width, int Height) {
		if (Width <= MinimalWidth) {
			Width = MinimalWidth;
		}
		if (Width <= MinimalWidth) {
			Height = MinimalWidth;
		}

		if (Handle == WindowHandle) {
			Win32Resized = true;

			ResizedWidth = Width;
			ResizedHeight = Height;
		}
	}

protected:
	/*
	 * SendMessageToGlobalWdiget override Functional
	*/
	void SendMessageToGlobalWidget(VMessage* Message) override {
		SendMessageToChild(Message);
	}

private:
	/*
	 * InitKernel Functional:
	 *	@description  : Init Widget WKernel
	*/
	void InitKernel() {
		FpsTimer.Start(16);
	}

	/*
	 * InitWindowStyle Functional:
	 *	@description  : Init Win32 Style
	*/
	void InitWindowStyle(bool Sizble) {
		if (Sizble == true) {
			SetWindowLong(GetHWnd(), GWL_STYLE, GetWindowLong(GetHWnd(), GWL_STYLE) | (WS_MAXIMIZEBOX));
			SetWindowLong(GetHWnd(), GWL_STYLE, (GetWindowLong(GetHWnd(), GWL_STYLE) | WS_THICKFRAME));

			VOriginWindowProcessFunctional = reinterpret_cast<WNDPROC>(GetWindowLongPtr(GetHWnd(), GWLP_WNDPROC));
			SetWindowLongPtr(GetHWnd(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(VMainWindowProcess));
		}

		BeginBatchDraw();

		VMainWindowResizeProcess = std::bind(&VMainWindow::VMainWindowResize, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	/*
	 * InitWindow Functional:
	 *	@description  : Init the Widget
	*/
	HWND InitWindow(int Width, int Height, bool Sizble) {
		HWND Handle = initgraph(Width, Height);

		InitWindowStyle(Sizble);

		return Handle;
	}

protected:
	/*
	 * Update override Functional:
	 *	@description  : Set Repaint Aera
	*/
	void Update(VRect Rect) override {
		VGetRepaintAeraMessage RepaintMessage(Rect);

		std::map<int, int>     AlreadySendedObject;

		int                    ObjectCount = 0;

		for (auto ChildObject = Kernel()->ChildObjectContainer.begin();
			ChildObject != Kernel()->ChildObjectContainer.end(); ++ObjectCount) {
			if (AlreadySendedObject.find(ObjectCount) == AlreadySendedObject.end()) {
				if (ChildObject.operator*()->SysDealyMessage(&RepaintMessage) == true) {
					AlreadySendedObject.insert(std::pair<int, bool>(ObjectCount, 1));

					ChildObject = Kernel()->ChildObjectContainer.begin();

					ObjectCount = 0;
					
					continue;
				}
			}

			++ChildObject;
		}

		for (auto& AlreadyExsitsMessage : RepaintMessageStack) {
			if (AlreadyExsitsMessage->DirtyRectangle == *(RepaintMessage.RepaintAera)) {
				return;
			}

			if (AlreadyExsitsMessage->DirtyRectangle.Overlap(*(RepaintMessage.RepaintAera)) == true) {
				AlreadyExsitsMessage->DirtyRectangle.FusionRect(*(RepaintMessage.RepaintAera));
				return;
			}
		}

		VRect RepaintRect = *(RepaintMessage.RepaintAera);

		RepaintMessageStack.push_back(new VRepaintMessage(RepaintRect));
	}

public:
	/*
	 * SetBackgroundColor Functional:
	 *	@description  : Set the Window Background Color
	*/
	void SetBackgroundColor(VColor Color) {
		Theme->BackgroundColor = Color;

		Update(Surface()->Rect);
	}

public:
	/*
	 * SizeOnChange Signal:
	 *	@description  : When the Window's Size Changed Emit this Signal
	*/
	VSignal<int, int> SizeOnChange;

public:
	/*
	 * IsWidget override Functional:
	 *	@descirption : This is Widget Okay?
	*/
	bool IsWidget() override {
		return true;
	}

public:
	/*
	 * GetWinID Functional:
	 *	@description  : Get the WIN32 HWND of Widget
	*/
	HWND GetWinID() {
		return WindowHandle;
	}

public:
	/*
	 * OnPaint Functional
	*/
	void OnPaint(VCanvas* Canvas, VRect Rect) {
		VPainterDevice Device(Canvas);
		VSolidBrush    SolidBrush(Theme->BackgroundColor);

		Device.SolidRectangle(&SolidBrush, Rect);
	}

public:
	/*
	 * Build up & Deleter Functional
	*/

	VMainWindow(int Width, int Height, VApplication* Parent, bool Sizble = true) : VUIObject(Parent) {
		VUIObject::Resize(Width, Height);

		Theme = static_cast<VWidgetTheme*>(SearchThemeFromParent(VWIDGET_THEME));

		if (VUnlikely(VCoreApplication::Instance() == nullptr)) {
			VAssertBox(L"VApplication Should be Inited Before VWidget or VMainWindow");

			exit(-1);
		}

		InitKernel();
		WindowHandle = InitWindow(Width, Height, Sizble);

		Update(Surface()->Rect);
	}
	~VMainWindow() { EndBatchDraw(); }

	void CheckFrame() {
		if (FpsTimer.End() == true) {
			FpsTimer.Start(16);

			if (Win32Resized == true) {
				Win32Resized = false;

				EasyXWindowResize(ResizedWidth, ResizedHeight);
				VUIObject::Resize(ResizedWidth, ResizedHeight);

				SizeOnChange.Emit(ResizedWidth, ResizedHeight);

				Update(Surface()->Rect);
			}

			if (RepaintMessageStack.empty() == false) {
				ObjectCanvas = new VCanvas(GetWidth(), GetHeight());

				for (auto& RepaintMessage : RepaintMessageStack) {
					OnPaint(ObjectCanvas, RepaintMessage->DirtyRectangle);

					SendMessageToChild(RepaintMessage, false);

					delete RepaintMessage;
				}

				RepaintMessageStack.clear();

				VGdiplus::Graphics     FlushGraphics(GetImageHDC());

				FlushGraphics.DrawImage(ObjectCanvas->GetNativeImage(), 0, 0);

				VFreeSourceMessage  GcMessage;
				SendMessageToChild(&GcMessage, false);

				delete ObjectCanvas;
			}

			for (auto& ChildObject : Kernel()->ChildObjectContainer) {
				ChildObject->CheckAllFrame(true);
			}


			FlushBatchDraw();
		}
	}

	void Move(int X, int Y) override {
		MoveWindow(GetWinID(), X, Y, GetWidth(), GetHeight(), FALSE);
	}
	void Resize(int Width, int Height) override {
		RECT Rect;
		GetWindowRect(GetWinID(), &Rect);
		
		MoveWindow(GetWinID(), Rect.left, Rect.top, Width, Height, true);
	}
};

VLIB_END_NAMESPACE
