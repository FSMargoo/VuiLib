// License(MIT)
// Athuor: Margoo
// This is the basic core file in vuilib
#pragma once

#ifndef VLIB_ENABLE_NAMESPACE
#define VLIB_BEGIN_NAMESPACE
#define VLIB_END_NAMESPACE
#else
#define VLIB_BEGIN_NAMESPACE namespace VuiLib {
#define VLIB_END_NAMESPACE   }
#endif

#define WINVER 0x0600

#include <new>
#include <graphics.h>
#include <string>
#include <crtdbg.h>
#include <d2d1helper.h>

VLIB_BEGIN_NAMESPACE

namespace Core {
#define VLIB_REPORT_ERROR(ErrorMessage)                   \
    do {												  \
        _RPTFW0(_CRT_ASSERT, ErrorMessage);                \
    } while (true)

#define VLIB_CHECK_REPORT(Expression, ErrorMessage)		  \
    while ((Expression)) {												  \
        _RPTFW0(_CRT_ASSERT, ErrorMessage);                \
    }

template<class DataType>
class VCoreRect {
 public:
	 DataType Left;
	 DataType Right;
	 DataType Top;
	 DataType Bottom;

 public:
	 VCoreRect(const VCoreRect& RectObject);
	 VCoreRect(DataType LeftData, DataType TopData, DataType RightData, DataType BottomData);
	 VCoreRect();

 public:
	 DataType GetWidth() const;
	 DataType GetHeight() const;

 public:
	 void Move(const DataType& X, const DataType& Y);
     VCoreRect* MoveRV(const DataType& X, const DataType& Y);
	 void Extended(const DataType& LeftOffset, const DataType& TopOffset,
		 const DataType& RightOffset, const DataType& BottomOffset);
	 void Resize(const DataType& Width, const DataType& Height);

	 void FusionRect(const VCoreRect& FusionObject);

	 bool Overlap(const VCoreRect& JudgedObject) const;
	 bool Include(const VCoreRect& JudgedObject) const;

	 VCoreRect Clone() const;

 public:
	 inline RECT ToRECT() const {
		 return RECT{ static_cast<LONG>(Left), static_cast<LONG>(Top), static_cast<LONG>(Right), static_cast<LONG>(Bottom) };
	 }
	 inline D2D1_SIZE_U ToDxPoint2U() const {
		 return D2D1_SIZE_U{ static_cast<UINT32>(GetWidth()), static_cast<UINT32>(GetHeight()) };
	 }

 public:
	 friend bool operator==(const VCoreRect& LeftObject, const VCoreRect& RightObject) {
		 return LeftObject.Left   == RightObject.Left &&
				LeftObject.Right  == RightObject.Right &&
				LeftObject.Bottom == RightObject.Bottom &&
				LeftObject.Top	  == RightObject.Top;
	 }
	 friend bool operator!=(const VCoreRect& LeftObject, const VCoreRect& RightObject) {
		 return !(LeftObject.Left == RightObject.Left &&
			 LeftObject.Right == RightObject.Right &&
			 LeftObject.Bottom == RightObject.Bottom &&
			 LeftObject.Top == RightObject.Top);
	 }
};

template<class DataType>
class VCorePoint {
 public:
	 DataType X;
	 DataType Y;

 public:
	 VCorePoint();
	 VCorePoint(const VCorePoint& PointObject);
	 VCorePoint(const DataType& TargetX, const DataType& TargetY)
		 : X(TargetX), Y(TargetY) {
	 }

 public:
	 bool InsideRectangle(const VCoreRect<DataType>& RectangleObject) const;

	 void Move(const DataType& TargetX, const DataType& TargetY);

     void Offset(const DataType& OffsetX, const DataType& OffsetY);

	 inline POINT ToPOINT() const {
		 return POINT { X, Y };
	 }
	 inline D2D1_POINT_2U ToDxPointU() {
		 return D2D1::Point2U(X, Y);
	 }
	 inline D2D1_POINT_2F ToDxPOINT2F() {
		 return D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y));
	 }

 public:
	friend bool operator==(const VCorePoint& LeftObject, const VCorePoint& RightObject) {
		return LeftObject.X == RightObject.X &&
			   LeftObject.Y == RightObject.Y;
	}
	friend bool operator!=(const VCorePoint& LeftObject, const VCorePoint& RightObject) {
		return !(LeftObject.X == RightObject.X &&
				 LeftObject.Y == RightObject.Y);
	}
};

template<class DataType>
class VCoreGeomtery {
public:
	DataType Width;
	DataType Height;

public:
	VCoreGeomtery();
	VCoreGeomtery(const VCoreGeomtery& GeomteryObject);
	VCoreGeomtery(const DataType& TargetWidth, const DataType& TargetHeight);

public:
	bool InsideRectangle(VCoreRect<DataType> RectangleObject);

	void Move(const DataType& TargetWidth, const DataType& TargetHeight);

public:
	friend bool operator==(const VCoreGeomtery& LeftObject, const VCoreGeomtery& RightObject) {
		return LeftObject.Width == RightObject.Width &&
			LeftObject.Height == RightObject.Height;
	}
	friend bool operator!=(const VCoreGeomtery& LeftObject, const VCoreGeomtery& RightObject) {
		return !(LeftObject.Width == RightObject.Width &&
			LeftObject.Height == RightObject.Height);
	}
};

using VRect     = VCoreRect<int>;
using VRectF    = VCoreRect<double>;
using VPoint    = VCorePoint<int>;
using VPointF   = VCorePoint<double>;
using VGeomtery = VCoreGeomtery<int>;

}

VLIB_END_NAMESPACE