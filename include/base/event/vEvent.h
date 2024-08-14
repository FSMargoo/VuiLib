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
 * \file vEvent.h
 * \brief The event library
 */

#pragma once

#include <functional>
#include <list>
#include <memory>
#include <mutex>

/***
 * The wrapper of std::function
 */
template <class... Parameters>
using VDelegate = std::function<void(Parameters...)>;

/***
 * The connection unit of VSignal
 * @tparam Parameters The function parameters
 */
template <class... Parameters>
class VConnectUnit {
public:
	/**
	 * Construct the connect unit with the function delegate
	 * @param Function The delegate instance to the function
	 */
	explicit VConnectUnit(VDelegate<Parameters...> Function) : _blocked(false) {
		_function = Function;
	}

public:
	/**
	 * Get the function delegate instance of the object
	 * @return The delegate instance
	 */
	inline VDelegate<Parameters...>* GetFunction() {
		return &_function;
	}
	/**
	 * Check the unit block status
	 * @return If return true, the unit is now on blocked status,
	 * otherwise not
	 */
	bool Blocked() {
		return _blocked;
	}
	/**
	 * Set the unit block status
	 * @param Status If the status is true, this unit will be blocked,
	 * otherwise not
	 */
	void SetBlock(bool Status) {
		_blocked = Status;
	}

private:
	VDelegate<Parameters...> _function;
	bool					 _blocked;
};
/***
 * The function connection unit
 * @tparam Parameters The function parameters
 */
template <class... Parameters>
class VFunctionConnection : public VConnectUnit<Parameters...> {
public:
	/**
	 * Alias of raw function pointer
	 */
	using FunctionPointer = void (*)(Parameters...);

public:
	/**
	 * Construct the function connection unit by a raw C function pointer
	 * @param InitFunction The function pointer
	 */
	explicit VFunctionConnection(FunctionPointer InitFunction)
		: VConnectUnit<Parameters...>(VDelegate<Parameters...>(InitFunction)) {
		_pointer = InitFunction;
	}

	/**
	 * Get the raw C pointer of this unit
	 * @return The function pointer formatted in raw C style
	 */
	inline FunctionPointer GetPointer() {
		return _pointer;
	}

private:
	FunctionPointer _pointer;
};
/***
 * The class based function connection unit
 * @tparam Parameters The function parameters
 */
template <class ObjectClass, class... Parameter>
class VClassConnection : public VConnectUnit<Parameter...> {
public:
	/**
	 * The alisa for member function pointer
	 */
	using FunctionPointer = void (ObjectClass::*)(Parameter...);

public:
	/**
	 * Construct a class connection unit by the object instance and
	 * a member function pointer
	 * @param Object The object pointer referred to the instance
	 * @param Function The member function pointer
	 */
	VClassConnection(ObjectClass *Object, FunctionPointer Function)
		: VConnectUnit<Parameter...>([Object, Function](Parameter... Args) { (*Object.*Function)(Args...); }) {
		_objectPointer = Object;
		_function	   = Function;
	}

	/**
	 * Get the object instance pointer
	 * @return The object instance pointer
	 */
	inline void *GetObject() {
		return _objectPointer;
	}
	/**
	 * Get the raw C pointer of this unit
	 * @return The function pointer formatted in raw C style
	 */
	inline FunctionPointer GetPointer() {
		return _function;
	}

private:
	ObjectClass	   *_objectPointer;
	FunctionPointer _function;
};

/***
 * Event operation type enum class
 */
enum class VEventOperation {
	Lookup,
	Delete,
	Block,
	Unblock
};

/***
 * The event class (Observer mode)
 * @tparam Parameters The event calling parameters type
 */
