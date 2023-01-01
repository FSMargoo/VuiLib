#include "../../../include/core/widget/vwidget.h"

VLIB_BEGIN_NAMESPACE

#include <cstring>

namespace Core {
	VMainWindow::VMainWindow(const int& Width, const int& Height, VApplication* Parent, const bool& Sizble)
		: VUIObject(Parent) {
		Theme = static_cast<VMainWindowTheme*>(GetTargetTheme(VUIThemeType::VMainWindow));
		VLIB_CHECK_REPORT(Theme == nullptr, L"Cannot load VMainWindow object's theme from current application!");

		Theme->WindowSizble = Sizble;

		if (Parent == nullptr) {
			VLIB_REPORT_ERROR(L"VMainWindow should be created after VApplication!");

			exit(-1);
		}

		InitWindow();
		InitKernel();
		InitIME();

		Resize(Width, Height);

		Update({ 0, 0, GetWidth(), GetHeight() });
	}
	VMainWindow::~VMainWindow() {
		_VMainConfigs.erase(WindowHandle);
		_VMainConfigs[WindowHandle] = nullptr;

		VUIObject::~VUIObject();
	}
	void VMainWindow::Win32ThreadResized(int Width, int Height) {
		Win32Cache.UserSetGeomtery = true;
		Win32Cache.UserSetWidth = Width;
		Win32Cache.UserSetHeight = Height;
	}
	void VMainWindow::Win32ThreadRepaint() {
		Win32Cache.Repaint = true;
	}
	void VMainWindow::Win32LoseFocus() {
		CallWidgetUnlockFocusID();

		VKillFocusMessage* KillFocusMessage = new VKillFocusMessage(CallWidgetGetHWND());
		SendMessageToChild(KillFocusMessage, false);

		delete KillFocusMessage;
	}
	bool VMainWindow::IsWidget() const {
		return true;
	}
	void VMainWindow::SetQuitEvent(const std::function<bool()>& QEventFunction) {
		WindowConfig.WindowQuitOnClicked = QEventFunction;
	}

