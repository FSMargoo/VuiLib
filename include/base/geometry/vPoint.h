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
 * \file vPoint.h
 * \brief The point data structure of geometry information
 */

#pragma once

#include <tuple>

/**
 * The abstract class for the point data.
 * @tparam DataType The type of the data, int, double, float e.t.c
 */
template <class DataType>
class VPointBase {
public:
	/**
	 * The axis tuple alias, the first element of axis tuple
	 * is X, the second element of the axis tuple is Y
	 */
	using AxisTuple = std::tuple<DataType, DataType>;

public:
	/**
	 * Construct a point by axis tuple
	 * @param Tuple The axis tuple, the first element of axis tuple
	 * is X, the second element of the axis tuple is Y
	 */
	explicit VPointBase(const AxisTuple &Tuple) : _x(std::get<0>(Tuple)), _y(std::get<1>(Tuple)) {

	}
	/**
	 * Construct the point with specified geometry data
	 * @param X The X axis data
	 * @param Y The Y axis data
	 */
	VPointBase(const DataType &X, const DataType &Y) : _x(X), _y(Y) {
	}
	/**
	 * Construct the point in default, it will create a point
	 * locate at (0, 0), the construct of zero will pas the
	 * construct function of the DataType class
	 */
	VPointBase() : _x(DataType(0)), _y(DataType(0)) {
	}

public:
	/**
	 * Get the two binaries tuple with the type of DataType.
	 * The first element of tuple is the X axis, the second is Y axis
	 * @return The tuple of DataType
	 */
	AxisTuple GetTuple() {
		return {_x, _y};
	}
	/**
	 * Get the X axis of the
	 * @return The X axis data
	 */
	DataType GetX() const {
		return _x;
	}
	/**
	 * Get the Y axis of the
	 * @return The Y axis data
	 */
	DataType GetY() const {
		return _y;
	}
	/**
	 * Set the X axis data
	 * @param X The new X axis data
	 */
	void SetX(const DataType &X) {
		_x = X;
	}
	/**
	 * Set the Y axis data
	 * @param Y The new Y axis data
	 */
	void SetY(const DataType &Y) {
		_y = Y;
	}
	/**
	 * Set the axis data by a tuple
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 */
	void SetAxisTuple(const AxisTuple &Tuple) {
		_x = std::get<0>(Tuple);
		_y = std::get<1>(Tuple);
	}

public:
	/**
	 * The operation of equal judgement for the points
	 * @param Left The point in the left of the expression
	 * @param Right The point in the right of the expression
	 * @return Whether two points are equaled
	 */
	friend bool operator==(const VPointBase<DataType>& Left, const VPointBase<DataType> &Right) {
		return Left._x == Right._x && Left._y == Right._y;
	}
	/**
	 * The operation of equal judgement for point between axis tuple
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @param Point The point to be judged
	 * @return Whether the tuple is equaled to the point
	 */
	friend bool operator==(const AxisTuple& Tuple, const VPointBase<DataType> &Point) {
		auto [x, y] = Tuple;
		return Point._x == x && Point._y == y;
	}
	/**
	 * The operation of equal judgement for point between axis tuple
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @param Point The point to be judged
	 * @return Whether the tuple is equaled to the point
	 */
	friend bool operator==(const VPointBase<DataType> &Point, const AxisTuple& Tuple) {
		auto [x, y] = Tuple;
		return Point._x == x && Point._y == y;
	}

	/**
	 * The operation of unequal judgement for the points
	 * @param Left The point in the left of the expression
	 * @param Right The point in the right of the expression
	 * @return Whether two points are equaled
	 */
	friend bool operator!=(const VPointBase<DataType>& Left, const VPointBase<DataType> &Right) {
		return Left._x != Right._x || Left._y != Right._y;
	}
	/**
	 * The operation of unequal judgement for point between axis tuple
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @param Point The point to be judged
	 * @return Whether the tuple is equaled to the point
	 */
	friend bool operator!=(const AxisTuple& Tuple, const VPointBase<DataType> &Point) {
		auto [x, y] = Tuple;
		return Point._x != x || Point._y != y;
	}
	/**
	 * The operation of unequal judgement for point between axis tuple
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @param Point The point to be judged
	 * @return Whether the tuple is equaled to the point
	 */
	friend bool operator!=(const VPointBase<DataType> &Point, const AxisTuple& Tuple) {
		auto [x, y] = Tuple;
		return Point._x != x || Point._y != y;
	}

