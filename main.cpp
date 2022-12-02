#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/visual/vvisual.h"
#include "./include/core/animation/vanimation.h"

#include "./include/core/uibasic/vsmarttimer.h"

#include "./include/vml/vmlwidget.h"
#include "./include/kits/vallocator.h"

#define fn  auto
#define var auto

#define format_parameter(variable) ((const char* const)variable)

void test() {
    MessageBox(NULL, L"Meta function test", L"Meta function test", MB_OK);
}

fn main() -> int {
    var Application = Core::VApplication();
    var MainWindow  = VML::VMLMainWindow(400, 400, &Application);

    MainWindow.Show();
    MainWindow.RegisterMetaFunction(VML_META_FUNCTION(test));

    MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

    return Application.Exec();
}