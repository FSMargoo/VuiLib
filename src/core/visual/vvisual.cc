#include "../../../include/core/visual/vvisual.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{

VCircleView::VCircleView(Core::VUIObject *Parent) : VUIObject(Parent)
{
}
bool VCircleView::OnMessageTrigger(VRepaintMessage *RepaintMessage)
{
	if (RepaintMessage->DirtyRectangle.Overlap(GetRegion()) &&
		(GetParent()->IsApplication() || GetParent()->GetChildrenVisualRegion().Overlap(GetRegion())))
	{
		VRepaintMessage *ChildRepaintMessage = RepaintMessage;

		if (Canvas != nullptr)
		{
			delete Canvas;

			Canvas = nullptr;
		}

		if (ObjectVisual.Transparency != 0)
		{
			Canvas = new VCanvasPainter(GetRegion().GetWidth(), GetRegion().GetHeight(), CallWidgetGetRenderHandle());
			Canvas->BeginDraw();
			Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
			Canvas->EndDraw();

			OnPaint(Canvas);

			if (!IsWidget() && !IsApplication())
			{
				ChildRepaintMessage = new VRepaintMessage(*RepaintMessage);

				ChildRepaintMessage->DirtyRectangle = *(GetRegion().Clone().MoveRV(0, 0));
			}

			Canvas->BeginDraw();
			SendMessageToChild(ChildRepaintMessage, false);
			Canvas->EndDraw();

			if (ObjectVisual.Shadow.EnableStatus)
			{
				D2D1_POINT_2U OriginPoint = {0, 0};
				D2D1_RECT_U CopyRect = {static_cast<unsigned int>(GetWidth()), static_cast<unsigned int>(GetHeight())};

				ID2D1Bitmap *CanvasSurface;

				Canvas->GetDXObject()->GetBitmap(&CanvasSurface);

				VImage ShadowImage(CanvasSurface);
				ShadowImage.ApplyShadowEffect(ObjectVisual.Shadow.Radius, ObjectVisual.Shadow.Color,
											  CallWidgetGetRenderHandle(), &ObjectVisual.Shadow.Offset);

				GetParentCanvas()->DrawImage(
					{static_cast<int>(GetX() + ObjectVisual.Shadow.Offset.X),
					 static_cast<int>(GetY() + ObjectVisual.Shadow.Offset.Y),
					 static_cast<int>(GetX() + ObjectVisual.Shadow.Offset.X + ShadowImage.GetWidth()),
					 static_cast<int>(GetY() + ObjectVisual.Shadow.Offset.Y + ShadowImage.GetHeight())},
					&ShadowImage, {0, 0, ShadowImage.GetWidth(), ShadowImage.GetHeight()}, ObjectVisual.Transparency);
			}

			if (ChildRepaintMessage != RepaintMessage)
			{
				delete ChildRepaintMessage;
			}

			EditCanvas(Canvas);

			Microsoft::WRL::ComPtr<ID2D1Bitmap> Bitmap;
			Canvas->GetDXObject()->GetBitmap(&Bitmap);

			VImage		 ImageHelper(Bitmap.Get());
			VBitmapBrush BitmapBrush(&ImageHelper, CallWidgetGetRenderHandle());

			GetParentCanvas()->SolidEllipse(GetRegion(), &BitmapBrush);

			delete Canvas;

			Bitmap.ReleaseAndGetAddressOf();
		}

		return true;
	}

	return false;
}

