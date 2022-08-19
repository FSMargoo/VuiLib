#include "../../include/core/vbasiccore.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

template<>
VCoreRect<int>::VCoreRect(const VCoreRect<int>& RectObject) {
    Left   = RectObject.Left;
    Right  = RectObject.Right;
    Top    = RectObject.Top;
    Bottom = RectObject.Bottom;
}
template<>
VCoreRect<int>::VCoreRect(int LeftData, int TopData, int RightData, int BottomData)
	: Left(int(LeftData)),
	  Right(int(RightData)),
	  Top(int(TopData)),
	  Bottom(int(BottomData)) {
}
template<>
VCoreRect<int>::VCoreRect()
	: Left(int(0)),
	  Right(int(0)),
	  Top(int(0)),
	  Bottom(int(0)) {
}
template<>
void VCoreRect<int>::Move(const int& X, const int& Y) {
	int Width = GetWidth();
	int Height = GetHeight();

	Left = X;
	Top = Y;
	Right = X + Width;
	Bottom = Y + Height;
}
template<>
void VCoreRect<int>::Extended(const int& LeftOffset, const int& TopOffset,
	const int& RightOffset, const int& BottomOffset) {
	Left -= LeftOffset;
	Right += RightOffset;
	Top -= TopOffset;
	Bottom += BottomOffset;
}
template<>
void VCoreRect<int>::FusionRect(const VCoreRect& FusionObject) {
	Left   = min(FusionObject.Left, Left);
	Right  = max(FusionObject.Right, Right);
	Top    = min(FusionObject.Top, Top);
	Bottom = max(FusionObject.Bottom, Bottom);
}
template<>
VCoreRect<int>* VCoreRect<int>::MoveRV(const int& X, const int& Y) {
    Move(X, Y);

    return this;
}
template<>
void VCoreRect<int>::Resize(const int& Width, const int& Height) {
	Top = Left + Width;
}
template<>
bool VCoreRect<int>::Overlap(const VCoreRect<int>& JudgedObject) const {
	return max(Left, JudgedObject.Left) < min(Right, JudgedObject.Right) &&
		   max(Top, JudgedObject.Top) < min(Bottom, JudgedObject.Bottom);
}
template<>
bool VCoreRect<int>::Include(const VCoreRect<int>& JudgedObject) const {
	return Left <= JudgedObject.Left && Top <= JudgedObject.Top &&
		   Bottom >= JudgedObject.Bottom && Right >= JudgedObject.Right;
}
template<>
VCoreRect<int> VCoreRect<int>::Clone() const {
	return VCoreRect{ Left, Top, Right, Bottom };
}
template<>
int VCoreRect<int>::GetHeight() const {
	return Bottom - Top;
}
template<>
int VCoreRect<int>::GetWidth() const {
	return Right - Left;
}

template<>
VCorePoint<int>::VCorePoint()
	: X(0), Y(0) {
}
template<>
VCorePoint<int>::VCorePoint(const VCorePoint<int>& PointObject) {
    X = PointObject.X;
    Y = PointObject.Y;
}
template<>
bool VCorePoint<int>::InsideRectangle(const VCoreRect<int>& RectangleObject) const {
	return RectangleObject.Left <= X && RectangleObject.Top <= Y &&
		   RectangleObject.Right >= X && RectangleObject.Bottom >= Y;
}
template<>
void VCorePoint<int>::Move(const int& TargetX, const int& TargetY) {
	X = TargetX;
	Y = TargetY;
}
template<>
void VCorePoint<int>::Offset(const int& OffsetX, const int& OffsetY) {
    X += OffsetX;
    Y += OffsetY;
}

template<>
VCoreGeomtery<int>::VCoreGeomtery()
	: Width(0), Height(0) {
}
template<>
VCoreGeomtery<int>::VCoreGeomtery(const int& TargetWidth, const int& TargetHeight)
	: Width(TargetWidth), Height(TargetHeight) {
}
template<>
VCoreGeomtery<int>::VCoreGeomtery(const VCoreGeomtery<int>& GeomteryObject) {
    Width  = GeomteryObject.Width;
    Height = GeomteryObject.Height;
}
template<>
bool VCoreGeomtery<int>::InsideRectangle(VCoreRect<int> RectangleObject) {
	return RectangleObject.Left <= Width && RectangleObject.Top <= Height &&
		RectangleObject.Right >= Width && RectangleObject.Bottom >= Height;
}
template<>
void VCoreGeomtery<int>::Move(const int& TargetWidth, const int& TargetHeight) {
	Width = TargetWidth;
	Height = TargetHeight;
}

}

VLIB_END_NAMESPACE