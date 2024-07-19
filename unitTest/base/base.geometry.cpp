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
 * \file base.point.cpp
 * \brief The unit test module for geometry module
 */

#include <unitTest/base/base.geometry.h>

bool VUnitTest(BaseGeometryPoint, Init) {
	VPoint defaultConstruction;
	VPoint tupleConstruction(VPoint::AxisTuple{10, 10});
	VPoint parameterConstruction(12, 12);
	VPoint copyConstruction(parameterConstruction);
	return defaultConstruction.GetTuple() == VPoint::AxisTuple{0, 0} &&
		   tupleConstruction.GetTuple() == VPoint::AxisTuple{10, 10} &&
		   parameterConstruction.GetTuple() == VPoint::AxisTuple{12, 12} &&
		   parameterConstruction.GetTuple() == VPoint::AxisTuple{12, 12};
}
bool VUnitTest(BaseGeometryPoint, Calculate) {
	VPoint point{0, 0};
	point = point + 2;
	point = point / 2;
	point = point + VPoint::AxisTuple{20, 3};
	point = point * 3;
	point = VPoint::AxisTuple{100, 120} - point;

	return point.GetTuple() == VPoint::AxisTuple{37, 108};
}
bool VUnitTest(BaseGeometryPoint, Compare) {
	return !(VPoint(2012, 2013) == VPoint(2014, 2015)) && (VPoint(2016, 2017) == VPoint(2016, 2017));
}