VPolygonView::VPolygonView(VUIObject *Parent) : VUIObject(Parent)
{
}
void VPolygonView::AddPoint(const VPointF &Point)
{
	PolygonPoint.push_back(Point);
}
bool VPolygonView::OnMessageTrigger(VRepaintMessage *RepaintMessage)
{
	if (PolygonPoint.size() >= 3 && RepaintMessage->DirtyRectangle.Overlap(GetRegion()) &&
		(GetParent()->IsApplication() || GetParent()->GetChildrenVisualRegion().Overlap(GetRegion())))
	{
		VRepaintMessage *ChildRepaintMessage = RepaintMessage;

		if (Canvas != nullptr)
		{
			delete Canvas;

			Canvas = nullptr;
		}

		if (ObjectVisual.Transparency != 0)
		{
			Canvas = new VCanvasPainter(GetRegion().GetWidth(), GetRegion().GetHeight(), CallWidgetGetRenderHandle());
			Canvas->BeginDraw();
			Canvas->Clear(VColor(0.f, 0.f, 0.f, 0.f));
			Canvas->EndDraw();

			OnPaint(Canvas);

			if (!IsWidget() && !IsApplication())
			{
				ChildRepaintMessage = new VRepaintMessage(*RepaintMessage);

				ChildRepaintMessage->DirtyRectangle = *(GetRegion().Clone().MoveRV(0, 0));
			}

			Canvas->BeginDraw();
			SendMessageToChild(ChildRepaintMessage, false);
			Canvas->EndDraw();

			VCanvasPainter ViewCanvas(GetWidth(), GetHeight(), CallWidgetGetRenderHandle());
			ID2D1Bitmap	  *Bitmap;
			Canvas->GetDXObject()->GetBitmap(&Bitmap);

			VImage ImageHelper(Bitmap);

			VBitmapBrush							  BitmapBrush(&ImageHelper, CallWidgetGetRenderHandle());
			Microsoft::WRL::ComPtr<ID2D1PathGeometry> PathGeometry;
			Microsoft::WRL::ComPtr<ID2D1GeometrySink> GeometrySink;
			VDirectXD2DFactory.GetInstance()->CreatePathGeometry(&PathGeometry);
			PathGeometry->Open(&GeometrySink);

			GeometrySink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);

			D2D_POINT_2F *PointArray = new D2D_POINT_2F[PolygonPoint.size()];

			for (auto Count = 0; Count < PolygonPoint.size(); ++Count)
			{
				PointArray[Count] =
					D2D1::Point2F(GetWidth() * PolygonPoint[Count].X, GetHeight() * PolygonPoint[Count].Y);
			}

			GeometrySink->AddLines(PointArray, PolygonPoint.size());

			GeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

			GeometrySink->Close();

			ViewCanvas.BeginDraw();

			ViewCanvas.Clear(VColor(0.f, 0.f, 0.f, 0.f));
			ViewCanvas.GetDXObject()->FillGeometry(PathGeometry.Get(), BitmapBrush.GetDxBrush());

			ViewCanvas.EndDraw();

			if (ChildRepaintMessage != RepaintMessage)
			{
				delete ChildRepaintMessage;
			}

			if (ObjectVisual.Shadow.EnableStatus)
			{
				D2D1_POINT_2U OriginPoint = {0, 0};
				D2D1_RECT_U CopyRect = {static_cast<unsigned int>(GetWidth()), static_cast<unsigned int>(GetHeight())};

				Microsoft::WRL::ComPtr<ID2D1Bitmap> CanvasSurface;

				ViewCanvas.GetDXObject()->GetBitmap(&CanvasSurface);

				VImage ShadowImage(CanvasSurface.Get());
				ShadowImage.ApplyShadowEffect(ObjectVisual.Shadow.Radius, ObjectVisual.Shadow.Color,
											  CallWidgetGetRenderHandle(), &ObjectVisual.Shadow.Offset);

				GetParentCanvas()->DrawImage(
					{static_cast<int>(GetX() + ObjectVisual.Shadow.Offset.X),
					 static_cast<int>(GetY() + ObjectVisual.Shadow.Offset.Y),
					 static_cast<int>(GetX() + ObjectVisual.Shadow.Offset.X + ShadowImage.GetWidth()),
					 static_cast<int>(GetY() + ObjectVisual.Shadow.Offset.Y + ShadowImage.GetHeight())},
					&ShadowImage, {0, 0, ShadowImage.GetWidth(), ShadowImage.GetHeight()}, ObjectVisual.Transparency);

				CanvasSurface.ReleaseAndGetAddressOf();
			}

			EditCanvas(Canvas);

			GetParentCanvas()->DrawCanvas(GetRegion(), &ViewCanvas,
										  {0, 0, GetRegion().GetWidth(), GetRegion().GetHeight()},
										  ObjectVisual.Transparency);

			PathGeometry.ReleaseAndGetAddressOf();
			GeometrySink.ReleaseAndGetAddressOf();

			delete[] PointArray;
			delete Canvas;

			Canvas = nullptr;
		}

		return true;
	}

	return false;
}

} // namespace Core

VLIB_END_NAMESPACE