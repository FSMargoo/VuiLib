#pragma once

#include "kernel/mem/vmemrbtree.h"

template <class Type, class Allocator>
class VConstantPool {
public:
	VConstantPool(Allocator &BindPool) : Pool(BindPool), Mapping(BindPool) {
	}
	void *FindOrInsert(const Type &Value) {
		if (Mapping.Exists(Value)) {
			return Mapping.Find(Value);
		}

		Mapping.Insert(Value, Pool.Allocate<Type>(Value));
		return Mapping.Find(Value);
	}

private:
	Allocator							   &Pool;
	_VMem::VRBTree<Type, void *, Allocator> Mapping;
};

template <class Allocator>
struct VConstantExtractor {
	using IntPool	 = VConstantPool<int, Allocator>;
	using DoublePool = VConstantPool<double, Allocator>;
	using FloatPool	 = VConstantPool<float, Allocator>;
};