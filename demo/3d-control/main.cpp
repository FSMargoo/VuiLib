#include "../../include/vml/vmlwidget.h"

#include <graphics.h>

#define MAX_POINT_AMOUNT 5000
#define PI				 3.1415926565

struct Point3d
{
	double X;
	double Y;
	double Z;
};

Core::VTextLabel	*PointInfoLabel;
Core::VTextLabel	*CanvasInfoLabel;
Core::VPushButton	*AnimationButton;
int					 PointAmount = 2500;
int					 CameraZ	 = 3;
int					 Fps		 = 0;
std::vector<Point3d> PointSets;
bool				 InAnimation = true;

void InitPoint()
{
	PointSets.clear();

	double rxy, a;
	for (int Count = 0; Count < PointAmount; ++Count)
	{
		Point3d PointTemp;

		PointTemp.Z = 2.0 * rand() / RAND_MAX - 1;

		rxy = sqrt(1 - PointTemp.Z * PointTemp.Z);
		a	= 2 * PI * rand() / RAND_MAX;

		PointTemp.X = cos(a) * rxy;
		PointTemp.Y = sin(a) * rxy;

		PointSets.push_back(PointTemp);
	}
}

void PointSetted(const double &Value)
{
	PointAmount = MAX_POINT_AMOUNT * Value;
	PointInfoLabel->SetPlaneText(L"Point amount : " + std::to_wstring(PointAmount));
	InitPoint();
}

void RotateX(Point3d &Point, double Angle)
{
	double Y = Point.Y;
	Point.Y	 = Point.Y * cos(Angle) + Point.Z * sin(-Angle);
	Point.Z	 = Y * sin(Angle) + Point.Z * cos(Angle);
}

void RotateY(Point3d &Point, double Angle)
{
	double X = Point.X;
	Point.X	 = Point.X * cos(Angle) + Point.Z * sin(-Angle);
	Point.Z	 = X * sin(Angle) + Point.Z * cos(Angle);
}

void RotateZ(Point3d &Point, double Angle)
{
	double X = Point.X;
	Point.X	 = Point.X * cos(Angle) + Point.Y * sin(-Angle);
	Point.Y	 = X * sin(Angle) + Point.Y * cos(Angle);
}

POINT Projection(Point3d Point)
{
	POINT Point2D;

	Point2D.x = (int)(Point.X * (CameraZ / (CameraZ - Point.Z)) * 200 + 0.5) + 320;
	Point2D.y = (int)(Point.Y * (CameraZ / (CameraZ - Point.Z)) * 200 + 0.5) + 240;

	return Point2D;
}

void CanvasShow(Core::VCanvasPainter *Painter, const Core::VRenderHandle &RenderHandle)
{
	++Fps;

	Painter->BeginDraw();
	Painter->Clear(Core::VColor::White);

	for (auto &Point : PointSets)
	{
		if (InAnimation)
		{
			RotateX(Point, PI / 800);
			RotateY(Point, PI / 600);
			RotateZ(Point, PI / 400);
		}

		BYTE DeepthColor = (BYTE)(Point.Z * 100) + 155;

		POINT Point2d = Projection(Point);

		Core::VPenBrush Solid(Core::VColor::FromBYTERGBA(0, 0, 0, DeepthColor), RenderHandle);

		Painter->DrawLine({Point2d.x, Point2d.y}, {Point2d.x + 1, Point2d.y}, &Solid);
	}

	Painter->EndDraw();
}

void ResetAllPoints()
{
	InitPoint();
}

void AnimationStatusSwitch()
{
	InAnimation = !InAnimation;

	if (InAnimation)
	{
		AnimationButton->SetPlaneText(L"Stop Animation");
	}
	else
	{
		AnimationButton->SetPlaneText(L"Play Animation");
	}
}

Core::VSmartTimer *FpsTimer;

void FpsOnFlush()
{
	CanvasInfoLabel->SetPlaneText(L"Canvas : 721x706 \nFps: " + std::to_wstring(Fps));
	FpsTimer->Start(1000);

	Fps = 0;
}

int main()
{
	Core::VApplication Application;
	VML::VMLMainWindow MainWindow(&Application);

	srand((unsigned)time(NULL));

	InitPoint();

	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(PointSetted));
	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(CanvasShow));
	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(ResetAllPoints));
	MainWindow.RegisterMetaFunction(VML_META_FUNCTION(AnimationStatusSwitch));

	MainWindow.LoadVML(L"./ui.xml", VML::VMLParserParseMode::FromFile);

	PointInfoLabel	= MainWindow[L"widget"][L"points-amount"].Get<Core::VTextLabel>();
	CanvasInfoLabel = MainWindow[L"widget"][L"canvas-info-text"].Get<Core::VTextLabel>();
	AnimationButton = MainWindow[L"widget"][L"animation-start"].Get<Core::VPushButton>();

	FpsTimer = new Core::VSmartTimer(&Application);

	FpsTimer->OnTime.Connect(FpsOnFlush);
	FpsTimer->Start(1000);

	return Application.Exec();
}