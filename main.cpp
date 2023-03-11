// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include "./include/core/animation/vanimation.h"
#include "./include/core/control/vcanvas.h"
#include "./include/core/control/veditor.h"
#include "./include/core/control/veditorhighlighter.h"
#include "./include/core/control/vlineeditor.h"
#include "./include/core/control/vscroller.h"
#include "./include/core/control/vswitchgroup.h"
#include "./include/core/control/vtitlebar.h"
#include "./include/core/control/vviewlabel.h"
#include "./include/core/uibasic/vsmarttimer.h"
#include "./include/core/visual/vvisual.h"
#include "./include/kits/vallocator.h"
#include "./include/vml/vmlwidget.h"
#include <iostream>

#define fn	auto
#define var auto

#define format_parameter(variable) ((const char *const)variable)

void ApplyGlitchEffect(ID2D1Bitmap *source, ID2D1DeviceContext *d2dContext, float intensity)
{
	// 创建离散传输效果
	ComPtr<ID2D1Effect> glitchEffect;
	HRESULT				hr = d2dContext->CreateEffect(CLSID_D2D1DiscreteTransfer, &glitchEffect);
	if (SUCCEEDED(hr))
	{
		// 设置输入图像
		glitchEffect->SetInput(0, source);

		// 设置颜色表
		uint32_t tableSize	   = 256;
		uint32_t glitchTable[] = {0, 255};
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_RED_TABLE, D2D1::Vector<float, 2>(glitchTable));
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_GREEN_TABLE, D2D1::Vector<float, 2>(glitchTable));
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_BLUE_TABLE, D2D1::Vector<float, 2>(glitchTable));
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_ALPHA_TABLE, D2D1::Vector<float, 2>(glitchTable));

		// 设置斜率
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_RED_SLOPE, 1.0f);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_GREEN_SLOPE, 1.0f);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_BLUE_SLOPE, 1.0f);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_ALPHA_SLOPE, 1.0f);

		// 设置阈值
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_RED_DISABLE_THRESHOLD, 0.0f);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_GREEN_DISABLE_THRESHOLD, 0.0f);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_BLUE_DISABLE_THRESHOLD, 0.0f);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_ALPHA_DISABLE_THRESHOLD, 0.0f);

		// 设置强度
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_RED_ENABLE_THRESHOLD, intensity);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_GREEN_ENABLE_THRESHOLD, intensity);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_BLUE_ENABLE_THRESHOLD, intensity);
		glitchEffect->SetValue(D2D1_DISCRETETRANSFER_PROP_ALPHA_ENABLE_THRESHOLD, intensity);

		// 将 glitchEffect 应用于目标图像
		d2dContext->DrawImage(glitchEffect.Get(), D2D1::RectF(0, 0, source->GetSize().width, source->GetSize().height));
	}
}

fn main()->int
{
	var Application = Core::VApplication();
	var MainWindow	= VML::VMLMainWindow(640, 800, &Application);

	MainWindow.Show();

	MainWindow.LoadVML(L"./testvml.xml", VML::VMLParserParseMode::FromFile);

	return Application.Exec();
}