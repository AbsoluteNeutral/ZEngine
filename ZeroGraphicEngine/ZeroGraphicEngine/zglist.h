#ifndef _ZG_LIST_H
#define _ZG_LIST_H

#include <iostream>
#include <iterator>
#include "zgAllocator.h"

#define _USEALLOCATOR_ 1

namespace zg 
{
	//__________________________________________________ Class
	template<typename T>
	class ZG_API list
	{
		struct Node 
		{
			T value;
			Node* next;
			Node* prev;
			Node()
				:value{}
				, next(nullptr)
				, prev(nullptr)
			{}

			Node(const T& value_)
				:value{ value_ }
				, next(nullptr)
				, prev(nullptr)
			{}
		};

		Node* newNode(const T&  value_)
		{
#if _USEALLOCATOR_
			Node* pNode = AllocateMalloc<Node>(1);
			new (pNode) Node { value_ };
			++sz;            //count the size of list
			return pNode;
#else
			Node *pNode = new Node{ value_ };
			++sz;            //count the size of list
			return pNode;
#endif
			
		}

	public:
		list () noexcept
			:root(nullptr), sz(0)
		{}
		
		// ______________________________________________________________________ Copy Constructor
		list(const list& rhs_) noexcept
			:root(nullptr), sz(0)
		{
			Node* tmp = rhs_.root;
			while (tmp)
			{
				push_back(tmp->value);
				tmp = tmp->next;
			}
		}

		// ______________________________________________________________________ Move Constructor
		list(list&& rhs_) noexcept
			:root(rhs_.root), sz(0)
		{
			rhs_.root = nullptr;
			rhs_.sz = 0;
		}

		// ______________________________________________________________________ Destructor
		~list()
		{
			clear();
		}

		// ______________________________________________________________________ Copy operator
		list& operator=	(const list& rhs_) 
		{
			list tmp{ rhs_ };
			root		= tmp.root;
			sz			= tmp.root;
			tmp.root	= nullptr;
			tmp.sz		= 0;
			return *this;
		}

		// ______________________________________________________________________ Move operator
		list& operator=	(list&& rhs_)
		{
			root		= rhs_.root;
			sz			= rhs_.sz;
			rhs_.root	= nullptr;
			rhs_.sz		= 0;
			return *this;
		}

		// ______________________________________________________________________ print list
		void print() const
		{
			Node* tmp = root;

			while (tmp)
			{
				std::cout << tmp->value << " ";
				tmp = tmp->next;
			}
			std::cout << std::endl;
		}
		bool empty() const;
		unsigned size() const;

		//__________________________________________________ clear
		void clear()
		{
			while (root) 
			{
				Node* toRemove = root;
				root = root->next;
#if _USEALLOCATOR_
				DeallocateMalloc(toRemove);
#else
				delete toRemove;            //free current Node
#endif
			}
			sz = 0;
			root = nullptr;
		}

		//__________________________________________________ push_front
		void push_front(const T&  value_)
		{
			Node* pNode = newNode(value_);	//allocate space for a Node
			pNode->next = root;				//point to the front of the list
			if (root)						//if a Node exist infront
				root->prev = pNode;			//point the previous list to the new list
			root = pNode;					//set the default to the front of the list/new list
		}

		//__________________________________________________ pop_front
		void pop_front()
		{
			Node* toRemove = root;
			root = root ? root->next : nullptr;
#if _USEALLOCATOR_
			DeallocateMalloc(toRemove);
#else
			delete toRemove;            //free current Node
#endif
			--sz;						//minus size
		}

		//__________________________________________________ push_back
		void push_back(const T&  value_)
		{
			Node* pNode = newNode(value_); //allocate space for a Node
			if (root)					    //if a Node exist
			{
				Node *tmp = root;			//a pointer to the front of the list
		
				while (tmp->next)           //while the tail is not NULL
					tmp = tmp->next;        //jump to the next list/last of the list
		
				tmp->next = pNode;            //anchor the last list to the new list
				pNode->prev = tmp;         //anchor the new list to the prev list
			}
			else
				root = pNode;           //reset the default to the front of the List
		}

		//__________________________________________________ pop_back
		void pop_back()
		{
			Node* toRemove = root;
			while (toRemove->next)				//travel to last Node  
				toRemove = toRemove->next;

			if (toRemove == root)
				root = nullptr;					//set to NULL
			else
				toRemove->prev->next = nullptr; //set to NULL

#if _USEALLOCATOR_
			DeallocateMalloc(toRemove);
#else
			delete toRemove;         //free current Node
#endif
			--sz;                    //decrease size
		}

		//__________________________________________________ insert
		void insert(unsigned pos, const T&  value_)
		{
			//if out of bound
			if (pos > sz) 
			{
				push_back(value_);
				return;
			}

			Node* pNewNode  = newNode(value_);		//new node to insert
			Node* pNextNode = root;					//a tmp Node to find the position

			while (pNextNode && pos--)
				pNextNode = pNextNode->next;		//point to next list

			if (pNextNode->next != nullptr)			//if the next pointer is not null
			{
				Node *pPrevNode = pNextNode->prev;	//Get previous Node

				if (pPrevNode != nullptr)
				{
					pNextNode->prev = pNewNode;       //anchor last list to new node
					pPrevNode->next = pNewNode;       //anchor prev list to new node
					pNewNode->next = pNextNode;       //anchor new list to last node
					pNewNode->prev = pNextNode->prev; //anchor new list to prev node
				}
				else if (pPrevNode == nullptr)
				{
					root = pNewNode;           //Set defalut to front of the list
					pNextNode->prev = pNewNode;   //anchor last list to new node
					pNewNode->next = pNextNode;   //anchor new list to last node
				}
			}
			else if (pNextNode->next == nullptr)
			{
				pNewNode->prev = pNextNode;     //anchor new list to prev node
				pNextNode->next = pNewNode;     //anchor prev list to new node
			}
		}

		//__________________________________________________ Reverse
		void reverse()
		{
			// _______ #version 2
			Node* move = root;
			Node* tmpR = root;
			while (move)
			{
				Node* tmp	= move->next;
				move->next	= move->prev;
				move->prev	= tmp;
				tmpR		= move;
				move		= tmp;
			} 
			root = tmpR;

			// _______ #version 1
			//Node *pNode = root;
			//if (pNode)								//if the list is not empty
			//{
			//	while (root->next)					//anchor the list to the back of the list
			//		root = root->next;
			//
			//	pNode = root;						//anchor the list to the back of the list
			//	while (pNode)                      //while not end of list
			//	{
			//		Node *pPrev = pNode->prev;		//get tmp Node
			//		pNode->prev = pNode->next;      //swap prev with next
			//		pNode->next = pPrev;            //swap next with prev
			//		pNode = pNode->next;             //check next node
			//	}
			//}
		}


		//__________________________________________________ Element access
		const T& front() const 
		{
			return root->value;
		}
		T& front() 
		{
			return root->value;
		}
		
	private:
		Node* root;
		unsigned sz;
	};

} // namespace zg



#endif //_UNITSTEST
