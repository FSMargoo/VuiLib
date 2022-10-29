#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/animation/vanimation.h"

#include "./include/vml/vmlwidget.h"

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    srand(time(NULL));

    VApplication Application;

    VMLWidget Widget(500, 400, &Application, true);

    auto   Result = Widget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    return Application.Exec();
}