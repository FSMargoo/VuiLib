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
 * \file vmemrbtree.h
 * \brief This file def a RBT data structure for the constant pool and the VMap
 */

#pragma once

namespace _VMem {

enum class VRBTNodeColor {
	Black,
	Red
};

/**
 * \brief The RBT tree's node.
 * \tparam KeyType Key type of the
 * \tparam Pair The type of the value
 */
template <class KeyType, class Pair>
struct VRBTNode {
	VRBTNodeColor Color;

	VRBTNode *Left;
	VRBTNode *Right;
	VRBTNode *Parent;

	KeyType Key;
	Pair	Context;

	VRBTNode(const VRBTNodeColor &NodeColor, VRBTNode *NilPtr)
		: Left(NilPtr), Right(NilPtr), Parent(nullptr), Color(NodeColor) {
	}
	VRBTNode(const VRBTNodeColor &NodeColor, const KeyType &KeyValue, const Pair &ContextValue, VRBTNode *NilPtr)
		: Left(NilPtr), Right(NilPtr), Parent(nullptr), Color(NodeColor), Key(KeyValue), Context(ContextValue) {
	}
	~VRBTNode() {
	}

	void FreeSelf() {
		if (!Left->IsNil()) {
			Left->FreeSelf();
			delete Left;
			Left = nullptr;
		}
		if (!Right->IsNil()) {
			Right->FreeSelf();
			delete Right;
			Right = nullptr;
		}
	}

	virtual bool IsNil() {
		return false;
	}
};
/**
 * \brief The nil node of RBT, it will be unique in one RBT
 * \tparam Key The type of key
 * \tparam Pair The type of value
 */
template <class Key, class Pair>
struct VRBTNilNode : public VRBTNode<Key, Pair> {
	VRBTNilNode() : VRBTNode<Key, Pair>(VRBTNodeColor::Black, nullptr) {
		this->Left	= this;
		this->Right = this;
	}
	~VRBTNilNode() {
		this->Left	= nullptr;
		this->Right = nullptr;
	}
	bool IsNil() override {
		return true;
	}
};

/**
 * \brief The key extractor of RBT (The alias of some type)
 * \tparam Key The key type
 * \tparam Pair The value type
 */
template <class Key, class Pair>
class VRBTTypeExtract {
public:
	using Pointer = Pair *;
	using Node	  = VRBTNode<Key, Pair>;
	template <class Type>
	struct RValue {
		using T = Type &&;
	};
	template <class Type>
	struct LValue {
		using T = Type &;
	};

