#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/visual/vvisual.h"
#include "./include/core/animation/vanimation.h"

#include "./include/vml/vmlwidget.h"

int main() {
    srand(time(NULL));

    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication  Application;
    VMLMainWindow Widget(500, 400, &Application, true);

    auto   Result = Widget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);
    Widget.Show();

    return Application.Exec();
}