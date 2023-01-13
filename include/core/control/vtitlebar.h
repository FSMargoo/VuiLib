#pragma once

#include "../widget/vwidget.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
	class VWidgetTitleBar : public VUIObject {
	private:
		void WidgetOnSize(const int& Width, const int& Height);

	public:
		VWidgetTitleBar(VMainWindow* MainWindow);

	public:
		void OnMessage(VMessage* Message);

	private:
		bool InDrag;
	};
}

VLIB_END_NAMESPACE