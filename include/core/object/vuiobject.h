/*
 * File name	: vuiobject.h
 * Author		: Margoo
 * Date			: 11/21/2022
 * Description	: This head defines the basic class in vuilib - VUIObject
*/
#pragma once

#include "../vbasiccore.h"
#include "../widget/vmessage.h"
#include "../render/vpainter.h"
#include "../uibasic/vuitheme.h"
#include "../signal/vsignal.h"

#include <vector>

VLIB_BEGIN_NAMESPACE

namespace Core {
    class VUIObject;
    
    /*
     * VUIObjectKernel structure:
     *  Description : This structure mainly log the visually irrelevant properties of a object
    */
    struct VUIObjectKernel {
        /*
         * Parent variable:
         *  Description : Tag the object's parent control
        */
        VUIObject*               Parent;
        /*
         * ParentID variable:
         *  Description : The id of this object (Relative to parent's vector container size)
         *  Tips        : This variable isn't unchanged, it will change dynamically,
         *                when the parent's ChildObjectContainer variable changed, generally, 
         *                its value changes, except for new objects were queued
        */
        size_t                   ParentID;
                                 
        /*
         * GlobalID variable:
         *  Description : The global id of this object, it is a globally unique id,
         *  Tips        : It is same as ParentID, the GlobalID isn't unchanged, it will also change,
         *                as the global object's layout has changed
        */
        std::wstring             GlobalID;


        /*
         * ChildObjectContainer variable:
         *  Description : The container to save object's children's pointer
        */
        std::vector<VUIObject* > ChildObjectContainer;

        VUIObjectKernel();
    };
    
    /*
     * VUIObjectShadowProperty structure:
     *  Description : The object's shadow's property
    */
    struct VUIObjectShadowProperty {
        /*
         * Color variable:
         *  Description : The color of shadow
        */
        VColor Color;
        /*
         * Offset variable:
         *  Description : The shadow position offset value
         *  Tips        : The Offset's value mean the shadow effect image's render position,
         *                attention, this position is relative to the position of object
        */
        VPoint Offset;

        /*
         * Radius variable:
         *  Description : The radius of shadow blur
         *  Tips        : This value can't be a invalid value like negative number
        */
        float  Radius;

        /*
         * EnableStatus variable:
         *  Description : The availability of shadow style
         *  Tips        : If the value is true, the shadow will be enabled,
         *                if the value is false, the shadow will be disabled
        */
        bool   EnableStatus;

        VUIObjectShadowProperty();
    };
    
    /*
     * VUIObjectUIStats enum:
     *  Description : This enum mark the local object's status on surface
    */
    enum VUIObjectUIStats {
        /*
         * The object is invisible
        */
        Hidden,
        /*
         * The object on mouse's click
        */
        OnClicked,
        /*
         * The object is on mouse's focus (the mouse's left key is holding)
        */
        OnFocus,
        /*
         * The object in a normal status (standby status)
        */
        Normal
    };
    
    /*
     * VUIObjectSurface:
     *  Description : The object's visual element's collection
    */
    struct VUIObjectSurface {
        /*
         * Rectangle variable:
         *  Description : This variable defines the object's area on the surface
        */
        VRect                   Rectangle;
        /*
         * Shadow variable:
         *  Description : The shadow property
        */
        VUIObjectShadowProperty Shadow;
    
        /*
         * Stats variable:
         *  Description : The object's status of object
        */
        VUIObjectUIStats        Stats;

        /*
         * Transparency variable:
         *  Description : The transparency of the object, if the transparency of a VUIObject is 0,
         *                it won't take part in any message's dealing which will cause repaint event
        */
        float                   Transparency;

        VUIObjectSurface();
    };
    
    /*
     * VUIObject class:
     *  Description : The basic control object of every control objects
    */
    class VUIObject {
     protected:
         /*
          * ObjectKernel variable:
          *     Description : The kernel configuration of object
         */
         VUIObjectKernel  ObjectKernel;
         /*
          * ObjectVisual variable:
          *     Description : The visual property of object
         */
         VUIObjectSurface ObjectVisual;
     
     public:
         /*
          * GetParent function:
          *     Description : Get parent object of this object
         */
         VUIObject*    GetParent() const;
     
