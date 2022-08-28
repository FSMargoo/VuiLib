// License(MIT)
// Author: Margoo
#include "./include/core/render/vpainter.h"
#include "./include/core/widget/vwidget.h"
#include "./include/core/control/vscrollbar.h"
#include "./include/core/control/vpushbutton.h"
#include "./include/core/control/vimagelabel.h"
#include "./include/core/control/vtextlabel.h"

#include "./include/vml/vmlwidget.h"
#include "./include/vml/vml.h"

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget VMLWidget(0, 0, &Application, true);
    auto	  Result= VMLWidget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    return Application.Exec();
}

Core::VTextLabel* TextLabel;

int Vmain() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;
    VMainWindow  MainWindow(1000, 400, &Application);

    TextLabel = new Core::VTextLabel(100, 40, &MainWindow);
    VSliderHorizontal SliderTest(100, &MainWindow);
    VSliderVertical   VecrticalSliderTest(100, &MainWindow);

    TextLabel->Move(80, 40);
    SliderTest.Move(80, 80);
    VecrticalSliderTest.Move(400, 80);

    SliderTest.ValueChanged.Connect([](const double& Value) -> void { TextLabel->SetPlaneText(std::to_wstring(Value));  });

    return Application.Exec();
}