	static Pointer _MakePointer(Node &SpecifiedNode) {
		return (Pointer)(&(SpecifiedNode.Context));
	}
};

/**
 * \brief The RBT tree
 * \tparam Key The type of key
 * \tparam Pair The type of value
 * \tparam RBTAllocator The allocator type
 * \tparam TypeExtract The type extractor
 */
template <class Key, class Pair, class RBTAllocator, class TypeExtract = VRBTTypeExtract<Key, Pair>>
class VRBTree {
public:
	using Node	  = VRBTNode<Key, Pair>;
	using Pointer = TypeExtract::Pointer;
	template <class Type>
	using RValue = VRBTTypeExtract<Key, Pair>::RValue<Type>::T;
	template <class Type>
	using LValue = VRBTTypeExtract<Key, Pair>::LValue<Type>::T;
	template <class T>
	using AllocatorPointer = T *(RBTAllocator::*)();

public:
	VRBTree(RBTAllocator &TreeAllocator) : Allocator(TreeAllocator) {
		Nil	 = TreeAllocator.template Allocate<VRBTNilNode<Key, Pair>>();
		Root = nullptr;
	}
	~VRBTree() {
		if (Root != nullptr) {
			Root->FreeSelf();
			delete Root;
		}

		Allocator.Delete(Nil);
	}

public:
	/**
	 * \brief Insert a element
	 * \param KeyValue
	 * \param PairValue
	 */
	void Insert(const Key &KeyValue, const Pair &PairValue) {
		Node *InsertNode = _AssumedRedNode(KeyValue, PairValue);
		if (Root == nullptr) {
			_InitRootNode(InsertNode);
		} else {
			_NormalInsert(InsertNode);
		}

		_InsertFixUp(InsertNode);
	}
	/**
	 * \brief Delete a element
	 * \param KeyValue
	 */
	void Delete(const Key &KeyValue) {
		Node *DeleteNode = _FindNode(KeyValue);
		if (DeleteNode == nullptr) {
			return;
		}

		_BSTDelete(DeleteNode);
	}
	/**
	 * \brief Find the key is existing or not
	 * \param KeyValue The value of key
	 * \return If exists, return true, nor return false
	 */
	bool Exists(const Key &KeyValue) {
		return _FindNode(KeyValue) != nullptr;
	}
	VRBTTypeExtract<Key, Pair>::template LValue<Pair>::T Find(const Key &KeyValue) {
		return _FindNode(KeyValue)->Context;
	}

public:
	/**
	 * \brief Is the RBT empty or not
	 * \return Whether the RBT empty or not
	 */
	bool Empty() {
		return Root == nullptr;
	}

private:
	void _InsertFixUp(Node *Local) {
		while (Local->Parent->Color != VRBTNodeColor::Black) {
			Node *Uncle;
			if (Local->Parent == Local->Parent->Parent->Right) {
				Uncle = Local->Parent->Parent->Left;
				if (Uncle->Color == VRBTNodeColor::Red) {
					Local->Parent->Color		 = VRBTNodeColor::Black;
					Uncle->Color				 = VRBTNodeColor::Black;
					Local->Parent->Parent->Color = VRBTNodeColor::Red;
					Local						 = Local->Parent->Parent;
				} else {
					if (Local == Local->Parent->Left) {
						Local = Local->Parent;
						_RightRotation(Local);
					}

					Local->Parent->Color		 = VRBTNodeColor::Black;
					Local->Parent->Parent->Color = VRBTNodeColor::Red;
					_LeftRotation(Local->Parent->Parent);
				}
			} else {
				Uncle = Local->Parent->Parent->Right;
				if (Uncle->Color == VRBTNodeColor::Red) {
					Local->Parent->Color		 = VRBTNodeColor::Black;
					Uncle->Color				 = VRBTNodeColor::Black;
					Local->Parent->Parent->Color = VRBTNodeColor::Red;
					Local						 = Local->Parent->Parent;
				} else {
					if (Local == Local->Parent->Right) {
						Local->Parent = Local->Parent;
						Local		  = Local->Parent;
						_LeftRotation(Local);
					}

					Local->Parent->Color		 = VRBTNodeColor::Black;
					Local->Parent->Parent->Color = VRBTNodeColor::Red;
					_RightRotation(Local->Parent->Parent);
				}
			}
		}

		Root->Color = VRBTNodeColor::Black;
	}
	void _DeleteFixUp(Node *Local) {
		while (Local != Root && Local->Color == VRBTNodeColor::Black) {
			if (Local == Local->Parent->Left) {
				Node *Sibling = Local->Parent->Right;
				if (Sibling->Color == VRBTNodeColor::Red) {
					Sibling->Color		 = VRBTNodeColor::Black;
					Local->Parent->Color = VRBTNodeColor::Red;

					_LeftRotation(Local->Parent);
					Sibling = Local->Parent->Right;
				}
				if (Sibling->Left->Color == VRBTNodeColor::Black && Sibling->Right->Color == VRBTNodeColor::Black) {
					Sibling->Color = VRBTNodeColor::Red;
					Local		   = Local->Parent;
				} else {
					if (Sibling->Right->Color == VRBTNodeColor::Black) {
						Sibling->Left->Color = VRBTNodeColor::Black;
						Sibling->Color		 = VRBTNodeColor::Red;

						_RightRotation(Sibling);
						Sibling = Local->Parent->Right;
					}

					Sibling->Color		  = Local->Parent->Color;
					Local->Parent->Color  = VRBTNodeColor::Black;
					Sibling->Right->Color = VRBTNodeColor::Black;

					_LeftRotation(Local->Parent);
					Local = Root;
				}
			} else {
				Node *Sibling = Local->Parent->Left;
				if (Sibling->Color == VRBTNodeColor::Red) {
					Sibling->Color		 = VRBTNodeColor::Black;
					Local->Parent->Color = VRBTNodeColor::Red;

					_RightRotation(Local->Parent);
					Sibling = Local->Parent->Left;
				}
				if (Sibling->Left->Color == VRBTNodeColor::Black && Sibling->Right->Color == VRBTNodeColor::Black) {
					Sibling->Color = VRBTNodeColor::Red;
					Local		   = Local->Parent;
				} else {
					if (Sibling->Left->Color == VRBTNodeColor::Black) {
						Sibling->Right->Color = VRBTNodeColor::Black;
						Sibling->Color		  = VRBTNodeColor::Red;

						_LeftRotation(Sibling);
						Sibling = Local->Parent->Left;
					}

					Sibling->Color		 = Local->Parent->Color;
					Local->Parent->Color = VRBTNodeColor::Black;
					Sibling->Left->Color = VRBTNodeColor::Black;

					_RightRotation(Local->Parent);
					Local = Root;
				}
			}
		}

		Local->Color = VRBTNodeColor::Black;
	}
	void _BSTDelete(Node *DeleteNode) {
		Node *Target = _GetDeleteTarget(DeleteNode);
		Node *Children;
		if (!Target->Left->IsNil()) {
			Children = Target->Left;
		} else {
			Children = Target->Right;
		}

		Children->Parent = Target->Parent;

		if (Target->Parent->IsNil()) {
			Root = Children;
		} else if (Target == Target->Parent->Left) {
			Target->Parent->Left = Children;
		} else {
			Target->Parent->Right = Children;
		}

		if (Target != DeleteNode) {
			_NodeMoveAssign(DeleteNode, Target);
		}

		if (Target->Color == VRBTNodeColor::Black) {
			_DeleteFixUp(Children);
		}
	}

private:
	void _InitRootNode(Node *InsertNode) {
		Root		 = InsertNode;
		Root->Color	 = VRBTNodeColor::Black;
		Root->Parent = Nil;
	}
	void _NormalInsert(Node *InsertNode) {
		Node *Ptr	 = Root;
		Node *Parent = Root;

		while (!Ptr->IsNil()) {
			if (InsertNode->Key > Ptr->Key) {
				Parent = Ptr;
				Ptr	   = Ptr->Right;
			} else if (InsertNode->Key < Ptr->Key) {
				Parent = Ptr;
				Ptr	   = Ptr->Left;
			} else {
				return;
			}
		}

		if (Parent->Key > InsertNode->Key) {
			Parent->Left = InsertNode;
		} else {
			Parent->Right = InsertNode;
		}

		InsertNode->Parent = Parent;
	}
	void _NodeMoveAssign(Node *Target, Node *Source) {
		Target->Key		= _MakeRValue<Key>(Source->Key);
		Target->Context = _MakeRValue<Pair>(Source->Context);
	}

private:
	Node *_FindSuccessor(Node *SpecifiedNode) {
		if (!SpecifiedNode->Right->IsNil()) {
			while (!SpecifiedNode->Left->IsNil()) {
				SpecifiedNode = SpecifiedNode->Left;
			}

			return SpecifiedNode;
		}
		Node *Ptr = SpecifiedNode->Parent;
		while (!Ptr->IsNil() && SpecifiedNode == Ptr->Right) {
			SpecifiedNode = Ptr;
			Ptr			  = Ptr->Parent;
		}

		return Ptr;
	}
	Node *_FindNode(const Key &KeyValue) {
		Node *Ptr = Root;
		if (Ptr == nullptr) {
			return Ptr;
		}

		while (!Ptr->IsNil()) {
			if (KeyValue > Ptr->Key) {
				Ptr = Ptr->Right;
			} else if (KeyValue < Ptr->Key) {
				Ptr = Ptr->Left;
			} else {
				return Ptr;
			}
		}

		return nullptr;
	}
	Node *_AssumedRedNode(const Key &KeyValue, const Pair &PairValue) {
		return new Node(VRBTNodeColor::Red, KeyValue, PairValue, Nil);
	}
	Node *_GetDeleteTarget(Node *DeleteNode) {
		if (DeleteNode->Left->IsNil() || DeleteNode->Right->IsNil()) {
			return DeleteNode;
		} else {
			return _FindSuccessor(DeleteNode);
		}
	}

private:
	Pointer _MakePointer(Node &SpecifiedNode) {
		return TypeExtract::_MakePointer(SpecifiedNode);
	}
	template <class Type>
	VRBTTypeExtract<Key, Pair>::RValue<Type>::T _MakeRValue(Type &&Value) {
		return static_cast<RValue<Type>>(Value);
	}
	template <class Type>
	VRBTTypeExtract<Key, Pair>::template LValue<Type>::T _MakeLValue(Type Value) {
		return static_cast<VRBTTypeExtract<Key, Pair>::template LValue<Type>::T>(Value);
	}

private:
	void _LeftRotation(Node *Rotation) {
		Node *Right		= Rotation->Right;
		Rotation->Right = Right->Left;

		if (!Right->Left->IsNil()) {
			Right->Left->Parent = Rotation;
		}

		Right->Parent = Rotation->Parent;

		if (Rotation->Parent->IsNil()) {
			Root = Right;
		} else if (Rotation == Rotation->Parent->Left) {
			Rotation->Parent->Left = Right;
		} else {
			Rotation->Parent->Right = Right;
		}

		Right->Left		 = Rotation;
		Rotation->Parent = Right;
	}
	void _RightRotation(Node *Rotation) {
		Node *Left	   = Rotation->Left;
		Rotation->Left = Left->Right;

		if (!Left->Right->IsNil()) {
			Left->Right->Parent = Rotation;
		}

		Left->Parent = Rotation->Parent;

		if (Rotation->Parent->IsNil()) {
			Root = Left;
		} else if (Rotation == Rotation->Parent->Left) {
			Rotation->Parent->Left = Left;
		} else {
			Rotation->Parent->Right = Left;
		}

		Left->Right		 = Rotation;
		Rotation->Parent = Left;
	}

private:
	VRBTNilNode<Key, Pair> *Nil;
	VRBTNode<Key, Pair>	   *Root;
	RBTAllocator		   &Allocator;
};

}; // namespace _VMem
