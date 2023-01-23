#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VFakeCaption : public VUIObject
{
public:
	VFakeCaption(const int &Width, const int &Height, VUIObject *Parent);
	VFakeCaption(VUIObject *Parent);

public:
	void OnMessage(VMessage *Message) override;
};

} // namespace Core

VLIB_END_NAMESPACE