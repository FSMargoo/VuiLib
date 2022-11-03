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

    VApplication Application;

    VMLWidget Widget(500, 400, &Application, true);

    auto   Result = Widget.LoadVML(L"./testvml.xml", VMLParserParseMode::FromFile);

    /* VImage Image(L"./test.jpg", Widget.CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());
    VImageLabel ImageLabel(500, 400, &Image, &Widget);
    VPolygonView* View = new VPolygonView(&Widget);
    VBlurLabel  BlurLabel(400, 80, View);
    VPushButton* PushButton = new VPushButton(400, 80, L"Click me", View);

    View->SetShadowStats(true);

    BlurLabel.GetTheme()->BlurRadius = 20;
    BlurLabel.GetTheme()->MixedColor = VColor(0.f, 0.f, 0.f, 0.f);

    auto Color = Image.GetPixel(300, 300, Widget.CallWidgetGetDCRenderTarget()->GetDirectXRenderTarget());

    View->AddPoint({ 40, 0 });
    View->AddPoint({ 0, 80 });
    View->AddPoint({ 400 - 40, 80 });
    View->AddPoint({ 400, 0 });

    View->Move(80, 80);

    View->Resize(400, 80); */

    return Application.Exec();
}