         /*
          * GetRegion function:
          *     Description : Get the region on the UI surface of this object
         */
         virtual VRect GetRegion();
         /*
          * GetRegion function:
          *     Description : Get the region on the UI surface of the child objects
          *     Tips        : The return value of this function will effect the paint of child objects
         */
         virtual VRect GetChildrenVisualRegion();
         /*
          * GetLocalWinId function:
          *     Description : Get the window handle of current window
          *     Tips        : It will return the window handle of this object's widget parent
         */
         virtual HWND  GetLocalWinId();
     
     protected:
         /*
          * GetApplicationTheme function:
          *     Description : Get the theme from application
         */
         virtual std::vector<VBasicUITheme *> GetApplicationTheme();
     
     public:
         /*
          * IsAnimation function:
          *     Description : Is this object a animation
         */
         virtual bool IsAnimation();
         /*
          * IsAnimation function:
          *     Description : Is this object a application
         */
         virtual bool IsApplication();
         /*
          * IsWidget:
          *     Description : Is the object a widget
         */
         virtual bool IsWidget() const;

     public:
         /*
          * GetChildObjectByPosition:
          *     Description : Get the child object by the position in memory
         */
         VUIObject* GetChildObjectByPosition(const int& Position);

     protected:
         /*
          * GetParentCanvas function:
          *     Description : Get the canvas of parent
         */
         VCanvasPainter* GetParentCanvas();
         /*
          * GetWidgetCanvas function:
          *     Description : Get the parent widget's canvas
         */
         VCanvasPainter* GetWidgetCanvas();
         /*
          * GetWidgetAllocator function:
          *     Description : Get the allocator of widget
         */
         virtual VKits::VAllocator* GetWidgetAllocator() const;
     
         /*
          * GetOriginX function:
          *     Description : Get the x relative to the widget
         */
         int             GetOriginX(const int& X = 0);
         /*
          * GetOriginX function:
          *     Description : Get the y relative to the widget
         */
         int             GetOriginY(const int& Y = 0);
     
         /*
          * GetTargetTheme function:
          *     Description : Get the specified theme from application
         */
         VBasicUITheme*  GetTargetTheme(VUIThemeType ThemeType);
     
     public:
         /*
          * GetTransparency function:
          *     Description : Get the transparency of this object
         */
         float           GetTransparency() const;
         /*
          * SetShadowStats function:
          *     Description : Set the shadow stats of this object
          *     Tips        : If the stats value is true, it will enable shadow effect,
          *                   if the value is false, it will disable shadow effect
         */
         void SetShadowStats(const bool& Stats);
         /*
          * SetShadowColor function:
          *     Description : Set the shadow color of this object
         */
         void SetShadowColor(const VColor& Color);
         /*
          * SetShadowRadius function:
          *     Description : Set the shadow radius of this object
         */
         void SetShadowRadius(const float& Radius);
     
     public:
         /*
          * InFocus signal:
          *     Description : This signal will be triggered, if the object got mouse's focus
         */
         VSignal<> InFocus;
         /*
          * LostFocus signal:
          *     Description ; If the object lost the mouse's focus, this signal will be triggered
         */
         VSignal<> LostFocus;
         /*
          * Resized signal:
          *     Description : If the object resized, this signal will be triggered
         */
         VSignal<const int&, const int&> Resized;
         /*
          * Moved signal:
          *     Description : If the object moved, this signal will be triggered
         */
         VSignal<const int&, const int&> Moved;
     
     private:
         /*
          * GeneratedGlobalID function:
          *     Description : Generated a global id for this object
         */
         std::wstring GeneratedGlobalID(std::wstring TargetID = L"");
     
     protected:
         /*
          * RestoreMousePosition function:
          *     Description : Restore the mouse message's parameter to a point which is relative to the parent
         */
         virtual void RestoreMousePosition(VPoint* MousePosition);
     
