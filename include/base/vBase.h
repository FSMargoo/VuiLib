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
 * \file vBase.h
 * \brief The base library in VUILib
 */

#pragma once

#include <base/binding/vBindingType.h>
#include <base/command/vCommand.h>
#include <base/test/vTest.h>

#include <OpenString-CMake/include/format.h>
#include <OpenString-CMake/include/text.h>

#pragma warning(disable : 4244)

namespace std {
/**
 * The template specialization for std::hash in order to provide for unordered_map key
 */
template <> struct hash<ostr::text> {
	_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef ostr::text argument_type;
	_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef size_t	 result_type;
	_NODISCARD size_t									 operator()(const ostr::text _Keyval) const noexcept {
		   return hash<string>().operator()(_Keyval.c_str());
	}
};
} // namespace std

using OString	  = ostr::text;
using OStringView = ostr::text_view;

namespace VConcept {
template <template <typename...> class C, typename... Ts> std::true_type IsBaseOfImpl(const C<Ts...> *);

template <template <typename...> class C> std::false_type IsBaseOfImpl(...);

template <template <typename...> class C, typename T> using IsBaseOf = decltype(IsBaseOfImpl<C>(std::declval<T *>()));
} // namespace VConcept

template <class RangeType>
	requires std::is_integral_v<RangeType> or std::is_floating_point_v<RangeType>
std::vector<RangeType> VRange(const RangeType &Start, const RangeType &End, const RangeType &Step = RangeType(1)) {
	std::vector<RangeType> result;
	for (RangeType count = Start; count < End; count += Step) {
		result.emplace_back(count);
	}

	return result;
}