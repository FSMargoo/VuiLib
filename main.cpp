#include "./include/core/control/vlineeditor.h"

#include "./include/vml/vmlwidget.h"

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget VMLWidget(400, 400, &Application, true);

    auto   Result = VMLWidget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    return Application.Exec();
}