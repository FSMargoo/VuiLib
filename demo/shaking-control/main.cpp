#include "../../include/vml/vmlwidget.h"

#include <random>

Core::VPushButton		 *MovitionObject;
Core::VPositionAnimation *PositionAnimation;

constexpr int OriginX	 = 640 / 2 - 100 / 2;
constexpr int OriginY	 = 480 / 2 - 100 / 2;
int			  ShakeDealy = 400;
int			  ShakeRange = 0;

double Random(double Start, double End)
{
	static std::random_device RandomDevice;
	static std::mt19937		  Genor(RandomDevice());

	std::uniform_real_distribution<double> Distribution(Start, End);

	return Distribution(Genor);
}

void ResetAnimation()
{
	int OperationToken1 = (rand() % 2) ? 1 : -1;
	int OperationToken2 = (rand() % 2) ? 1 : -1;

	PositionAnimation->SetTargetPosition(
		Core::VPoint(OriginX + OperationToken1 * Random(20 - ShakeRange, 22 + ShakeRange),
					 OriginY + OperationToken2 * Random(20 - ShakeRange, 22 + ShakeRange)));
	PositionAnimation->SetDuration(ShakeDealy);
	PositionAnimation->Start();
}

struct PerlinCurve : public Core::VAnimationCurve
{
	PerlinCurve()
	{
	}

	double CurveFunction(const double &X) override
	{
		return 6 * pow(X, 5) - 15 * pow(X, 4) + 10 * pow(X, 3);
	}
};

int main()
{
	Core::VApplication		App;
	Core::VMainWindow		Window(640, 480, &App);
	Core::VSliderHorizontal ShakeDealySlider(200, &Window);
	Core::VSliderHorizontal ShakeRangeSlider(200, &Window);

	ShakeDealySlider.SetValue(0.4);
	ShakeDealySlider.Move(640 / 2 - 200 / 2, 40);
	ShakeDealySlider.ValueChanged.Connect([](const double &Value) -> void { ShakeDealy = 1000 * Value; });
	ShakeRangeSlider.SetValue(0);
	ShakeRangeSlider.Move(640 / 2 - 200 / 2, 60);
	ShakeRangeSlider.ValueChanged.Connect([](const double &Value) -> void { ShakeRange = 100 * Value; });

	MovitionObject	  = new Core::VPushButton(100, 100, "I AM SHAKING!", &Window);
	PositionAnimation = new Core::VPositionAnimation(MovitionObject, new PerlinCurve, ShakeDealy);

	MovitionObject->Move(OriginX, OriginY);

	PositionAnimation->SetTargetPosition({OriginX, OriginY});
	PositionAnimation->AnimationEnd.Connect(ResetAnimation);
	PositionAnimation->Start();

	Window.Show();

	return App.Exec();
}