     protected:
         /*
          * CallWidgetSetIME function:
          *     Description : The function will call the widget to set the IME input box's position
         */
         virtual void         CallWidgetSetIME(const int& X, const int& Y);
         /*
          * CallWidgetSetFocusID function:
          *     Description : The function will call the widget to set the global focus object's id
         */
         virtual void         CallWidgetSetFocusID(const std::wstring& ObjectID);
         /*
          * CallWidgetSetFocusID function:
          *     Description : The function will call the widget to get the global focus object's id
         */
         virtual std::wstring CallWidgetGetFocusID() const;
         /*
          * CallWidgetSetFocusID function:
          *     Description : The function will call the widget to lock the global focus object's id
          *     Tips        : If this function be called, the object's focus status won't be changed
          *                   though whatever operation user do
         */
         virtual void         CallWidgetLockFocusID();
         /*
          * CallWidgetUnlockFocusID function:
          *     Description : The function will call the widget to unlock the global focus object's id
         */
         virtual void         CallWidgetUnlockFocusID();
         /*
          * CallWidgetGetLockingStatus function:
          *     Description : The function will call the widget to get the global id's locking status
         */
         virtual bool         CallWidgetGetLockingStatus();
         /*
          * CallWidgetSendMessage function:
          *     Description : The function will call the widget send message to every object
         */
         virtual void         CallWidgetSendMessage(VMessage* Message);
         /*
          * CallWidgetGetHWND function:
          *     Description : The function will get the win32 window handle of parent widget
         */
         virtual HWND         CallWidgetGetHWND();
         /*
          * CallWidgetSetIMEFontStyle:
          *     Description : Set the IME font style by widget
         */
         virtual void         CallWidgetSetIMEFontStyle(const LOGFONT& LogfontStyle);
         /*
          * GetWidgetIMEFontStlye:
          *     Description : Get the IME font style of widget
         */
         virtual LOGFONT      GetWidgetIMEFontStlye() const;
     
     public:
         /*
          * CallWidgetGetRenderHandle function:
          *     Description : The function will call the widget to get the render handle
         */
         Core::VRenderHandle CallWidgetGetRenderHandle() const;
         /*
          * CallWidgetGetStaticRenderHandle function:
          *     Description : The function will call the widget to get the render handle
         */
         Core::VRenderHandle CallWidgetGetStaticRenderHandle() const;
         /*
          * CallWidgetGetDCRenderTarget function:
          *     Description : The function will call the widget to get the render handle
         */
         virtual Core::VRenderHandle CallWidgetGetDCRenderTarget() const;
     
     protected:
         /*
          * CallWidgetGetCanvas function:
          *     Description : The function will call the widget to get the canvas of parent widget
         */
         virtual Core::VCanvasPainter* CallWidgetGetCanvas();
     
     protected:
         /*
          * EditChildMessage function:
          *     Description : If a message will be sent to child object, this function will be called
          *                   to do any operation of the message
         */
         virtual void EditChildMessage(VMessage *Message) {  }
     
     protected:
         /*
          * HelperGetSourceRect function:
          *     Description : Get the source rectangle area of this object
          *     Tips        : This rectangle area won't include region area
         */
         VRect HelperGetSourceRect();
     
     public:
         /*
          * Update function:
          *     Description : Update the target area of widget
         */
         virtual void Update(VRect UpdateRect);
         /*
          * Update function:
          *     Description : Update the object area of widget
         */
         void Update();
     
     public:
         /*
          * OnPaint function:
          *     Description : This function is provided to paint a object on surface
         */
         virtual void OnPaint(VCanvasPainter*) {  }
         /*
          * EditCanvas function:
          *     Description : This function is provided to do some operation of a canvas before painting
         */
         virtual void EditCanvas(VCanvasPainter*) {  }
         /*
          * OnMessageTrigger function:
          *     Description : When a repaint message triggered, this function will be called
         */
         virtual bool OnMessageTrigger(VRepaintMessage* Message);
     
     public:
         /*
          * GotMouseFocus function:
          *     Description : If the object got the focus from mouse, this message will be triggered
         */
         virtual void GotMouseFocus() {  }
         /*
          * GotMouseFocus function:
          *     Description : If the object got the focus from mouse, this message will be triggered
         */
         virtual void LostMouseFocus() {  }
         /*
          * MouseLeftClicked function:
          *     Description : If the mouse's left key clicked, the function will be called
         */
         virtual void MouseLeftClicked(const VMouseClickedFlag&) {  }
         /*
          * MouseRightClicked function:
          *     Description : If the mouse's right key clicked, the function will be called
         */
         virtual void MouseRightClicked(const VMouseClickedFlag&) {  }
         /*
          * MouseRightClicked function:
          *     Description : If the mouse's right key clicked, the function will be called
         */
         virtual void MouseMiddleClicked(const VMouseClickedFlag&) {  }
         /*
          * MouseRightClicked function:
          *     Description : If the mouse's middle key dragged, the function will be called
         */
         virtual bool MouseMiddleDragged(const short&)       { return false; }
         /*
          * CheckDown function:
          *     Description : If a key was pressed (down or up), this function will be called
          *     Tips        : If the function's return value is false, this object's child objects won't
          *                   receive the VKeyClickedMessage
         */
         virtual bool CheckDown(VKeyClickedMessage*)         { return false; }
         /*
          * IMECharInputed function:
          *     Description : If a IME message was triggered, the function will be called
         */
         virtual bool IMECharInputed(const wchar_t& Message) { return false; }
     
