#include "./include/core/render/vpainter.h"
#include "./include/core/widget/vwidget.h"

#include "./include/vml/vmlwidget.h"
#include "./include/core/control/vblurlabel.h"

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget VMLWidget(400, 400, &Application, true);

    auto   Result = VMLWidget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    return Application.Exec();
}