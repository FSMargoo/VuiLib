/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file vRect.h
 * \brief The rectangle data structure of geometry information
 */

#pragma once

#include <include/base/geometry/vPoint.h>

#undef max
#undef min

/**
 * The abstract class for the rectangle data.
 * @tparam DataType The type of the data, int, double, float e.t.c
 */
template<class DataType>
class VRectBase {
public:
	/**
	 * The point alias class for shortcut name
	 */
	using Point = VPointBase<DataType>;
	/**
	 * The tuple of the geometry, the four parameters obey following sort:
	 * 		[1] Left, [2] Top, [3] Right, [4] Bottom
	 * Basically is { (x1, y1), (x2, y2) }
	 */
	using GeometryTuple = std::tuple<DataType, DataType, DataType, DataType>;
	/**
	 * The tuple of point, the first point is the left-top point, the second is
	 * the right-bottom point
	 */
	using PointTuple = std::tuple<Point, Point>;

public:
	/**
	 * Create a rectangle by the point tuple
	 * @param Tuple The tuple of point, the first point is the left-top point, the second is
	 * the right-bottom point
	 */
	explicit VRectBase(const Point &Tuple) : VRectBase(std::get<0>(Tuple), std::get<1>(Tuple)) {
	}
	/**
	 * Create a rectangle by the geometry tuple
	 * @param Tuple The tuple of geometry. The tuple of the geometry, the four
	 * parameters obey following sort:
	 * 		[1] Left, [2] Top, [3] Right, [4] Bottom
	 * Basically is { (x1, y1), (x2, y2) }
	 */
	explicit VRectBase(const GeometryTuple &Tuple)
	    : _left(std::get<0>(Tuple)), _top(std::get<1>(Tuple)), _right(std::get<2>(Tuple)), _bottom(std::get<3>(Tuple)) {
	}
	/**
	 * Create a rectangle by two points geometry data
	 * @param LeftTop The point located on the left corner of the rectangle
	 * @param RightBottom The point located on the right corner of the rectangle
	 */
	VRectBase(const Point &LeftTop, const Point &RightBottom)
	    : _left(LeftTop._x), _top(LeftTop._y), _right(RightBottom._x), _bottom(RightBottom._y) {
	}
	/**
	 * Construct the rectangle with specified geometry data
	 * @param Left The left position of the rectangle
	 * @param Top The top position of the rectangle
	 * @param Right The right position of the rectangle
	 * @param Bottom The bottom position of the rectangle
	 */
	VRectBase(const DataType &Left, const DataType &Top, const DataType &Right, const DataType &Bottom)
	    : _left(Left), _top(Top), _right(Right), _bottom(Bottom) {
	}
	/**
	 * Construct the point in default, it will create a rect
	 * locate at (0, 0) with the size of [0x0], the construct
	 * of zero will pas the construct function of the DataType class
	 */
	VRectBase() : _left(DataType(0)), _right(DataType(0)), _top(DataType(0)), _bottom(DataType(0)) {
	}

public:
	/**
	 * This API will judge whether THIS rectangle is included in the target
	 * rectangle, please notice that, when a rectangle A equals to rectangle B
	 * the rectangle A is also included rectangle as well as rectangle B included
	 * in rectangle A, the detailed judge rule can be seen in the following figure:
	 *
	 * 		┌───────────────────────┐    ┌────────────┐
	 * 		│ Rectangle A           │    │Rectangle A │
	 * 		│  ┌────────────────┐   │    │            │
	 * 		│  │ Rectangle B    │   │    │            │
	 * 		│  │                │   │    │     ┌──────┴─────┐
	 * 		│  │                │   │    │     │Rectangle B │
	 * 		│  │                │   │    │     │      │     │
	 * 		│  │                │   │    │     │      │     │
	 * 		│  │                │   │    └─────┼──────┘     │
	 * 		│  └────────────────┘   │          │            │
	 * 		│                       │          │            │
	 * 		└───────────────────────┘          └────────────┘
	 * 			IsInclude -> True         IsInclude -> False
	 *
	 * 					 ┌───────────┐
	 * 					 │Rectangle A│
	 * 					 │           │
	 * 					 │           │ ┌───────────────┐
	 * 					 │           │ │ Rectangle B   │
	 * 					 │           │ │               │
	 * 					 └───────────┘ │               │
	 * 								   │               │
	 * 								   │               │
	 * 								   │               │
	 * 								   └───────────────┘
	 * 							IsInclude -> False
	 *
	 * @param Target The target rectangle
	 * @return If the rectangle was included in the target rectangle, return true,
	 * otherwise false.
	 */
	bool IsInclude(const VRectBase<DataType> &Target) {
		return Target._left <= _left && Target._top <= _top && Target._right >= _right && Target._bottom >= _bottom;
	}
	/**
	 * This API will judge whether THIS rectangle is overlapped with the target
	 * rectangle, please notice that, when a rectangle A equals to rectangle B
	 * the rectangle A is also overlapped with rectangle as well as rectangle B overlapped
	 * with rectangle A, the detailed judge rule can be seen in the following figure:
	 *
	 * ┌───────────────────────┐    ┌────────────┐
	 * │ Rectangle A           │    │Rectangle A │
	 * │  ┌────────────────┐   │    │            │
	 * │  │ Rectangle B    │   │    │            │
	 * │  │                │   │    │     ┌──────┴─────┐
	 * │  │                │   │    │     │Rectangle B │
	 * │  │                │   │    │     │      │     │
	 * │  │                │   │    │     │      │     │
	 * │  │                │   │    └─────┼──────┘     │
	 * │  └────────────────┘   │          │            │
	 * │                       │          │            │
	 * └───────────────────────┘          └────────────┘
	 * 	IsOverlap -> True         IsOverlap -> True
	 *
	 * 			 ┌───────────┐
	 * 			 │Rectangle A│
	 * 			 │           │
	 * 			 │           │ ┌───────────────┐
	 * 			 │           │ │ Rectangle B   │
	 * 			 │           │ │               │
	 * 			 └───────────┘ │               │
	 * 						   │               │
	 * 						   │               │
	 * 						   │               │
	 * 						   └───────────────┘
	 * 					IsOverlap -> False
	 *
	 * @param Targets The target rectangle
	 * @return If the rectangle was overlapped with the target rectangle, return true,
	 * otherwise false.
	 */
	bool IsOverlap(const VRectBase<DataType> &Targets) {
		return std::max(_left, Targets.Left) < std::min(_right, Targets.Right) &&
		       std::max(_top, Targets.Top) < std::min(_bottom, Targets.Bottom);
	}

public:
	/**
	 * Get the width of the rectangle, it will calculate on time
	 * @return The width of the rectangle
	 */
	DataType GetWidth() const {
		return _right - _left;
	}
	/**
	 * Get the height of the rectangle, it will calculate on time
	 * @return The height of the rectangle
	 */
	DataType GetHeight() const {
		return _bottom - _top;
	}
	/**
	 * Get the point on the left-up corner
	 * @return The point location on the left-up corner
	 */
	Point GetLeftTopPoint() const {
		return Point(_left, _top);
	}
	/**
	 * Get the point on the right-bottom corner
	 * @return The point location on the right-bottom corner
	 */
	Point GetRightBottomPoint() const {
		return Point(_right, _bottom);
	}
	/**
	 * Get the geometry tuple of the rectangle
	 * @return The tuple of the geometry, the four parameters obey following sort:
	 * 		[1] Left, [2] Top, [3] Right, [4] Bottom
	 * Basically is { (x1, y1), (x2, y2) }
	 */
	GeometryTuple GetGeometryTuple() const {
		return {_left, _top, _right, _bottom};
	}
	/**
	 * Set the width of the rectangle, it will calculate the new position
	 * of right-bottom position base on the left-top position
	 * @param Width The width of the rectangle
	 */
	void SetWidth(const int &Width) {
		_right = _left + Width;
	}
	/**
	 * Set the height of the rectangle, it will calculate the new position
	 * of right-bottom position base on the left-top position
	 * @param Height The height of the rectangle
	 */
	void SetHeight(const int &Height) {
		_bottom = _top + Height;
	}
	/**
	 * Set the geometry information of the rectangle, it will calculate the new position
	 * of right-bottom position base on the left-top position
	 * @param Width The width of the rectangle
	 * @param Height The height of the rectangle
	 */
	void Resize(const int &Width, const int &Height) {
		SetWidth(Width);
		SetHeight(Height);
	}
	/**
	 * Set the point on the left-up corner
	 * @param LeftTop The point location on the left-up corner
	 */
	void SetLeftTopPoint(const Point &LeftTop) const {
		_left = LeftTop._x;
		_top  = LeftTop._y;
	}
	/**
	 * Set the point on the right-bottom corner
	 * @param RightBottom The point location on the left-up corner
	 */
	void SetRightBottomPoint(const Point &RightBottom) const {
		_right  = RightBottom._x;
		_bottom = RightBottom._y;
	}
	/**
	 * Extend the rectangle size by the given offset parameter, for the
	 * negative number parameter, it will scale the rectangle
	 * @param Left The offset of left
	 * @param Top The offset of top
	 * @param Right The offset of right
	 * @param Bottom The offset of bottom
	 */
	void Extend(const DataType &Left, const DataType &Top, const DataType &Right, const DataType &Bottom) {
		_left   -= Left;
		_top    -= Top;
		_bottom += Bottom;
		_right  += Right;
	}
	/**
	 * This function will fusion the target rectangle with THIS rectangle, the
	 * way to fusion is to use the minimum or maximum value of the point, for
	 * example, when rectangle A [(0, 0), (8, 8)] fusion with rectangle B
	 * [(-1, -1), (3, 3)] the rectangle will be fusion as [(-1, -1), (8, 8)]
	 * @param Target The target rectangle to be fusion with
	 */
	void FusionWith(const VRectBase<DataType> &Target) {
		_left   = std::min(_left, Target._left);
		_top    = std::min(_top, Target._top);
		_right  = std::max(_right, Target._right);
		_bottom = std::max(_bottom, Target._bottom);
	}

public:
	/**
	 * The operation of equal judgement for the rectangles
	 * @param Left The rectangle in the left of the expression
	 * @param Right The rectangle in the right of the expression
	 * @return Whether two rectangles are equaled
	 */
	friend bool operator==(const VRectBase<DataType> &Left, const VRectBase<DataType> &Right) {
		return Left._left == Right._left && Left._top == Right._top && Left._right == Right._right && Left._bottom == Right._bottom;
	}
	/**
	 * The operation of equal judgement for rectangle between geometry tuple
	 * @param Tuple The tuple of the geometry, the four parameters obey following sort:
	 * 		[1] Left, [2] Top, [3] Right, [4] Bottom
	 * Basically is { (x1, y1), (x2, y2) }
	 * @param Rectangle The rectangle to be judged
	 * @return Whether the tuple is equaled to the rectangle
	 */
	friend bool operator==(const VRectBase<DataType> &Rectangle, const GeometryTuple &Tuple) {
		auto [left, top, right, bottom] = Tuple;
		return Rectangle._left == left && Rectangle._top == top && Rectangle._right == right && Rectangle._bottom == bottom;
	}
	/**
	 * The operation of equal judgement for rectangle between point tuple
	 * @param Tuple The tuple of point, the first point is the left-top point, the second is
	 * the right-bottom point
	 * @param Rectangle The rectangle to be judged
	 * @return Whether the tuple is equaled to the rectangle
	 */
	friend bool operator==(const VRectBase<DataType> &Rectangle, const PointTuple &Tuple) {
		auto [leftTop, rightBottom] = Tuple;
		return Rectangle._left == leftTop._x && Rectangle._top == leftTop._y && Rectangle._right == rightBottom._x && Rectangle._bottom == rightBottom._y;
	}

