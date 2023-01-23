#include "../../../include/core/control/vtitlebar.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
void VWidgetTitleBar::WidgetOnSize(const int &Width, const int &Height)
{
	Resize(Width, GetSystemMetrics(SM_CYCAPTION));
}
VWidgetTitleBar::VWidgetTitleBar(VMainWindow *MainWindow) : VUIObject(MainWindow)
{
	WidgetOnSize(MainWindow->GetWidth(), 0);

	InDrag = false;

	MainWindow->Resized.Connect(this, &VWidgetTitleBar::WidgetOnSize);
}
void VWidgetTitleBar::OnMessage(VMessage *Message)
{
	if (Message->GetType() == VMessageType::MouseClickedMessage)
	{
		auto MouseClickedMessage = static_cast<VMouseClickedMessage *>(Message);

		if (MouseClickedMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			if (MouseClickedMessage->ClickedMethod == Down && MouseClickedMessage->ClickedKey == Left)
			{
				InDrag = true;
			}
			if (MouseClickedMessage->ClickedMethod == Up && MouseClickedMessage->ClickedKey == Left)
			{
				InDrag = false;
			}
		}
	}
	if (Message->GetType() == VMessageType::MouseMoveMessage && InDrag)
	{
		auto MouseMovedMessage = static_cast<VMouseClickedMessage *>(Message);

		PostMessage(CallWidgetGetHWND(), WM_NCLBUTTONDOWN, HTCAPTION,
					MAKELPARAM(MouseMovedMessage->MousePosition.X, MouseMovedMessage->MousePosition.Y));
	}
}
} // namespace Core

VLIB_END_NAMESPACE