	/**
	 * Adding point with offset, basically, it will add the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator+(const VPointBase<DataType> &Point, const DataType &Offset) {
		return VPointBase<DataType>(Point._x + Offset, Point._y + Offset);
	}
	/**
	 * Adding point with offset, basically, it will add the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator+(const DataType &Offset, const VPointBase<DataType> &Point) {
		return VPointBase<DataType>(Point._x + Offset, Point._y + Offset);
	}
	/**
	 * Subtracting point with offset, basically, it will subtract the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator-(const VPointBase<DataType> &Point, const DataType &Offset) {
		return VPointBase<DataType>(Offset - Point._x, Offset - Point._y);
	}
	/**
	 * Subtracting point with offset, basically, it will subtract the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator-(const DataType &Offset, const VPointBase<DataType> &Point) {
		return VPointBase<DataType>(Offset - Point._x, Offset - Point._y);
	}
	/**
	 * Multiplying point with offset, basically, it will multiply the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator*(const VPointBase<DataType> &Point, const DataType &Offset) {
		return VPointBase<DataType>(Offset * Point._x, Offset * Point._y);
	}
	/**
	 * Multiplying point with offset, basically, it will multiply the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator*(const DataType &Offset, const VPointBase<DataType> &Point) {
		return VPointBase<DataType>(Offset * Point._x, Offset * Point._y);
	}
	/**
	 * Dividing point with offset, basically, it will divide the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator/(const VPointBase<DataType> &Point, const DataType &Offset) {
		return VPointBase<DataType>(Offset / Point._x, Offset / Point._y);
	}
	/**
	 * Dividing point with offset, basically, it will divide the X,Y axis by the offset
	 * @param Point The point in the left of the expression
	 * @param Offset The offset in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator/(const DataType &Offset, const VPointBase<DataType> &Point) {
		return VPointBase<DataType>(Offset / Point._x, Offset / Point._y);
	}

	/**
	 * Adding two point, basically, it will add the X,Y axis of two points
	 * @param Left The point in the left of the expression
	 * @param Right The point in the right of the expression
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator+(const VPointBase<DataType> &Left, const VPointBase<DataType> &Right) {
		return VPointBase<DataType>(Left._x + Right._x, Left._y + Right.y);
	}
	/**
	 * Adding point and axis tuple, basically, it will add the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator+(const VPointBase<DataType> &Point, const AxisTuple &Tuple) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(Point._x + x, Point._y + y);
	}
	/**
	 * Adding point and axis tuple, basically, it will add the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The add operation result
	 */
	friend VPointBase<DataType> operator+(const AxisTuple &Tuple, const VPointBase<DataType> &Point) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(Point._x + x, Point._y + y);
	}
	
	/**
	 * Subtracting two point, basically, it will subtract the X,Y axis of two points
	 * @param Left The point in the left of the expression
	 * @param Right The point in the right of the expression
	 * @return The subtract operation result
	 */
	friend VPointBase<DataType> operator-(const VPointBase<DataType> &Left, const VPointBase<DataType> &Right) {
		return VPointBase<DataType>(Left._x - Right._x, Left._y - Right.y);
	}
	/**
	 * Subtracting point and axis tuple, basically, it will subtract the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The subtract operation result
	 */
	friend VPointBase<DataType> operator-(const VPointBase<DataType> &Point, const AxisTuple &Tuple) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(Point._x - x, Point._y - y);
	}
	/**
	 * Subtracting point and axis tuple, basically, it will subtract the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The subtract operation result
	 */
	friend VPointBase<DataType> operator-(const AxisTuple &Tuple, const VPointBase<DataType> &Point) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(x - Point._x, y - Point._y);
	}
	
	/**
	 * Multiplying two point, basically, it will multiply the X,Y axis of two points
	 * @param Left The point in the left of the expression
	 * @param Right The point in the right of the expression
	 * @return The multiply operation result
	 */
	friend VPointBase<DataType> operator*(const VPointBase<DataType> &Left, const VPointBase<DataType> &Right) {
		return VPointBase<DataType>(Left._x * Right._x, Left._y * Right.y);
	}
	/**
	 * Multiplying point and axis tuple, basically, it will multiply the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The multiply operation result
	 */
	friend VPointBase<DataType> operator*(const VPointBase<DataType> &Point, const AxisTuple &Tuple) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(Point._x * x, Point._y * y);
	}
	/**
	 * Multiplying point and axis tuple, basically, it will multiply the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The multiply operation result
	 */
	friend VPointBase<DataType> operator*(const AxisTuple &Tuple, const VPointBase<DataType> &Point) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(Point._x * x, Point._y * y);
	}
	
	/**
	 * Dividing two point, basically, it will divide the X,Y axis of two points
	 * @param Left The point in the left of the expression
	 * @param Right The point in the right of the expression
	 * @return The divide operation result
	 */
	friend VPointBase<DataType> operator/(const VPointBase<DataType> &Left, const VPointBase<DataType> &Right) {
		return VPointBase<DataType>(Left._x / Right._x, Left._y / Right.y);
	}
	/**
	 * Dividing point and axis tuple, basically, it will divide the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The divide operation result
	 */
	friend VPointBase<DataType> operator/(const VPointBase<DataType> &Point, const AxisTuple &Tuple) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(Point._x / x, Point._y / y);
	}
	/**
	 * Dividing point and axis tuple, basically, it will divide the X,Y axis between point the axis tuple
	 * @param Point The point in the left of the expression
	 * @param Tuple The axis tuple, the first element of the tuple is
	 * the X axis with the second is X axis.
	 * @return The divide operation result
	 */
	friend VPointBase<DataType> operator/(const AxisTuple &Tuple, const VPointBase<DataType> &Point) {
		auto [x, y] = Tuple;
		return VPointBase<DataType>(x / Point._x, y / Point._y);
	}

private:
	template<class T>
	friend class VRectBase;

private:
	DataType _x;
	DataType _y;
};

/**
 * The alias of VPointBase with the long long data type
 */
using VPoint = VPointBase<long long>;
/**
 * The alis of VPointBase with the float data type
 */
using VFloatPoint = VPointBase<float>;