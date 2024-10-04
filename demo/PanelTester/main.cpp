/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file main.cpp
 * \brief The demo to use the panel control
 */

/**
 * \file main.cpp
 * \brief The demo to create a simple widget in VUILib
 */

#include <include/control/shape/vRectangleControl.h>
#include <include/layout/vPanelLayout.h>
#include <include/layout/vStackPanelLayout.h>
#include <include/widget/vMainWindow.h>

void CreatePanelViewer(VApplication *Target) {
	auto window = new VMainWindow(Target, 640, 480, "Panel Viewer");

	auto panel = new VPanel(window);

	auto rectangleBackground = new VRectangleControl(panel);
	auto rectangle1			 = new VRectangleControl(panel);
	auto rectangle2			 = new VRectangleControl(panel);
	auto rectangle3			 = new VRectangleControl(panel);
	auto rectangle4			 = new VRectangleControl(panel);

	auto property1 = std::make_unique<VStringProperty>("left");
	rectangle1->RegisterProperty("panel.align", std::move(property1));
	auto property2 = std::make_unique<VStringProperty>("right");
	rectangle2->RegisterProperty("panel.align", std::move(property2));
	auto property3 = std::make_unique<VStringProperty>("top");
	rectangle3->RegisterProperty("panel.align", std::move(property3));
	auto property4 = std::make_unique<VStringProperty>("bottom");
	rectangle4->RegisterProperty("panel.align", std::move(property4));
	auto property5 = std::make_unique<VStringProperty>("center");
	rectangleBackground->RegisterProperty("panel.align", std::move(property5));

	rectangle1->SetOpacity(0.5);
	rectangle1->SetBackgroundColor(SK_ColorBLUE);
	rectangle2->SetBackgroundColor(SK_ColorCYAN);
	rectangle2->SetOpacity(0.5);
	rectangle3->SetBackgroundColor(SK_ColorGREEN);
	rectangle3->SetOpacity(0.5);
	rectangle4->SetOpacity(0.5);

	rectangleBackground->Resize(100, 100);
	rectangleBackground->LockSizing();

	panel->Arrange();

	window->Show();
}
void CreateStackPanelViewer(VApplication *Target) {
	auto window = new VWidget(Target, 640, 480, "Stack Panel Viewer");

	auto panel = new VStackPanel(window);

	auto rectangleBackground = new VRectangleControl(panel);
	auto rectangle1			 = new VRectangleControl(400, 60, panel);
	auto rectangle2			 = new VRectangleControl(panel);
	auto rectangle3			 = new VRectangleControl(panel);
	auto rectangle4			 = new VRectangleControl(panel);

	auto property1 = std::make_unique<VStringProperty>("left");
	rectangle1->RegisterProperty("stack.align", std::move(property1));
	auto property2 = std::make_unique<VStringProperty>("right");
	auto propertyMargin = std::make_unique<VIntProperty>(20);
	rectangle2->RegisterProperty("stack.align", std::move(property2));
	rectangle2->RegisterProperty("stack.margin", std::move(propertyMargin));
	auto property3 = std::make_unique<VStringProperty>("center");
	rectangle3->RegisterProperty("stack.align", std::move(property3));
	auto property4 = std::make_unique<VStringProperty>("right");
	auto propertyTopMargin = std::make_unique<VIntProperty>(12);
	rectangle4->RegisterProperty("stack.align", std::move(property4));
	rectangle4->RegisterProperty("stack.margin.top", std::move(propertyTopMargin));
	auto property5 = std::make_unique<VStringProperty>("center");
	rectangleBackground->RegisterProperty("stack.align", std::move(property5));

	rectangle1->SetOpacity(0.5);
	rectangle1->SetBackgroundColor(SK_ColorMAGENTA);
	rectangle2->SetBackgroundColor(SK_ColorCYAN);
	rectangle2->SetOpacity(0.5);
	rectangle3->SetBackgroundColor(SK_ColorRED);
	rectangle3->SetOpacity(0.5);
	rectangle4->SetOpacity(0.5);

	rectangleBackground->Resize(100, 100);
	rectangleBackground->LockSizing();

	panel->Arrange();

	window->Show();
}

int main() {
	VApplication application;

	CreatePanelViewer(&application);
	CreateStackPanelViewer(&application);

	return application.Run();
}