	/**
	 * The operation of equal judgement for the rectangles
	 * @param Left The rectangle in the left of the expression
	 * @param Right The rectangle in the right of the expression
	 * @return Whether two rectangles are equaled
	 */
	friend bool operator!=(const VRectBase<DataType> &Left, const VRectBase<DataType> &Right) {
		return Left._left != Right._left && Left._top != Right._top && Left._right != Right._right && Left._bottom != Right._bottom;
	}
	/**
	 * The operation of unequal judgement for rectangle between geometry tuple
	 * @param Tuple The tuple of the geometry, the four parameters obey following sort:
	 * 		[1] Left, [2] Top, [3] Right, [4] Bottom
	 * Basically is { (x1, y1), (x2, y2) }
	 * @param Rectangle The rectangle to be judged
	 * @return Whether the tuple is equaled to the rectangle
	 */
	friend bool operator!=(const VRectBase<DataType> &Rectangle, const GeometryTuple &Tuple) {
		auto [left, top, right, bottom] = Tuple;
		return Rectangle._left != left && Rectangle._top != top && Rectangle._right != right && Rectangle._bottom != bottom;
	}
	/**
	 * The operation of unequal judgement for rectangle between point tuple
	 * @param Tuple The tuple of point, the first point is the left-top point, the second is
	 * the right-bottom point
	 * @param Rectangle The rectangle to be judged
	 * @return Whether the tuple is equaled to the rectangle
	 */
	friend bool operator!=(const VRectBase<DataType> &Rectangle, const PointTuple &Tuple) {
		auto [leftTop, rightBottom] = Tuple;
		return Rectangle._left != leftTop._x && Rectangle._top != leftTop._y && Rectangle._right != rightBottom._x && Rectangle._bottom != rightBottom._y;
	}

private:
	DataType _left;
	DataType _top;
	DataType _right;
	DataType _bottom;
};

/**
 * The alias of VRectBase with the long long data type
 */
using VRect = VRectBase<long long>;
/**
 * The alias of VRectBase with the float data type
 */
using VFloatRect = VRectBase<float>;