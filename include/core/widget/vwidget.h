/*
 * File name	: vapplicaiton.h
 * Author		: Margoo
 * Date			: 11/22/2022
 * Description	: The widget class in vuilib
*/
#pragma once

#include "../object/vapplication.h"
#include "../control/vswitchgroup.h"
#include "vprivatewidget.h"

#include <map>

VLIB_BEGIN_NAMESPACE

namespace Core {
    /*
     * VWin32StatusCache structure:
     *  Description : The win32 message's cache
    */
    struct VWin32StatusCache {
        int  UserSetWidth  = 0;
        int  UserSetHeight = 0;
    
        bool UserSetGeomtery = false;
        bool Repaint         = false;
    };
    
    /*
     * VMainWindow Description:
     *  Description : The main widget of vuilib
    */
    class VMainWindow : public VUIObject {
     private:
         /*
          * DefaultOnQuitFunction function:
          *     Description : The default on quit event's responsive function
          *     Tips        : It will return true directly in every condition
         */
         bool DefaultOnQuitFunction();
    
     public:
         /*
          * SetQuitEvent function:
          *     Description : Set the on quit event's responsive function
         */
         void SetQuitEvent(const std::function<bool()>& QEventFunction);
    
     public:
         /*
          * IsWidget override function:
          *     Description : Mark the object as widget
         */
         bool IsWidget() const override;
    
     private:
         /*
          * InitKernel:
          *     Description : Initialize the kernel of widget
         */
         void InitKernel();
         /*
          * InitKernel:
          *     Description : Create the win32 widget
         */
         void InitWindow();
         /*
          * InitIME:
          *     Description : Initialize the IME input box style
         */
         void InitIME();
    
     private:
         /*
          * Win32ThreadResized function:
          *     Description : When the widget got the message from win32 message loop,
          *                   the WNDPROC function will call this function through std::function
         */
         void Win32ThreadResized(int Width, int Height);
         /*
          * Win32ThreadRepaint function:
          *     Description : If the widget received the WM_PAINT message, this function will be
          *                   called from WNDPROC through std::function
         */
         void Win32ThreadRepaint();
         void Win32LoseFocus();
    
     public:
         void                CallWidgetSetIME(const int& X, const int& Y) override;
         void                CallWidgetSetFocusID(const std::wstring& ObjectID) override;
         std::wstring        CallWidgetGetFocusID() const override;
         void                CallWidgetLockFocusID() override;
         void                CallWidgetUnlockFocusID() override;
         bool                CallWidgetGetLockingStatus() override;
         void                CallWidgetSendMessage(VMessage *Message) override;
         void                CallWidgetSetIMEFontStyle(const LOGFONT& LogfontStyle) override;
         LOGFONT             GetWidgetIMEFontStlye() const override;
         HWND                CallWidgetGetHWND() override;
         VCanvasPainter*     CallWidgetGetCanvas() override;
         Core::VRenderHandle CallWidgetGetDCRenderTarget() const override;
         VKits::VAllocator*  GetWidgetAllocator() const override;
    
         bool                CheckQuitWindowMessage(VMessage* Message) override;
    
    public:
        HWND GetLocalWinId() override;
    
     public:
         void Update(VRect UpdateRect) override;
    
     public:
         void OnPaint(VCanvasPainter* Canvas, const VRect& Rect);
    
     public:
         void SetTitle(const std::wstring& WindowText);
         void SetBackgroundColor(const VColor& Color);
         void SetMaxSize(const VGeometry& MaxSize);
         void SetMiniSize(const VGeometry& MiniSize);
         void SetSizable(const bool& Sizble);
    
         void SetRadius(VPoint Radius = { 0, 0 });
    
         void SetFrameless(const bool& FramelessStatus);
    
         void Resize(const int& Width, const int& Height) override;
    
     public:
         void Show() override;
         void Hide() override;
    
     public:
         void CheckFrame() override;

     public:
         VMainWindow(const int& Width, const int& Height, VApplication* Parent, const bool& Sizble = true);
         ~VMainWindow();

     private:
         std::vector<VRepaintMessage*> RepaintMessages;
         VPainter*                     BufferPainter;
         HWND                          WindowHandle;

      private:
         VBasicTimer                   FpsTimer;
                                       
         VMainWindowTheme*             Theme;
         VWin32ThreadPipe              WindowConfig;
         VWin32StatusCache             Win32Cache;
                                       
         VDCRender*                    Direct2DRender;
                                       
         VKits::VAllocator*            RepaintAllocator;
    };
    
    class VWidget : public VUIObject {    
     public:
         bool IsWidget() const override {
             return true;
         }
    
     private:
         void InitKernel();
         void InitWindow(const std::wstring& ClassName, HWND ParentWindow);
         void InitIME();
    
     private:
         void Win32ThreadResized(int Width, int Height);
         void Win32ThreadRepaint();
         void Win32LoseFocus();
    
     private:
        bool OnQuit() { return false; }
    
     public:
        void SetQuitEvent(const std::function<bool()>& QEventFunction);
    
     public:
         VWidget(const int& Width, const int& Height, VApplication* Parent, const std::wstring& ClassName, const bool& Sizble = true);
         VWidget(const int& Width, const int& Height, VMainWindow* Parent, const std::wstring& ClassName, const bool& Sizble = true);
         VWidget(const int& Width, const int& Height, VWidget* Parent, const std::wstring& ClassName, const bool& Sizble = true);
         ~VWidget();
    
     public:
         void CallWidgetSetIME(const int& X, const int& Y) override;
         void CallWidgetSetFocusID(const std::wstring& ObjectID) override;
         std::wstring CallWidgetGetFocusID() const override;
         void CallWidgetLockFocusID() override;
         void CallWidgetUnlockFocusID() override;
         bool CallWidgetGetLockingStatus() override;
         void CallWidgetSendMessage(VMessage *Message) override;
         HWND CallWidgetGetHWND() override;
         VCanvasPainter* CallWidgetGetCanvas() override;
    
         bool CheckQuitWindowMessage(VMessage* Message) override;

    public:
        VKits::VAllocator* GetWidgetAllocator() const override;
    
     public:
         HWND GetLocalWinId() override;
    
     public:
         void Update(VRect UpdateRect) override;
    
     public:
         void OnPaint(VCanvasPainter* Canvas, const VRect& Rect);
    
     public:
         void Show() override;
         void Hide() override;
    
     public:
         void SetTitle(const std::wstring& WindowText);
         void SetBackgroundColor(const VColor& Color);
         void SetMaxSize(const VGeometry& MaxSize);
         void SetMiniSize(const VGeometry& MiniSize);
         void SetSizable(const bool& Sizable);
    
         void SetRadius(VPoint Radius = { 0, 0 });
    
         void SetFrameless(const bool& FramelessStatus);
    
         void Resize(const int& Width, const int& Height) override;
    
     public:
         void CheckFrame() override;

     private:
         std::vector<VRepaintMessage*> RepaintMessages;
         VPainter* BufferPainter;
         HWND                          WindowHandle;
     
     private:
         VBasicTimer                   FpsTimer;
     
         VMainWindowTheme*             Theme;
         VWin32ThreadPipe              WindowConfig;
         VWin32StatusCache             Win32Cache;
     
         VDCRender*                    Direct2DRender;

         VKits::VAllocator*            RepaintAllocator;
    };
}

VLIB_END_NAMESPACE