	void VMainWindow::InitKernel() {
		FpsTimer.Start(14);
		_VMainConfigs.insert(std::pair<HWND, VWin32ThreadPipe*>(WindowHandle, &WindowConfig));
	}
	void VMainWindow::InitWindow() {
		WindowHandle = Win32Core::VWin32CreateWindow(GetWidth(), GetHeight(), L"", L"VuilibMainWindow", NULL);

#ifdef VENABLE_DWM_ARCLIY_EFFECT
		InitArcliyWindow();
#endif

		if (Theme->WindowSizble) {
			SetWindowLongPtr(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) | (WS_MAXIMIZEBOX));
			SetWindowLongPtr(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) | WS_THICKFRAME));
		}
		else {
			SetWindowLongPtr(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_MAXIMIZEBOX));
			SetWindowLongPtr(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_THICKFRAME)));
		}

		WindowConfig.OriginWindowProcess = reinterpret_cast<WNDPROC>(GetWindowLongPtr(WindowHandle, GWLP_WNDPROC));
		SetWindowLongPtr(WindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_VWidgetWNDPROC));

		Direct2DRender = new VDCRender(VDirectXD2DFactory.GetInstance(), GetDC(WindowHandle), { 0, 0, GetWidth(), GetHeight() }, true);
		BufferPainter  = new VPainter(VRenderHandle(nullptr, nullptr, nullptr, Direct2DRender->GetDirectXRenderTarget(), nullptr));

		WindowConfig.WindowOnSize = [this](auto PH1, auto PH2) {
			Win32ThreadResized(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); 
		};
		WindowConfig.LosedUserFocus		 = [this]() -> void { Win32LoseFocus(); };
		WindowConfig.WinRepaintMessage	 = [this]() { Win32ThreadRepaint(); };
		WindowConfig.WindowQuitOnClicked = [this]() -> bool { return DefaultOnQuitFunction(); };

		WindowConfig.IMEFontStyle.lfHeight = 16;
		WindowConfig.IMEFontStyle.lfWeight = FW_NORMAL;
		WindowConfig.IMEFontStyle.lfQuality = PROOF_QUALITY;
		wcscpy_s(WindowConfig.IMEFontStyle.lfFaceName, L"微软雅黑");
	}
	void VMainWindow::InitIME() {
		WindowConfig.IMEX = -1;
		WindowConfig.IMEY = -1;
	}
	bool VMainWindow::DefaultOnQuitFunction() {
		return false;
	}
	void VMainWindow::CallWidgetSetIME(const int& X, const int& Y) {
		WindowConfig.IMEX = X;
		WindowConfig.IMEY = Y;
	}
	void VMainWindow::CallWidgetSetFocusID(const std::wstring& ObjectID) {
		if (!WindowConfig.LockFocus) {
			WindowConfig.GlobalFocusID = ObjectID;
		}
	}
	std::wstring VMainWindow::CallWidgetGetFocusID() const {
		return WindowConfig.GlobalFocusID;
	}
	void VMainWindow::CallWidgetLockFocusID() {
		WindowConfig.LockFocus = true;
	}
	void VMainWindow::CallWidgetUnlockFocusID() {
		WindowConfig.LockFocus = false;
	}
	bool VMainWindow::CallWidgetGetLockingStatus() {
		return WindowConfig.LockFocus;
	}
	void VMainWindow::CallWidgetSetIMEFontStyle(const LOGFONT& LogfontStyle) {
		WindowConfig.IMEFontStyle = LogfontStyle;
	}

	LOGFONT VMainWindow::GetWidgetIMEFontStlye() const {
		return WindowConfig.IMEFontStyle;
	}

	void VMainWindow::CallWidgetSendMessage(VMessage* Message) {
		SendMessageToChild(Message);
	}
	HWND VMainWindow::CallWidgetGetHWND() {
		return WindowHandle;
	}
	VCanvasPainter* VMainWindow::CallWidgetGetCanvas() {
		return Canvas;
	}
	VRenderHandle VMainWindow::CallWidgetGetDCRenderTarget() const {
		VRenderHandle RenderHandle;
		RenderHandle._IRenderTarget = Direct2DRender->DirectXDCTarget.Get();
		RenderHandle.Allocator		= GetWidgetAllocator();
		
		return RenderHandle;
	}
	VKits::VAllocator* VMainWindow::GetWidgetAllocator() const {
		return RepaintAllocator;
	}

	void VMainWindow::OnPaint(VCanvasPainter* Canvas, const VRect& Rect) {
		VSolidBrush BackgroundColor(Theme->BackgroundColor, CallWidgetGetRenderHandle());

		Canvas->SolidRectangle(Rect, &BackgroundColor);
	}

	bool VMainWindow::CheckQuitWindowMessage(VMessage* Message) {
		if (Message->GetType() == VMessageType::QuitWindowMessage &&
			Message->MessageTriggerWidget == WindowHandle) {
			exit(0);

			return true;
		}

		return false;
	}

	void VMainWindow::Update(VRect UpdateRect) {
		VGetRepaintAeraMessage RepaintMessage(CallWidgetGetHWND(), UpdateRect);

		std::map<int, int>     AlreadySendedObject;

		int                    ObjectCount = 0;

		for (auto ChildObject = ObjectKernel.ChildObjectContainer.begin();
			ChildObject != ObjectKernel.ChildObjectContainer.end(); ++ObjectCount) {
			if (AlreadySendedObject.find(ObjectCount) == AlreadySendedObject.end()) {
				if (ChildObject.operator*()->SysProcessMessage(&RepaintMessage)) {
					AlreadySendedObject.insert(std::pair<int, bool>(ObjectCount, 1));

					ChildObject = ObjectKernel.ChildObjectContainer.begin();

					ObjectCount = -1;

					continue;
				}
			}

			++ChildObject;
		}

		for (auto& AlreadyExsitsMessage : RepaintMessages) {
			if (AlreadyExsitsMessage->DirtyRectangle == *(RepaintMessage.RepaintAera)) {
				return;
			}

			if (AlreadyExsitsMessage->DirtyRectangle.Overlap(*(RepaintMessage.RepaintAera))) {
				AlreadyExsitsMessage->DirtyRectangle.FusionRect(*(RepaintMessage.RepaintAera));
				return;
			}
		}

		RepaintMessages.push_back(std::move(new VRepaintMessage(CallWidgetGetHWND(), *(RepaintMessage.RepaintAera))));
	}

	void VMainWindow::Show() {
		VUIObject::Show();

		ShowWindow(WindowHandle, SW_SHOW);
	}
	void VMainWindow::Hide() {
		VUIObject::Hide();

		ShowWindow(WindowHandle, SW_HIDE);
	}

	void VMainWindow::CheckFrame() {
		if (FpsTimer.End()) {
			FpsTimer.Start(14);

			if (Win32Cache.Repaint) {
				if (!Win32Cache.UserSetGeomtery) {
					Win32Cache.Repaint = false;

					Update({ 0, 0, GetWidth(), GetHeight() });
				}
				else {
					Win32Cache.Repaint = false;
				}
			}

			if (Win32Cache.UserSetGeomtery) {
				Win32Cache.UserSetGeomtery = false;

				ObjectVisual.Rectangle.Right  = Win32Cache.UserSetWidth;
				ObjectVisual.Rectangle.Bottom = Win32Cache.UserSetHeight;

				Update(ObjectVisual.Rectangle);

				Resized.Emit(GetWidth(), GetHeight());

				RECT NewRect = {0, 0, Win32Cache.UserSetWidth, Win32Cache.UserSetHeight };
				Direct2DRender->DirectXDCTarget.Get()->BindDC(GetDC(WindowHandle), &NewRect);
			}

			if (!RepaintMessages.empty()) {
				RepaintAllocator = new VKits::VAllocator;

				Canvas			 = new VCanvasPainter(GetWidth(), GetHeight(),
					VRenderHandle(nullptr, nullptr, nullptr, Direct2DRender->GetDirectXRenderTarget(), RepaintAllocator));
				Canvas->BeginDraw();
				Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
				if (!WindowConfig.EnableRadius) {
					for (auto& Message : RepaintMessages) {
						OnPaint(Canvas, Message->DirtyRectangle);
						SendMessageToChild(Message, false);

						delete Message;
					}
				}
				else {
					for (auto& Message : RepaintMessages) {
						delete Message;
					}
					RepaintMessages.clear();

					VRepaintMessage RepaintAll(CallWidgetGetHWND(), { 0, 0, GetWidth(), GetHeight() });

					OnPaint(Canvas, RepaintAll.DirtyRectangle);
					SendMessageToChild(&RepaintAll, false);
				}

				Canvas->EndDraw();

				RepaintMessages.clear();
				if (WindowConfig.EnableRadius) {
					HDC     WindowDC	  = GetDC(WindowHandle);
					HBITMAP BorderImage	  = CreateCompatibleBitmap(WindowDC, GetWidth(), GetHeight());
					HDC     BorderImageDC = CreateCompatibleDC(WindowDC);
					HBITMAP OldBitmap	  = (HBITMAP)SelectObject(BorderImageDC, BorderImage);

					auto Property = D2D1::RenderTargetProperties(
						D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
						D2D1::PixelFormat(
							DXGI_FORMAT_B8G8R8A8_UNORM,
							D2D1_ALPHA_MODE_PREMULTIPLIED
						), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
					);
					ID2D1DCRenderTarget* DCRenderTarget;
					HRESULT Result = VDirectXD2DFactory.GetInstance()->CreateDCRenderTarget(
						&Property,
						&DCRenderTarget
					);
					RECT WindowRect = { 0, 0, GetWidth(), GetHeight() };

					ID2D1Bitmap* Bitmap;
					Canvas->GetDXObject()->GetBitmap(&Bitmap);
					VBitmapBrush BitmapBrush(Bitmap, CallWidgetGetRenderHandle());

					DCRenderTarget->BindDC(BorderImageDC, &WindowRect);

					DCRenderTarget->BeginDraw();

					DCRenderTarget->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
					DCRenderTarget->FillRoundedRectangle(D2D1_ROUNDED_RECT{
							{0, 0, static_cast<float>(GetWidth() - 1), static_cast<float>(GetHeight() - 1)},
							static_cast<float>(WindowConfig.BorderRadius.X),
							static_cast<float>(WindowConfig.BorderRadius.Y) }, BitmapBrush.GetDxBrush());

					DCRenderTarget->EndDraw();

					BLENDFUNCTION BlendFN = { 0 };
					BlendFN.BlendOp = AC_SRC_OVER;
					BlendFN.SourceConstantAlpha = 255;
					BlendFN.AlphaFormat = AC_SRC_ALPHA;

					POINT	SourcePoint = { 0, 0 };
					SIZE	WindowSize = { GetWidth(), GetHeight() };
					UpdateLayeredWindow(WindowHandle, WindowDC, NULL, &WindowSize, BorderImageDC,
						&SourcePoint, NULL, &BlendFN, ULW_ALPHA);

					ReleaseDC(WindowHandle, WindowDC);

					Bitmap->Release();
					DCRenderTarget->Release();

					SelectObject(BorderImageDC, OldBitmap);
					DeleteObject(BorderImage);
					ReleaseDC(WindowHandle, WindowDC);
					ReleaseDC(WindowHandle, BorderImageDC);
				}
				else {
					BufferPainter->BeginDraw();
					BufferPainter->DrawCanvas(GetRegion(), Canvas, GetRegion(), 1.f);
					BufferPainter->EndDraw();
				}

				delete Canvas;
				delete RepaintAllocator;

				RepaintAllocator = nullptr;
			}
		}
	}

	void VMainWindow::SetTitle(const std::wstring& WindowText) {
		Theme->WindowPlaneText = WindowText;
		SetWindowTextW(WindowHandle, Theme->WindowPlaneText.c_str());
	}
	void VMainWindow::SetBackgroundColor(const VColor& Color) {
		Theme->BackgroundColor = Color;

		Update({ 0, 0, GetWidth(), GetHeight() });
	}
	void VMainWindow::SetMaxSize(const VGeometry& MaxSize) {
		WindowConfig.UseMaxMinSize = true;
		WindowConfig.WindowMaxSize = MaxSize;
	}
	void VMainWindow::SetMiniSize(const VGeometry& MiniSize) {
		WindowConfig.UseMaxMinSize = true;
		WindowConfig.WindowMiniSize = MiniSize;
	}
	void VMainWindow::SetFrameless(const bool& FramelessStatus) {
		if (FramelessStatus) {
			WindowConfig.InFrameless = true;

			SetWindowLongPtr(GetLocalWinId(), GWL_STYLE, GetWindowLongPtr(GetLocalWinId(), GWL_STYLE) & ~(WS_THICKFRAME | WS_SIZEBOX | WS_CAPTION));

			RECT WindowRect;
			GetWindowRect(GetLocalWinId(), &WindowRect);
			MoveWindow(GetLocalWinId(), WindowRect.left, WindowRect.top, GetWidth(), GetHeight(), FALSE);
		}
		else {
			if (WindowConfig.InFrameless) {
				SetWindowLongPtr(GetLocalWinId(), GWL_STYLE, 349110272ll);
			}

			WindowConfig.InFrameless = false;
		}
	}
	void VMainWindow::SetSizable(const bool& Sizable) {
		WindowConfig.Sizable = Sizable;

		if (Sizable) {
			SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) | (WS_MAXIMIZEBOX));
			SetWindowLong(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) | WS_THICKFRAME));
		}
		else {
			SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_MAXIMIZEBOX));
			SetWindowLong(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_THICKFRAME)));
		}
	}
	void VMainWindow::SetRadius(VPoint Radius) {
		if (Radius == VPoint{ 0, 0 }) {
			WindowConfig.EnableRadius = false;
			WindowConfig.BorderRadius = Radius;

			WindowConfig.InFrameless = false;
		}
		else {
			SetWindowLongPtr(WindowHandle, GWL_EXSTYLE, WS_EX_LAYERED);

			WindowConfig.EnableRadius = true;
			WindowConfig.BorderRadius = Radius;

			WindowConfig.InFrameless = true;
		}

		Update({ 0, 0, GetWidth(), GetHeight() });
	}
	void VMainWindow::Resize(const int& Width, const int& Height) {
		VUIObject::Resize(Width, Height);

		if (Width > 0 && Height > 0) {
			RECT Rect;
			GetWindowRect(WindowHandle, &Rect);

			MoveWindow(WindowHandle, Rect.left, Rect.right, Width, Height, FALSE);
		}
	}

	HWND VMainWindow::GetLocalWinId() {
		return WindowHandle;
	}

	void VWidget::Win32ThreadResized(int Width, int Height) {
		Win32Cache.UserSetGeomtery = true;
		Win32Cache.UserSetWidth = Width;
		Win32Cache.UserSetHeight = Height;
	}
	void VWidget::Win32ThreadRepaint() {
		Win32Cache.Repaint = true;
	}
	void VWidget::Win32LoseFocus() {
		CallWidgetUnlockFocusID();

		VKillFocusMessage* KillFocusMessage = new VKillFocusMessage(CallWidgetGetHWND());
		SendMessageToChild(KillFocusMessage, false);

		delete KillFocusMessage;
	}

	void VWidget::SetQuitEvent(const std::function<bool()>& QEventFunction) {
		WindowConfig.WindowQuitOnClicked = QEventFunction;
	}

	void VWidget::InitKernel() {
		FpsTimer.Start(14);
		_VMainConfigs.insert(std::pair<HWND, VWin32ThreadPipe*>(WindowHandle, &WindowConfig));
	}
	void VWidget::InitWindow(const std::wstring& ClassName, HWND ParentWindow) {
		WindowHandle = Win32Core::VWin32CreateWindow(GetWidth(), GetHeight(), L"", ClassName.c_str(), ParentWindow);

#ifdef VENABLE_DWM_ARCLIY_EFFECT
		InitArcliyWindow();
#endif

		if (Theme->WindowSizble) {
			SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) | (WS_MAXIMIZEBOX));
			SetWindowLong(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) | WS_THICKFRAME));
		}
		else {
			SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_MAXIMIZEBOX));
			SetWindowLong(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_THICKFRAME)));
		}

		WindowConfig.OriginWindowProcess = reinterpret_cast<WNDPROC>(GetWindowLongPtr(WindowHandle, GWLP_WNDPROC));
		SetWindowLongPtr(WindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_VWidgetWNDPROC));

		Direct2DRender = new VDCRender(VDirectXD2DFactory.GetInstance(), GetDC(WindowHandle), { 0, 0, GetWidth(), GetHeight() }, true);
		BufferPainter  = new VPainter(VRenderHandle(nullptr, nullptr, nullptr, Direct2DRender->GetDirectXRenderTarget(), nullptr));

		WindowConfig.WindowOnSize = [this](auto PH1, auto PH2)
		{ Win32ThreadResized(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); };
		WindowConfig.LosedUserFocus = [this]() -> void
		{ Win32LoseFocus(); };
		WindowConfig.WinRepaintMessage = [this]() { Win32ThreadRepaint(); };
		WindowConfig.WindowQuitOnClicked = [this]() -> bool { return OnQuit(); };

		WindowConfig.IMEFontStyle.lfHeight = 16;
		WindowConfig.IMEFontStyle.lfWeight = FW_NORMAL;
		wcscpy_s(WindowConfig.IMEFontStyle.lfFaceName, L"微软雅黑");
		WindowConfig.IMEFontStyle.lfQuality = PROOF_QUALITY;
	}
	void VWidget::InitIME() {
		WindowConfig.IMEX = -1;
		WindowConfig.IMEY = -1;
	}

	VWidget::VWidget(const int& Width, const int& Height, VMainWindow* Parent, const std::wstring& ClassName, const bool& Sizble)
		: VUIObject(Parent) {
		Theme = static_cast<VMainWindowTheme*>(GetTargetTheme(VUIThemeType::VMainWindow));
		VLIB_CHECK_REPORT(Theme == nullptr, L"Object [VWidget] lost!");

		Theme->WindowSizble = Sizble;

		if (Parent == nullptr) {
			VLIB_REPORT_ERROR(L"VWidget should be init after VApplication!");

			exit(-1);
		}

		InitWindow(ClassName, Parent->CallWidgetGetHWND());
		InitKernel();
		InitIME();

		Resize(Width, Height);

		Update({ 0, 0, GetWidth(), GetHeight() });
	}

	VWidget::VWidget(const int& Width, const int& Height, VWidget* Parent, const std::wstring& ClassName, const bool& Sizble)
		: VUIObject(Parent) {
		Theme = static_cast<VMainWindowTheme*>(GetTargetTheme(VUIThemeType::VMainWindow));
		VLIB_CHECK_REPORT(Theme == nullptr, L"Object [VWidget] losed!");

		Theme->WindowSizble = Sizble;

		if (Parent == nullptr) {
			VLIB_REPORT_ERROR(L"VWidget should be init after VApplication!");

			exit(-1);
		}

		InitWindow(ClassName, Parent->CallWidgetGetHWND());
		InitKernel();
		InitIME();

		Resize(Width, Height);

		Update({ 0, 0, GetWidth(), GetHeight() });
	}

	VWidget::VWidget(const int& Width, const int& Height, VApplication* Parent, const std::wstring& ClassName, const bool& Sizble)
		: VUIObject(Parent) {
		Theme = new VMainWindowTheme(*static_cast<VMainWindowTheme*>(GetTargetTheme(VUIThemeType::VMainWindow)));
		VLIB_CHECK_REPORT(Theme == nullptr, L"Object [VWidget] lost!");

		Theme->WindowSizble = Sizble;

		if (Parent == nullptr) {
			VLIB_REPORT_ERROR(L"VWidget should be init after VApplication!");

			exit(-1);
		}

		InitWindow(ClassName, NULL);
		InitKernel();
		InitIME();

		Resize(Width, Height);

		Update({ 0, 0, GetWidth(), GetHeight() });
	}
	VWidget::~VWidget() {
		VUIObject::~VUIObject();

		_VMainConfigs.erase(WindowHandle);
		_VMainConfigs[WindowHandle] = nullptr;

		PostMessage(WindowHandle, WM_CLOSE, NULL, NULL);
	}

	void VWidget::Show() {
		VUIObject::Show();

		ShowWindow(WindowHandle, SW_SHOW);
	}
	void VWidget::Hide() {
		VUIObject::Hide();

		ShowWindow(WindowHandle, SW_HIDE);
	}

	void VWidget::CallWidgetSetIME(const int& X, const int& Y) {
		WindowConfig.IMEX = X;
		WindowConfig.IMEY = Y;
	}
	void VWidget::CallWidgetSetFocusID(const std::wstring& ObjectID) {
		WindowConfig.GlobalFocusID = ObjectID;
	}
	std::wstring VWidget::CallWidgetGetFocusID() const {
		return WindowConfig.GlobalFocusID;
	}
	void VWidget::CallWidgetLockFocusID() {
		WindowConfig.LockFocus = true;
	}
	void VWidget::CallWidgetUnlockFocusID() {
		WindowConfig.LockFocus = false;
	}
	bool VWidget::CallWidgetGetLockingStatus() {
		return WindowConfig.LockFocus;
	}
	void VWidget::CallWidgetSendMessage(VMessage* Message) {
		SendMessageToChild(Message);
	}
	HWND VWidget::CallWidgetGetHWND() {
		return WindowHandle;
	}
	VCanvasPainter* VWidget::CallWidgetGetCanvas() {
		return Canvas;
	}

	void VWidget::OnPaint(VCanvasPainter* Canvas, const VRect& Rect) {
		VSolidBrush BackgroundColor(Theme->BackgroundColor, CallWidgetGetRenderHandle());

		Canvas->SolidRectangle(Rect, &BackgroundColor);
	}

	bool VWidget::CheckQuitWindowMessage(VMessage* Message) {
		if (Message->GetType() == VMessageType::QuitWindowMessage &&
			Message->MessageTriggerWidget == WindowHandle) {
			VWidget::~VWidget();

			return true;
		}

		return false;
	}

	void VWidget::Update(VRect UpdateRect) {
		VGetRepaintAeraMessage RepaintMessage(CallWidgetGetHWND(), UpdateRect);

		std::map<int, int>     AlreadySendedObject;

		int                    ObjectCount = 0;

		for (auto ChildObject = ObjectKernel.ChildObjectContainer.begin();
			ChildObject != ObjectKernel.ChildObjectContainer.end(); ++ObjectCount) {
			if (AlreadySendedObject.find(ObjectCount) == AlreadySendedObject.end()) {
				if (ChildObject.operator*()->SysProcessMessage(&RepaintMessage)) {
					AlreadySendedObject.insert(std::pair<int, bool>(ObjectCount, 1));

					ChildObject = ObjectKernel.ChildObjectContainer.begin();

					ObjectCount = -1;

					continue;
				}
			}

			++ChildObject;
		}

		for (auto& AlreadyExsitsMessage : RepaintMessages) {
			if (AlreadyExsitsMessage->DirtyRectangle == *(RepaintMessage.RepaintAera)) {
				return;
			}

			if (AlreadyExsitsMessage->DirtyRectangle.Overlap(*(RepaintMessage.RepaintAera))) {
				AlreadyExsitsMessage->DirtyRectangle.FusionRect(*(RepaintMessage.RepaintAera));
				return;
			}
		}

		VRect RepaintRect = *(RepaintMessage.RepaintAera);

		RepaintMessages.push_back(new VRepaintMessage(CallWidgetGetHWND(), RepaintRect));
	}
	VKits::VAllocator* VWidget::GetWidgetAllocator() const {
		return RepaintAllocator;
	}

	void VWidget::CheckFrame() {
		if (FpsTimer.End()) {
			FpsTimer.Start(14);

			if (Win32Cache.Repaint) {
				if (!Win32Cache.UserSetGeomtery) {
					Win32Cache.Repaint = false;

					Update({ 0, 0, GetWidth(), GetHeight() });
				}
				else {
					Win32Cache.Repaint = false;
				}
			}

			if (Win32Cache.UserSetGeomtery) {
				Win32Cache.UserSetGeomtery = false;

				ObjectVisual.Rectangle.Right = Win32Cache.UserSetWidth;
				ObjectVisual.Rectangle.Bottom = Win32Cache.UserSetHeight;

				Update(ObjectVisual.Rectangle);

				Resized.Emit(GetWidth(), GetHeight());

				RECT NewRect = { 0, 0, Win32Cache.UserSetWidth, Win32Cache.UserSetHeight };
				Direct2DRender->DirectXDCTarget.Get()->BindDC(GetDC(WindowHandle), &NewRect);
			}

			if (!RepaintMessages.empty()) {
				RepaintAllocator = new VKits::VAllocator;

				Canvas			 = new VCanvasPainter(GetWidth(), GetHeight(),
					VRenderHandle(nullptr, nullptr, nullptr, Direct2DRender->GetDirectXRenderTarget(), RepaintAllocator));
				Canvas->BeginDraw();
				Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
				if (!WindowConfig.EnableRadius) {
					for (auto& Message : RepaintMessages) {
						OnPaint(Canvas, Message->DirtyRectangle);
						SendMessageToChild(Message, false);

						delete Message;
					}
				}
				else {
					for (auto& Message : RepaintMessages) {
						delete Message;
					}
					RepaintMessages.clear();

					VRepaintMessage RepaintAll(CallWidgetGetHWND(), { 0, 0, GetWidth(), GetHeight() });

					OnPaint(Canvas, RepaintAll.DirtyRectangle);
					SendMessageToChild(&RepaintAll, false);
				}

				Canvas->EndDraw();

				RepaintMessages.clear();
				if (WindowConfig.EnableRadius) {
					HDC     WindowDC = GetDC(WindowHandle);
					HBITMAP BorderImage = CreateCompatibleBitmap(WindowDC, GetWidth(), GetHeight());
					HDC     BorderImageDC = CreateCompatibleDC(WindowDC);
					HBITMAP OldBitmap = (HBITMAP)SelectObject(BorderImageDC, BorderImage);

					auto Property = D2D1::RenderTargetProperties(
						D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE,
						D2D1::PixelFormat(
							DXGI_FORMAT_B8G8R8A8_UNORM,
							D2D1_ALPHA_MODE_PREMULTIPLIED
						), 0.0, 0.0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT
					);
					ID2D1DCRenderTarget* DCRenderTarget;
					HRESULT Result = VDirectXD2DFactory.GetInstance()->CreateDCRenderTarget(
						&Property,
						&DCRenderTarget
					);
					RECT WindowRect = { 0, 0, GetWidth(), GetHeight() };

					ID2D1Bitmap* Bitmap;
					Canvas->GetDXObject()->GetBitmap(&Bitmap);
					VBitmapBrush BitmapBrush(Bitmap, CallWidgetGetRenderHandle());

					DCRenderTarget->BindDC(BorderImageDC, &WindowRect);

					DCRenderTarget->BeginDraw();

					DCRenderTarget->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
					DCRenderTarget->FillRoundedRectangle(D2D1_ROUNDED_RECT{
							{0, 0, static_cast<float>(GetWidth() - 1), static_cast<float>(GetHeight() - 1)},
							static_cast<float>(WindowConfig.BorderRadius.X),
							static_cast<float>(WindowConfig.BorderRadius.Y) }, BitmapBrush.GetDxBrush());

					DCRenderTarget->EndDraw();

					BLENDFUNCTION BlendFN = { 0 };
					BlendFN.BlendOp = AC_SRC_OVER;
					BlendFN.SourceConstantAlpha = 255;
					BlendFN.AlphaFormat = AC_SRC_ALPHA;

					POINT	SourcePoint = { 0, 0 };
					SIZE	WindowSize = { GetWidth(), GetHeight() };
					UpdateLayeredWindow(WindowHandle, WindowDC, NULL, &WindowSize, BorderImageDC,
						&SourcePoint, NULL, &BlendFN, ULW_ALPHA);

					ReleaseDC(WindowHandle, WindowDC);

					Bitmap->Release();
					DCRenderTarget->Release();

					SelectObject(BorderImageDC, OldBitmap);
					DeleteObject(BorderImage);
					ReleaseDC(WindowHandle, WindowDC);
					ReleaseDC(WindowHandle, BorderImageDC);
				}
				else {
					BufferPainter->BeginDraw();
					BufferPainter->DrawCanvas(GetRegion(), Canvas, GetRegion(), 1.f);
					BufferPainter->EndDraw();
				}

				delete Canvas;
				delete RepaintAllocator;

				RepaintAllocator = nullptr;
			}
		}
	}

	void VWidget::SetTitle(const std::wstring& WindowText) {
		Theme->WindowPlaneText = WindowText;
		SetWindowTextW(WindowHandle, Theme->WindowPlaneText.c_str());
	}
	void VWidget::SetBackgroundColor(const VColor& Color) {
		Theme->BackgroundColor = Color;

		Update({ 0, 0, GetWidth(), GetHeight() });
	}
	void VWidget::SetMaxSize(const VGeometry& MaxSize) {
		WindowConfig.UseMaxMinSize = true;
		WindowConfig.WindowMaxSize = MaxSize;
	}
	void VWidget::SetMiniSize(const VGeometry& MiniSize) {
		WindowConfig.UseMaxMinSize = true;
		WindowConfig.WindowMiniSize = MiniSize;
	}
	void VWidget::SetFrameless(const bool& FramelessStatus) {
		if (FramelessStatus) {
			WindowConfig.InFrameless = true;

			SetWindowLongPtr(GetLocalWinId(), GWL_STYLE, GetWindowLongPtr(GetLocalWinId(), GWL_STYLE) & ~(WS_THICKFRAME | WS_SIZEBOX | WS_CAPTION));

			RECT WindowRect;
			GetWindowRect(GetLocalWinId(), &WindowRect);
			MoveWindow(GetLocalWinId(), WindowRect.left, WindowRect.top, GetWidth(), GetHeight(), FALSE);
		}
		else {
			WindowConfig.InFrameless = false;

			SetWindowLongPtr(GetLocalWinId(), GWL_STYLE, 349110272ll);
		}
	}
	void VWidget::SetSizable(const bool& Sizable) {
		WindowConfig.Sizable = Sizable;

		if (Sizable) {
			SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) | (WS_MAXIMIZEBOX));
			SetWindowLong(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) | WS_THICKFRAME));
		}
		else {
			SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_MAXIMIZEBOX));
			SetWindowLong(WindowHandle, GWL_STYLE, (GetWindowLong(WindowHandle, GWL_STYLE) ^ (WS_THICKFRAME)));
		}
	}
	void VWidget::SetRadius(VPoint Radius) {
		if (Radius == VPoint{ 0, 0 }) {
			WindowConfig.EnableRadius = false;
			WindowConfig.BorderRadius = Radius;

			WindowConfig.InFrameless = false;
		}
		else {
			SetWindowLongPtr(WindowHandle, GWL_EXSTYLE, WS_EX_LAYERED);

			WindowConfig.EnableRadius = true;
			WindowConfig.BorderRadius = Radius;

			WindowConfig.InFrameless = true;
		}

		Update({ 0, 0, GetWidth(), GetHeight() });
	}
	void VWidget::Resize(const int& Width, const int& Height) {
		VUIObject::Resize(Width, Height);

		if (Width > 0 && Height > 0) {
			RECT Rect;
			GetWindowRect(WindowHandle, &Rect);

			MoveWindow(WindowHandle, Rect.left, Rect.right, Width, Height, FALSE);
		}
	}

	HWND VWidget::GetLocalWinId() {
		return WindowHandle;
	}
}

VLIB_END_NAMESPACE
