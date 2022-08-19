// License(MIT)
// Author: Margoo
// This is a file to test vuilib
#include "./include/core/render/vpainter.h"
#include "./include/core/widget/vwidget.h"
#include "./include/core/control/vpushbutton.h"
#include "./include/core/control/vimagelabel.h"
#include "./include/core/control/vtextlabel.h"

#include "./include/vml/vml.h"

#include <graphics.h>
#include <conio.h>

#include <time.h>

int TestVRenderMain() {
    initgraph(640, 480);

    BeginBatchDraw();

    ID2D1Factory* test = nullptr;

    Core::VDCRender DCRender(Core::VDirectXD2DFactory.GetInstance(), GetImageHDC(), Core::VRect( 0, 0, 640, 480 ), true);
    Core::VPainter  Painter(DCRender.GetDirectXRenderTarget());
    Core::VCanvasPainter CanvasPainter(140, 140, DCRender.GetDirectXRenderTarget());
    Core::VPenBrush Brush(DCRender.GetDirectXRenderTarget(), Core::VColor(Core::VColor::White));
    Core::VSolidBrush SolidBrush(DCRender.GetDirectXRenderTarget(), Core::VColor::FromBYTERGBA(255, 105, 0, 255));
    Core::VImage	Image(L"G:\\VueLib\\vuilib\\vuilib\\test.png", DCRender.GetDirectXRenderTarget());
    Core::VFont     MyFontTest(L"Google Sans", Core::VFont::WEIGHT_BOLD, Core::VFont::STYLE_NORMAL, Core::VFont::STRETCH_NORMAL, 30.f, L"zh-cn");

    MyFontTest.SetLineAlignment(Core::VFontAlignment::DWRITE_TEXT_ALIGNMENT_CENTER);
    MyFontTest.SetParagraphAlignment(Core::VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    Brush.SetThickness(2.f);

    CanvasPainter.BeginDraw();

    CanvasPainter.SolidEllipse({ 0, 0, 140, 140 }, &SolidBrush);

    CanvasPainter.EndDraw();

    time_t Start = clock();

    Painter.BeginDraw();
    Painter.Clear(Core::VColor::FromBYTERGBA(0, 0, 0, 0));
    Painter.FillRoundedRectangle({ 40, 40, 180, 180 }, { 40, 40 }, &Brush, &SolidBrush);
    Painter.DrawString(L"Google", { 40, 40, 180, 180 }, &MyFontTest, &Brush);
    Painter.DrawImage({ 140, 140, 512 + 140, 288 + 140 }, &Image, { 0, 0, 5120, 2880 }, 1.f);
    Painter.DrawCanvas({ 0, 0, 140, 140 }, &CanvasPainter, { 0, 0, 140, 140 }, 1.f);
    Painter.EndDraw();

    printf("%ws\n", (L"Rended Used Time (Text With Rounded Rectangle): " + std::to_wstring(clock() - Start) + L"ms\n").c_str());

    while (true) {
        FlushBatchDraw();

        Sleep(10);
    }

    return _getch();
}

void OnClicked() {
    MessageBoxW(GetHWnd(), L"Hello World!", L"Hello World!", MB_OK);
}

Core::VImageLabel* BackgroundImage;

void WndOnSize(const int& Width, const int& Height) {
    BackgroundImage->Resize(Width, Height);
}

int vwidgetmain() {
    using namespace Core;

    VApplication Application;
    VMainWindow MainWindow(800, 800, &Application);
    BackgroundImage = new VImageLabel(1920, 1080,
L"bk.jpg", &MainWindow);
    VPushButton PushButton(100, 100, L"Hello World", &MainWindow);
    VTextLabel  TextLabel(100, 40, L"My ImageTester", &MainWindow);

    MainWindow.Resized.Connect(::WndOnSize);

    PushButton.Move(100, 100);
    PushButton.ButtonPushed.Connect(::OnClicked);

    return Application.Exec();
}

int main() {
    using namespace Core;
    using namespace VML;
    using namespace VSS;

    VApplication Application;

    VMLWidget VMLWidget(0, 0, &Application, true);
    VMLParser Parser(L"./testvml.xml", VMLParserParseMode::FromFile);
    auto	  Result      = VMLWidget.LoadVML(Parser.ParseVML());

    return Application.Exec();
}
