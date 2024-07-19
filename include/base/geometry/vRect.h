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

/**
 * The abstract class for the rectangle data.
 * @tparam DataType The type of the data, int, double, float e.t.c
 */
template <class DataType>
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
	VRectBase(const Point &Tuple) : VRectBase(std::get<0>(Tuple), std::get<1>(Tuple)) {
	}
	/**
	 * Create a rectangle by the geometry tuple
	 * @param Tuple The tuple of geometry. The tuple of the geometry, the four
	 * parameters obey following sort:
	 * 		[1] Left, [2] Top, [3] Right, [4] Bottom
	 * Basically is { (x1, y1), (x2, y2) }
	 */
	VRectBase(const GeometryTuple &Tuple)
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
	 * @param Target The target rectangle
	 */
	 bool IsInclude(const VRectBase &Target) {
		return Target._left <= _left && Target._top <= _top && Target._right >= _right && Target._bottom >= _bottom;
	 }

private:
	DataType _left;
	DataType _top;
	DataType _right;
	DataType _bottom;
};