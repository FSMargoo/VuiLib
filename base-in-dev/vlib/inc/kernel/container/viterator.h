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
 * \file viterator.h
 * \brief This file contains the definition of various iterator classes.
 */
#pragma once

#include <kernel/container/vtypeextractor.h>

/**
 * \enum VIteratorType
 * \brief An enum class for different types of iterators.
 */
enum class VIteratorType {
	Direction, ///< Direction iterator
	Forward,   ///< Forward iterator
	Input,	   ///< Input iterator
	Output,	   ///< Output iterator
	Random	   ///< Random iterator
};

/**
 * \class VBaseIterator
 * \brief The base class for all iterator types.
 */
template <class Type>
class VBaseIterator {
public:
	/**
	 * Default constructor.
	 * Initializes the index to 0.
	 */
	VBaseIterator() noexcept : Index(0) {
	}

public:
	/**
	 * Returns the current index of the iterator.
	 *
	 * @return The current index.
	 */
	[[nodiscard]] constexpr size_t GetIndex() const {
		return Index;
	}

	/**
	 * Dereference operator.
	 * Must be overridden by derived classes.
	 *
	 * @return A reference to the element pointed to by the iterator.
	 */
	virtual Type &operator*() = 0;

	/**
	 * Returns the type of the iterator.
	 * Must be overridden by derived classes.
	 *
	 * @return The type of the iterator.
	 */
	virtual VIteratorType GetType() = 0;

protected:
	size_t Index; ///< The current index of the iterator.
};

/**
 * \class VOutputIterator
 * \brief A class that represents an output iterator.
 *
 * An output iterator is a type of iterator that can be used to modify the
 * elements it points to. It is typically used in output operations.
 */
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

/**
 * \class VForwardIterator
 * \brief A class that represents a forward iterator.
 *
 * A forward iterator is a type of iterator that can be used to access
 * the container elements sequentially. It can only move one step in the
 * forward direction.
 */
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

/**
 * \class VDirectionIterator
 * \brief A class that represents a direction iterator.
 *
 * A direction iterator is a type of iterator that can be used to access
 * the container elements in both forward and backward direction.
 */
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

/**
 * \class VRandomIterator
 * \brief A class that represents a random iterator.
 *
 * A random iterator is a type of iterator that can be used to access
 * the container elements in any order. It supports a broad range of
 * operations including arithmetic and comparison operations.
 */
template <class Type>
class VRandomIterator : public VBaseIterator<Type> {
public:
	VRandomIterator() noexcept = default;

public:
	virtual Type &operator++()				 = 0;
	virtual Type &operator++(int)			 = 0;
	virtual Type &operator--()				 = 0;
	virtual Type &operator+=(const size_t &) = 0;
	virtual Type &operator-=(const size_t &) = 0;
	virtual Type &operator[](const size_t &) = 0;

	VIteratorType GetType() override {
		return VIteratorType::Random;
	}
};

template <class Type>
struct VRangeType {
	Type Start;
	Type End;

	[[nodiscard]] constexpr bool ValidRange() const {
		return Start <= End;
	}
};

template <class Key, class Map>
struct VMapPair {
	Key OriginKey;
	Map MapValue;
};

using VRange = VRangeType<size_t>;