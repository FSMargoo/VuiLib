/*
 * File name	: vbasiccore.h
 * Author		: Margoo
 * Date			: 11/9/2022
 * Description	: The most basic definition of objects and functions in vuilib
*/
#pragma once

#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4305)
#pragma warning(disable : 4302)

/*
 * VLIB_ENABLE_NAMESAPCE Marco [Optional for user]:
 *   Description : If user define the VLIB_ENABLE_NAMESAPCE Marco, all the VuiLib functional will be 
 *				   include in a namespace called VuiLib
*/
#ifndef VLIB_ENABLE_NAMESPACE
#define VLIB_BEGIN_NAMESPACE
#define VLIB_END_NAMESPACE
#else
#define VLIB_BEGIN_NAMESPACE namespace VuiLib {
#define VLIB_END_NAMESPACE   }
#endif

/*
 * The smallest client version support
*/
#define WINVER 0x0600

#include <new>
#include <string>
#include <crtdbg.h>
#include <d2d1helper.h>

#include "../win32base/vwin32base.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

	/*
	 * VLIB_REPORT_ERROR Marco:
	 *	Description : Report the error message directly
	*/
	#define VLIB_REPORT_ERROR(ErrorMessage)                   \
	    do {												  \
	        _RPTFW0(_CRT_ASSERT, ErrorMessage);               \
	    } while (true)
	/*
	 * VLIB_CHECK_REPORT Marco:
	 *  Description : Report the error message if the expression is true
	*/
	#define VLIB_CHECK_REPORT(Expression, ErrorMessage)		  \
	    while ((Expression)) {								  \
	        _RPTFW0(_CRT_ASSERT, ErrorMessage);               \
	    }
	/*
	 * VLIB_ERPORT_IF_FAILED Marco:
	 *  Description : Check the Win32 operation (or any operation that return HRESULT as status code),
	 *				  if the operation failed   (that is, ReturnValue != S_OK), it will report operation failed.
	*/
	#define VLIB_REPORT_IF_FAILED(Expression)				  \
			{ if (Expression != S_OK) { VLIB_CHECK_REPORT(true, L"Operation failed!"); } }
	
	/*
	 * VCoreRect class (template):
	 *	Description : This class describes a rectangle
	*/
	template<class DataType>
	class VCoreRect {
	 public:
		 /*
		  * GetWidth function:
		  *		Description : Get the width of this rectangle
		 */
		 DataType			GetWidth()  const;
		 /*
		  * GetHeight function:
		  *		Description : Get the height of this rectangle
		 */
		 DataType			GetHeight() const;
	
	 public:
		 /*
		  * Move function:
		  *		Description : Move the rectangle to the target point
		 */
		 void		        Move(const DataType& X, const DataType& Y);
		 /*
		  * MoveRV function:
		  *		Description : Move the rectangle to the target point
		  *		Tips		: Don't same with Move function, this function will return a pointer which refer to the object
		 */
	     VCoreRect*         MoveRV(const DataType& X, const DataType& Y);
		 /*
		  * Extended function:
		  *		Description : Extended this rectangle's border with target value
		 */
		 void		        Extended(const DataType& LeftOffset, const DataType& TopOffset,
							 const DataType& RightOffset, const DataType& BottomOffset);
		 /*
		  * Resize function:
		  *		Description : Set this rectangle's size
		 */
		 void		        Resize(const DataType& Width, const DataType& Height);
	
		 /*
		  * FusionRect function:
		  *		Description : Fusion target rectangle with this rectangle
		  *		Tips		: For example, if this rectangle's area is [ 40, 50, 80, 10 ],
		  *					  a rectangle provided's area is [ 30, 60, 100, 40 ], after FusionRect,
		  *					  this rectangle's value will change into [ 30, 50, 100, 40 ], therefore,
		  *					  the new rectangle's area will be:
		  *					  [ min(R1'left, R2'left), min(R1'top, R2'top), max(R1'right, R2'right), max(R1'bottom, R2'bottom) ]
		 */
		 void		        FusionRect(const VCoreRect& FusionObject);
	
		 /*
		  * Overlap function:
		  *		Description : Judge a target rectangle overlap or not with this rectangle,
		  *					  if it overlaps with this rectangle, return true, or false
		  *		Tips		: Not same as Include, Overlap will return true whenever there is an intersection area exists
		 */
		 bool		        Overlap(const VCoreRect& JudgedObject) const;
		 /*
		  * Include function:
		  *		Description : Judge a target rectangle's area was included by this rectangle's area
		  *		Tips		: Not same as Overlap, Include won't return true when there is only an intersection area exists,
		  *					  it require that the following expressions is valid:
		  *						R1'left >= R2'left && R1'top >= R2'top && R1'bottom >= R2'bottom && R1'right >= R2'right
		 */
		 bool		        Include(const VCoreRect& JudgedObject) const;
	
		 /*
		  * Clone function:
		  *		Description : This function will copy this rectangle's area and save it as a new VCoreRect then return as a
		  *					  unwritable variable
		 */
		 VCoreRect          Clone()		 const;
	
	 public:
		 /*
		  * ToRECT function:
		  *		Description : Convert VCoreRect into win32's RECT structure
		 */
		 inline RECT        ToRECT()	 const;
		 /*
		  * ToDxSize2U function:
		  *		Description : Convert VCoreRect into Direct2D's SIZE2U structure
		 */
		 inline D2D1_SIZE_U ToDxSize2U() const {
			 return D2D1_SIZE_U{ static_cast<UINT32>(GetWidth()), static_cast<UINT32>(GetHeight()) };
		 }
	
	 public:
		 /*
		  * operator== function:
		  *		Description : Operator overload
		 */
		 friend bool operator==(const VCoreRect& LeftObject, const VCoreRect& RightObject) {
			 return LeftObject.Left   == RightObject.Left	&&
					LeftObject.Right  == RightObject.Right  &&
					LeftObject.Bottom == RightObject.Bottom &&
					LeftObject.Top	  == RightObject.Top;
		 }
		 /*
		  * operator!= function:
		  *		Description : Operator overload
		 */
		 friend bool operator!=(const VCoreRect& LeftObject, const VCoreRect& RightObject) {
			 return !(LeftObject.Left   == RightObject.Left	  &&
					  LeftObject.Right  == RightObject.Right  &&
					  LeftObject.Bottom == RightObject.Bottom &&
					  LeftObject.Top    == RightObject.Top);
		 }

	 public:
		 DataType Left;
		 DataType Right;
		 DataType Top;
		 DataType Bottom;

	 public:
		 VCoreRect(const VCoreRect& RectObject);
		 VCoreRect(DataType LeftData, DataType TopData, DataType RightData, DataType BottomData);
		 VCoreRect();
	};
	
	/*
	 * VCorePoint class (template):
	 *	Description : This class describes a point on the geometry surface
	*/
	template<class DataType>
	class VCorePoint {
	 public:
		 /*
		  * InsideRectangle function:
		  *		Description : Judge the point inside the target rectangle or not,
		  *				      if the point inside the target rectangle, return true, nor false
		 */
		 bool				  InsideRectangle(const VCoreRect<DataType>& RectangleObject) const;
	
		 /*
		  * Move function:
		  *		Description : Move the point into target point
		 */
		 void				  Move(const DataType& TargetX, const DataType& TargetY);
		 /*
		  * Offset function:
		  *		Description : Add the increment into x and y
		  *		Tips		: The new position p2's position calculate expression will be like:
		  *						p2x = p1x + dx
		  *						p2y = p2y + dy
		 */
	     void				  Offset(const DataType& OffsetX, const DataType& OffsetY);
	
	 public:
		 /*
		  * ToPOINT:
		  *		Description : Convert the VPoint into Win32's POINT structure
		 */
		 inline POINT		  ToPOINT() const {
			 return POINT { X, Y };
		 }
		 /*
		  * ToDxPointU:
		  *		Description : Convert the VPoint into DirectX's Point2U structure
		 */
		 inline D2D1_POINT_2U ToDxPointU() {
			 return D2D1::Point2U(X, Y);
		 }
		 /*
		  * ToDxPOINT2F:
		  *		Description : Convert the VPoint into DirectX's Point2F structure
		 */
		 inline D2D1_POINT_2F ToDxPOINT2F() {
			 return D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y));
		 }
	
	 public:
		 /*
		  * operator== function:
		  *		Description : Operator overload
		 */
		 friend bool operator==(const VCorePoint& LeftObject, const VCorePoint& RightObject) {
			return LeftObject.X == RightObject.X &&
				   LeftObject.Y == RightObject.Y;
		 }
		 /*
		  * operator!= function:
		  *		Description : Operator overload
		 */
		 friend bool operator!=(const VCorePoint& LeftObject, const VCorePoint& RightObject) {
			return !(LeftObject.X == RightObject.X &&
					 LeftObject.Y == RightObject.Y);
		 }

		
	 public:
		 DataType X;
		 DataType Y;
	
	 public:
		 VCorePoint();
		 VCorePoint(const VCorePoint& PointObject);
		 VCorePoint(const DataType& TargetX, const DataType& TargetY) : X(TargetX), Y(TargetY) {  }
	};
	
	/*
	 * VCoreGeometry class (template):
	 *	Description : This class describe a rectangle, but not same as VRect, it only
	 *				  mark the width and height information in a rectangle, no position information
	 *				  is provided here.
	*/
	template<class DataType>
	class VCoreGeometry {
	 public:
	 	 DataType Width;
	 	 DataType Height;
	 
	 public:
		 VCoreGeometry();
		 VCoreGeometry(const VCoreGeometry& GeomteryObject);
		 VCoreGeometry(const DataType& TargetWidth, const DataType& TargetHeight);
	 
	 public:
		 /*
		  * Resize function:
		  *		Description : Set the size of rectangle
		 */
	 	 void Resize(const DataType& TargetWidth, const DataType& TargetHeight);
	 
	 public:
		 /*
		  * operator== function:
		  *		Description : Operator overload
		 */
	 	 friend bool operator==(const VCoreGeometry& LeftObject, const VCoreGeometry& RightObject) {
	 		return LeftObject.Width  == RightObject.Width &&
	 			   LeftObject.Height == RightObject.Height;
	 	 }
		 /*
		  * operator!= function:
		  *		Description : Operator overload
		 */
	 	 friend bool operator!=(const VCoreGeometry& LeftObject, const VCoreGeometry& RightObject) {
	 		return !(LeftObject.Width  == RightObject.Width &&
	 				 LeftObject.Height == RightObject.Height);
	 	 }
	};

	/*
	 * VRect (alias from VCoreRect<int>):
	 *	Description : The rectangle structure which only support int data type
	*/
	using VRect     = VCoreRect<int>;
	/*
	 * VRectF (alias from VCoreRect<double>):
	 *	Description : The rectangle structure which only support double data type
	*/
	using VRectF    = VCoreRect<double>;
	/*
	 * VPoint (alias from VCorePoint<double>):
	 *	Description : The point structure which only support int data type
	*/
	using VPoint    = VCorePoint<int>;
	/*
	 * VPointF (alias from VCorePoint<double>):
	 *	Description : The point structure which only support double data type
	*/
	using VPointF   = VCorePoint<double>;
}

VLIB_END_NAMESPACE