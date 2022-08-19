// License(MIT)
// Athuor: Margoo
// The application
#pragma once

#include "../widget/vmessage.h"
#include "../uibasic/vtimer.h"
#include "../uibasic/vuitheme.h"
#include "vuiobject.h"

#include <graphics.h>

VLIB_BEGIN_NAMESPACE

namespace Core {

class VApplication;

extern VApplication* VLib_Application;

class VApplication : public VUIObject {
 private:
     std::vector<VBasicUITheme*> ThemeList;

     void InitTheme();

 protected:
     std::vector<VBasicUITheme*> GetApplicationTheme() override;

 private:
     static VMessage* PatchEvent();
     void      ProcessEvent(VMessage* PatchedMessage);

 public:
     VApplication();

 public:
     static VApplication* GetInstance() {
         return VLib_Application;
     }

     bool IsApplication() override {
         return true;
     }

 public:
     int Exec();

 public:
     void SetTheme(VBasicUITheme* Theme);
};

}

VLIB_END_NAMESPACE