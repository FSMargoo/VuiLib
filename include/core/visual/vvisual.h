#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

class VVisualView : public VUIObject {
public:
    VVisualView(VUIObject* Parent) : VUIObject(Parent) { }
};



}

VLIB_END_NAMESPACE