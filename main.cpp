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

int demomain() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget VMLWidget(0, 0, &Application, true);
    auto	  Result= VMLWidget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    return Application.Exec();
}

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;
    VMainWindow  MainWindow(1000, 400, &Application);

    VCircleScrollBarButton TestScrollBar(VDragTowardsMode::Horizontal, 40, 40 ,&MainWindow);
    TestScrollBar.SetDragRange({ 0, 0, 400, 400 });

    return Application.Exec();
}