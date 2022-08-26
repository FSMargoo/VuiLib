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

int vscrollmain() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;
    VMainWindow  MainWindow(1000, 400, &Application);

    VCircleScrollBarButton TestScrollBar(VDragTowardsMode::Horizontal, 16, 16 ,&MainWindow);
    TestScrollBar.SetDragRange({ 0, 0, 400, 400 });

    return Application.Exec();
}

#include <stdio.h>

int main() {
    printf("VSS Parser Engine 处理十万次 VSS Parser 任务\n开始 ");

    long start = clock();

    for (int i = 0; i < 1000000; ++i) {
        VSS::VSSParser MyTestParser(L".main-title {\n    color: #ffffff;\n    font-size: 34px;\n}\n.description-text {\n    color:#737373;\n    font-size: 16px;\n    text-align: left;\n}\n.version-subtext-text {\n    color: rgba(242, 242, 242, 0.7);\n    font-size: 17px;\n}\n.target-radio-button:hover {\n    background-color: #ffffff;\n}\n.target-radio-button:enabled {\n    background-color: rgb(0, 255, 0);\n}\n.target-radio-button:disabled {\n    background-color: #000000;\n}\n.scale-text {\n    text-align: center;\n    vertical-align: center;\n\n    color: #000000;\n\n    background-color: rgb(255, 255, 255);\n}\n\nslider::groove:horizontal { \n    border-radius: 6px;\n    background:rgba(0, 0, 0, 50);\n}\n", VSS::VVVSParserParseMode::FromString);
        auto ParseResult = MyTestParser.ParseVSS();
    }

    printf("\n完成! 共用时：%ldms", clock() - start);

    return 0;
}