#pragma once

#include "core/control/vtextlabel.h"

namespace VTeX
{
class VTeXLabel : public Core::VUIObject
{
public:
	VTeXLabel(Core::VUIObject *Parent);
	VTeXLabel(Core::VUIObject *Parent, const VString &TeXCode);
};
} // namespace VTeX