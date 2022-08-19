// License(MIT)
// Athuor: Margoo
// This is the signal system in vuilib
#pragma once

#include "../vbasiccore.h"

#include <functional>
#include <memory>
#include <list>

VLIB_BEGIN_NAMESPACE

namespace Core {

template<class... Type>
class connect_basic {
private:
	std::function<void(Type...)> call_function;
	bool                         be_blocked = false;

public:
	explicit connect_basic(std::function<void(Type...)> function) {
		call_function = function;
	}
	inline std::function<void(Type...)>* get_function() {
		return &call_function;
	}

	bool is_block() {
		return be_blocked;
	}
	void set_block(bool stats) {
		be_blocked = stats;
	}
};
template<class... Type>
class connect_functional : public connect_basic<Type...> {
public:
	using functional_ptr = void(*)(Type...);

private:
	functional_ptr functional;

public:
	explicit connect_functional(functional_ptr init_function)
		: connect_basic<Type...>(std::function<void(Type...)>(init_function)) {
		functional = init_function;
	}

	inline functional_ptr get_raw_function() {
		return functional;
	}
};

template<class ObjectType, class... Type>
class connection : public connect_basic<Type ...> {
public:
	using object_functional_ptr = void (ObjectType::*)(Type ...);

private:
	ObjectType* object_ref;
	object_functional_ptr object_functional;

public:
	connection(ObjectType* object, object_functional_ptr functional)
		: connect_basic<Type...>([object, functional](Type... args) { (*object.*functional)(args...); }) {
		object_ref = object;
		object_functional = functional;
	}

	inline void* get_raw_object() {
		return object_ref;
	}
	inline object_functional_ptr
		get_raw_call() {
		return object_functional;
	}
};

template<class... Type>
class VSignal {
private:
	std::list<std::shared_ptr<connect_basic<Type...> > >
		slots;

private:
	void _operator(void (*functional)(Type...), int operator_stage) {
		for (auto iterator = slots.begin(); iterator != slots.end();) {
			connect_functional<Type...>* connect_function = static_cast<connect_functional<Type...>*>((*iterator).get());

			if (connect_function->get_raw_function() == functional) {
				switch (operator_stage) {
				case 0: {
					slots.erase(iterator++);

					break;
				}
				case 1: {
					connect_function->set_block(true);

					++iterator;

					break;
				}
				case 2: {
					connect_function->set_block(false);

					++iterator;

					break;
				}
				}
			}
			else
			{
				++iterator;
			}
		}
	}
	template<class ObjectType>
	void _operator(ObjectType* object, void (ObjectType::* object_function)(Type...), int opreator_stage) {
		for (auto iterator = slots.begin(); iterator != slots.end(); ) {
			if (iterator->get()->is_block() == true) {
				continue;
			}

			connection<ObjectType, Type...>* connect = static_cast<connection<ObjectType, Type...>*>((*iterator).get());

			if (connect->get_raw_call() == object_function &&
				connect->get_raw_object() == object) {
				switch (opreator_stage) {
				case 0: {
					slots.erase(iterator++);

					break;
				}
				case 1: {
					connect->set_block(true);

					++iterator;

					break;
				}
				case 2: {
					connect->set_block(false);

					++iterator;

					break;
				}
				}
			}
			else
			{
				++iterator;
			}
		}
	}

public:
	inline void Connect(void (*functional)(Type...)) {
		std::shared_ptr<connect_functional<Type...>> functional_ptr
		(new connect_functional<Type...>(functional));
		slots.push_back(functional_ptr);
	}
	template<class ObjectType>
	inline void Connect(ObjectType* object, void (ObjectType::* functional)(Type...)) {
		std::shared_ptr<connection<ObjectType, Type...> > functional_ptr
		(new connection<ObjectType, Type...>(object, functional));

		slots.push_back(functional_ptr);
	}

	inline void Disconnect(void (*functional)(Type...)) {
		_operator(functional, 0);
	}
	template<class ObjectType>
	inline void Disconnect(ObjectType* object, void (ObjectType::* functional)(Type...)) {
		_operator(object, functional, 0);
	}

	void Block(void (*functional)(Type ...), bool block_stats) {
		_operator(functional, block_stats ? 1 : 2);
	}
	template<class ObjectType>
	void Block(ObjectType* object, void (ObjectType::* functional)(Type ...), bool block_stats) {
		_operator(object, functional, block_stats ? 1 : 2);
	}

	void Emit(Type... args) {
		for (auto iterator = slots.begin(); iterator != slots.end(); ++iterator) {
			if (iterator->get()->is_block() == true) {
				continue;
			}

			auto call_function = iterator->get()->get_function();

			(*call_function)(args...);
		}
	}
};

}

VLIB_END_NAMESPACE