template <class... Parameters>
class VEvent {
public:
	using FunctionPointer = void (*)(Parameters...);
	template <class ObjectClass>
	using ClassFunctionPointer = void (ObjectClass::*)(Parameters...);

public:
	/**
	 * By default, the VEvent will maintain a list of VConnectionUnit in
	 * std::shared_ptr
	 */
	VEvent() {
		_slots = new std::list<std::shared_ptr<VConnectUnit<Parameters...>>>;
	}
	~VEvent() {
		delete _slots;
	}

public:
	/***
	 * Register handle function
	 * @param Function The function pointer
	 */
	inline void Connect(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		if (!Operation(Function, VEventOperation::Lookup)) {
			std::shared_ptr<VConnectUnit<Parameters...> > pointer(new VFunctionConnection(Function));
			_slots->emplace_back(pointer);
		}
	}
	/***
	 * Register class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template<class ObjectType>
	inline void Connect(ObjectType* Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		if (!Operation(Object, Function, VEventOperation::Lookup)) {
			std::shared_ptr<VConnectUnit<Parameters...> > pointer(new VClassConnection<ObjectType, Parameters...>(Object, Function));
			_slots->emplace_back(pointer);
		}
	}
	/***
	 * Unregister handle function
	 * @param Function The function pointer
	 */
	inline void Disconnect(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Function, VEventOperation::Delete);
	}
	/***
	 * Unregister class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template<class ObjectType>
	inline void Disconnect(ObjectType* Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Object, Function, VEventOperation::Delete);
	}
	/***
	 * Block handle function
	 * @param Function The function pointer
	 */
	inline void Block(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Function, VEventOperation::Block);
	}
	/***
	 * Block class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template<class ObjectType>
	inline void Block(ObjectType* Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Object, Function, VEventOperation::Block);
	}
	/***
	 * Unblock handle function
	 * @param Function The function pointer
	 */
	inline void Unblock(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Function, VEventOperation::Unblock);
	}
	/***
	 * Unblock class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template<class ObjectType>
	inline void Unblock(ObjectType* Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Object, Function, VEventOperation::Unblock);
	}

public:
	/**
	 * Publish the event to event listener
	 * @param Args The args to publish the event
	 */
	void Emit(Parameters... Args) {
		for (auto function = _slots->begin(); function != _slots->end(); ++function) {
			if (function->get()->Blocked()) {
				continue;
			}

			auto pointer = function->get()->GetFunction();

			(*pointer)(Args...);
		}
	}
	/**
	 * Publish the event to event listener
	 * @param Args The args to publish the event
	 */
	void operator()(Parameters... Args) {
		Emit(Args...);
	}

private:
	/***
	 * Conduct Operation of slots for function connection
	 * @return If the target exists in slots, return true.
	 */
	bool Operation(FunctionPointer Function, const VEventOperation &Operation) {
		for (auto iterator = _slots->begin(); iterator != _slots->end();) {
			auto connectFunction = static_cast<VFunctionConnection<Parameters...> *>((*iterator).get());
			if (connectFunction->GetPointer() == Function) {
				switch (Operation) {
				case VEventOperation::Delete: {
					_slots->erase(iterator++);

					break;
				}
				case VEventOperation::Block: {
					connectFunction->SetBlock(true);
					++iterator;

					break;
				}
				case VEventOperation ::Unblock: {
					connectFunction->SetBlock(false);
					++iterator;

					break;
				}
				default: {
					break;
				}
				}

				return true;
			}
			else {
				++iterator;
			}
		}
		return false;
	}
	/***
	 * Conduct Operation of slots for class connection
	 * @return If the target exists in slots, return true.
	 */
 	template<class ObjectType>
	bool Operation(ObjectType* Object, ClassFunctionPointer<ObjectType> Function, const VEventOperation &Operation) {
		for (auto iterator = _slots->begin(); iterator != _slots->end();) {
			auto connectFunction = static_cast<VClassConnection<ObjectType, Parameters...> *>((*iterator).get());
			if (connectFunction->GetPointer() == Function && connectFunction->GetObject() == Object) {
				switch (Operation) {
				case VEventOperation::Delete: {
					_slots->erase(iterator++);

					break;
				}
				case VEventOperation::Block: {
					connectFunction->SetBlock(true);
					++iterator;

					break;
				}
				case VEventOperation ::Unblock: {
					connectFunction->SetBlock(false);
					++iterator;

					break;
				}
				default: {
					break;
				}
				}

				return true;
			}
			else {
				++iterator;
			}
		}
		return false;
	}

private:
	std::mutex												 _lock;
	std::list<std::shared_ptr<VConnectUnit<Parameters...>>> *_slots;
};