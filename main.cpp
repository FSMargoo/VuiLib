#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vscroller.h"

#include "./include/vml/vmlwidget.h"

Core::VTextLabel* ValueLabel;
Core::VScrollerPushButton* ScrollerButton;
Core::VScrollerVertical* VerticalTest;
Core::VScrollerHorizontal* HorizontalTest;

void ValueOnChange() {
    ValueLabel->SetPlaneText(std::to_wstring(float(ScrollerButton->GetY()) / (100.f - 40.f) * 100) + L"%");
}

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget Widget(400, 400, &Application, true);

    // auto   Result = Widget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    VerticalTest = new VScrollerVertical(11, 200, 300, &Widget);
    HorizontalTest = new VScrollerHorizontal(200, 11, 300, &Widget);
    ValueLabel = new VTextLabel(100, 40, L"0%", &Widget);
    ValueLabel->Move(100, 0);
    HorizontalTest->Move(100, 170);
    VerticalTest->Move(100, 200);

    return Application.Exec();
}