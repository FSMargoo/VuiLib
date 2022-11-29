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
    	: Left(LeftData), Right(RightData), Top(TopData), Bottom(BottomData) {
    }
    template<>
    VCoreRect<int>::VCoreRect() : Left(int(0)), Right(int(0)), Top(int(0)), Bottom(int(0)) {
    }

    template<>
    void VCoreRect<int>::Move(const int& X, const int& Y) {
    	int Width  = GetWidth();
    	int Height = GetHeight();
    
    	Left       = X;
    	Top        = Y;
    	Right      = X + Width;
    	Bottom     = Y + Height;
    }
    template<>
    void VCoreRect<int>::Extended(const int& LeftOffset,  const int& TopOffset,
    	                          const int& RightOffset, const int& BottomOffset) {
    	Right  += RightOffset;
    	Bottom += BottomOffset;
        Left   -= LeftOffset;
    	Top    -= TopOffset;
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
    	Right  = Left + Width;
        Bottom = Top  + Height;
    }
    template<>
    bool VCoreRect<int>::Overlap(const VCoreRect<int>& JudgedObject) const {
    	return max(Left, JudgedObject.Left) < min(Right, JudgedObject.Right) &&
    		   max(Top, JudgedObject.Top)   < min(Bottom, JudgedObject.Bottom);
    }
    template<>
    bool VCoreRect<int>::Include(const VCoreRect<int>& JudgedObject) const {
    	return Left  <= JudgedObject.Left    && Top   <= JudgedObject.Top &&
    		   Bottom >= JudgedObject.Bottom && Right >= JudgedObject.Right;
    }
    template<>
    RECT VCoreRect<int>::ToRECT() const {
        return RECT{ static_cast<LONG>(Left), static_cast<LONG>(Top), static_cast<LONG>(Right), static_cast<LONG>(Bottom) };
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
    VCoreRect<double>::VCoreRect(const VCoreRect<double>& RectObject) {
        Left   = RectObject.Left;
        Right  = RectObject.Right;
        Top    = RectObject.Top;
        Bottom = RectObject.Bottom;
    }
    template<>
    VCoreRect<double>::VCoreRect(double LeftData, double TopData, double RightData, double BottomData)
            : Left(LeftData), Right(RightData), Top(TopData), Bottom(BottomData) {
    }
    template<>
    VCoreRect<double>::VCoreRect() : Left(double(0.f)), Right(double(0.f)), Top(double(0.f)), Bottom(double(0.f)) {
    }
    template<>
    void VCoreRect<double>::Move(const double& X, const double& Y) {
        double Width  = GetWidth();
        double Height = GetHeight();
    
        Left          = X;
        Top           = Y;
        Right         = X + Width;
        Bottom        = Y + Height;
    }
    template<>
    void VCoreRect<double>::Extended(const double& LeftOffset , const double& TopOffset,
                                     const double& RightOffset, const double& BottomOffset) {
        Right  += RightOffset;
        Bottom += BottomOffset;
        Left   -= LeftOffset;
        Top    -= TopOffset;
    }
    template<>
    void VCoreRect<double>::FusionRect(const VCoreRect& FusionObject) {
        Left   = min(FusionObject.Left, Left);
        Right  = max(FusionObject.Right, Right);
        Top    = min(FusionObject.Top, Top);
        Bottom = max(FusionObject.Bottom, Bottom);
    }
    template<>
    VCoreRect<double>* VCoreRect<double>::MoveRV(const double& X, const double& Y) {
        Move(X, Y);
    
        return this;
    }
    template<>
    void VCoreRect<double>::Resize(const double& Width, const double& Height) {
        Right  = Left + Width;
        Bottom = Top  + Height;
    }
    template<>
    bool VCoreRect<double>::Overlap(const VCoreRect<double>& JudgedObject) const {
        return max(Left, JudgedObject.Left) < min(Right,  JudgedObject.Right) &&
               max(Top,  JudgedObject.Top)  < min(Bottom, JudgedObject.Bottom);
    }
    template<>
    bool VCoreRect<double>::Include(const VCoreRect<double>& JudgedObject) const {
        return Left   <= JudgedObject.Left   && Top   <= JudgedObject.Top &&
               Bottom >= JudgedObject.Bottom && Right >= JudgedObject.Right;
    }
    template<>
    VCoreRect<double> VCoreRect<double>::Clone() const {
        return VCoreRect { Left, Top, Right, Bottom };
    }
    template<>
    double VCoreRect<double>::GetHeight() const {
        return Bottom - Top;
    }
    template<>
    double VCoreRect<double>::GetWidth() const {
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
    VCorePoint<double>::VCorePoint() : X(0), Y(0) {
    }
    template<>
    VCorePoint<double>::VCorePoint(const VCorePoint<double>& PointObject) {
        X = PointObject.X;
        Y = PointObject.Y;
    }
    template<>
    bool VCorePoint<double>::InsideRectangle(const VCoreRect<double>& RectangleObject) const {
        return RectangleObject.Left  <= X && RectangleObject.Top    <= Y &&
               RectangleObject.Right >= X && RectangleObject.Bottom >= Y;
    }
    template<>
    void VCorePoint<double>::Move(const double& TargetX, const double& TargetY) {
        X = TargetX;
        Y = TargetY;
    }
    template<>
    void VCorePoint<double>::Offset(const double& OffsetX, const double& OffsetY) {
        X += OffsetX;
        Y += OffsetY;
    }


    template<>
    VCoreGeometry<int>::VCoreGeometry()
        : Width(0), Height(0) {
    }
    template<>
    VCoreGeometry<int>::VCoreGeometry(const int& TargetWidth, const int& TargetHeight)
        : Width(TargetWidth), Height(TargetHeight) {
    }
    template<>
    VCoreGeometry<int>::VCoreGeomtery(const VCoreGeomtery<int>& GeomteryObject) {
        Width  = GeomteryObject.Width;
        Height = GeomteryObject.Height;
    }
    template<>
    void VCoreGeometry<int>::Resize(const int& TargetWidth, const int& TargetHeight) {
        Width  = TargetWidth;
        Height = TargetHeight;
    }
}

VLIB_END_NAMESPACE