     public:
         /*
          * CheckFrame function:
          *     Description : This function will be called every frame to
          *                   check object's status or something
         */
         virtual void CheckFrame() {}
         /*
          * CheckAllFrame function:
          *     Description : Call this object and it's child objects to check frame
         */
         void         CheckAllFrame(bool RecursionChildren = false);
     
     public:
         /*
          * GetX function:
          *     Description : Get the x property of this object
         */
         int GetX()      const;
         /*
          * GetY function:
          *     Description : Get the y property of this object
         */
         int GetY()      const;
         /*
          * GetWidth function:
          *     Description : Get the width property of this object
         */
         int GetWidth()  const;
         /*
          * GetHeight function:
          *     Description : Get the height property of this object
         */
         int GetHeight() const;
     
     public:
         /*
          * Resize function:
          *     Description : Resize the object
         */
         virtual void Resize(const int& Width, const int& Height);
         /*
          * Move function:
          *     Description : Move the object
         */
         virtual void Move(const int& X, const int& Y);
         /*
          * Resize function:
          *     Description : Resize the object
         */
         void Resize(const VGeometry& Size);
         /*
          * Move function:
          *     Description : Move the object
         */
         void Move(const VPoint& Point);
     
     public:
         /*
          * SetParent function:
          *     Description : Set the parent of this object
         */
         void SetParent(VUIObject* Parent);
     
     public:
         /*
          * SetTransparency function:
          *     Description : Set the transparency of this object
         */
         void  SetTransparency(const float& Transparency);
     
     public:
         /*
          * OnMessage function:
          *     Description : When a object received a message, this function will be called
         */
         virtual void OnMessage(VMessage* Message) {  }
         /*
          * CatchMessage function:
          *     Description : When a object received a message, this function will be called
         */
         virtual bool CatchMessage(VMessage* Message) { return false; }
         /*
          * CheckMousePosition function:
          *     Description : Do more detailed judgment if object need
         */
         virtual bool CheckMousePosition(const VPoint& Point) { return true; }
         /*
          * SysProcessMessage function:
          *     Description : The native message processed function
         */
         bool SysProcessMessage(VMessage* Message);
         /*
          * CheckQuitWindowMessage function:
          *     Description : When the user want to close the widget,
          *                   this function will be called, if this function's
          *                   return value is true, the window will be closed,
          *                   or the window won't close
         */
         virtual bool CheckQuitWindowMessage(VMessage* Message) { return false; }
     
     protected:
         /*
          * SendMessageToChlid function:
          *     Description : Send a message to child object
          *     Tips        : If the BreakWhenMeetTrue value is true, the message's sending will be
          *                   break when a child object's SysProcessMessage function return true
         */
         bool SendMessageToChild(VMessage* Message, bool BreakWhenMeetTrue = true);
         /*
          * CheckUIFocusStatus function:
          *     Description : Check the object's focus status by a specified point with a message
         */
         bool CheckUIFocusStatus(const VPoint& MousePosition, VMessage* SourceMessage);
         /*
          * CheckElementUIStatus function:
          *     Description : Check the object's element status by a specified message
         */
         bool CheckElementUIStatus(VMessage* SourceMessage);

     public:
         const std::vector<VUIObject* > GetChildLayout();
         VCanvasPainter*                GetCanvas();

     public:
         /*
          * Hide function:
          *     Description : Hide the object on surface
         */
         virtual void Hide();
         /*
          * Hide function:
          *     Description : Show the object on surface
         */
         virtual void Show();
     
     public:
         VUIObject(VUIObject* Parent);
         ~VUIObject();

     protected:
         VCanvasPainter* Canvas;
    };

}

VLIB_END_NAMESPACE