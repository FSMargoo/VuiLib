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

int* AllocatoredIntPointer;
int* IntPointer;
VKits::VAllocator ThreadAllocator;

void testAllocator() {
    VKits::VAllocator Allocator(ThreadAllocator);

    AllocatoredIntPointer  = Allocator.Malloc<int>();
    *AllocatoredIntPointer = 40;
}
void testAllocatorWithout() {
    IntPointer = new int(40);
}

#define format_parameter(variable) ((const char* const)variable)

fn main() -> int {
    var Application = Core::VApplication();
    var MainWindow  = VML::VMLMainWindow(400, 400, &Application);
    // var Button      = Core::VPushButton(200, 100, L"Test", &MainWindow);

    MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

    MainWindow.Show();

    return Application.Exec();
}