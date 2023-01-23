#pragma once

#include "../object/vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

class VVisualView : public VUIObject
{
public:
	VVisualView(VUIObject *Parent) : VUIObject(Parent)
	{
	}
};

class VCircleView : public VUIObject
{
public:
	VCircleView(VUIObject *Parent);

public:
	bool OnMessageTrigger(VRepaintMessage *RepaintMessage) override;
};

class VPolygonView : public VUIObject
{
private:
	std::vector<VPointF> PolygonPoint;

public:
	void AddPoint(const VPointF &Point);

public:
	VPolygonView(VUIObject *Parent);

public:
	bool OnMessageTrigger(VRepaintMessage *RepaintMessage) override;
};

} // namespace Core

VLIB_END_NAMESPACE