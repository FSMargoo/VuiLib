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

#include <kernel/container/vtypeextractor.h>

enum class VIteratorType {
	Direction,
	Forward,
	Input,
	Output,
	Random
};

template <class Type>
class VBaseIterator {
public:
	VBaseIterator() noexcept : Index(0) {
	}

public:
	[[nodiscard]] constexpr size_t GetIndex() const {
		return Index;
	}

	virtual Type &operator*() = 0;

	virtual VIteratorType GetType() = 0;

protected:
	size_t Index;
};

template <class Type>
class VInputIterator : public VBaseIterator<Type> {
public:
	VInputIterator() noexcept = default;

public:
	virtual Type &operator++()	  = 0;
	virtual Type &operator++(int) = 0;

	virtual bool operator==(const VInputIterator<Type> &) = 0;
	virtual bool operator!=(const VInputIterator<Type> &) = 0;

	VIteratorType GetType() override {
		return VIteratorType::Input;
	}
};

template <class Type>
class VOutputIterator : public VBaseIterator<Type> {
public:
	VOutputIterator() noexcept = default;

public:
	virtual Type &operator++()	  = 0;
	virtual Type &operator++(int) = 0;

	VIteratorType GetType() override {
		return VIteratorType::Output;
	}
};

template <class Type>
class VForwardIterator : public VBaseIterator<Type> {
public:
	VForwardIterator() noexcept = default;

public:
	virtual Type	   &operator++()							   = 0;
	virtual const Type &operator++(int)							   = 0;
	virtual bool		operator==(const VForwardIterator<Type> &) = 0;
	virtual bool		operator!=(const VForwardIterator<Type> &) = 0;

	VIteratorType GetType() override {
		return VIteratorType::Forward;
	}
};

template <class Type>
class VDirectionIterator : public VBaseIterator<Type> {
public:
	VDirectionIterator() noexcept = default;

public:
	virtual Type &operator++()								   = 0;
	virtual Type &operator++(int)							   = 0;
	virtual Type &operator--()								   = 0;
	virtual Type &operator*()								   = 0;
	virtual bool  operator==(const VDirectionIterator<Type> &) = 0;
	virtual bool  operator!=(const VDirectionIterator<Type> &) = 0;

	VIteratorType GetType() override {
		return VIteratorType::Direction;
	}
};

template <class Type>
class VRandomIterator : public VBaseIterator<Type> {
public:
	VRandomIterator() noexcept = default;

public:
	virtual Type				  &operator++()								 = 0;
	virtual Type				  &operator++(int)							 = 0;
	virtual Type				  &operator--()								 = 0;
	virtual Type				  &operator*()								 = 0;
	virtual VRandomIterator<Type> &operator+=(const size_t &)				 = 0;
	virtual VRandomIterator<Type> &operator-=(const size_t &)				 = 0;
	virtual bool				   operator==(const VRandomIterator<Type> &) = 0;
	virtual bool				   operator!=(const VRandomIterator<Type> &) = 0;
	virtual Type				  &operator[](const size_t &)				 = 0;

	VIteratorType GetType() override {
		return VIteratorType::Random;
	}
};