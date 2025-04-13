#include<iostream>
#include<stack>
#include<forward_list>

namespace _0155_Min_Stack {

	using namespace std;


	/*
	* for example this is the stack (grows from left to right):
	* 
	*     a  b  c  d  e  f  g  h  i  j  k  l  m  n  p  q
	*                 |
	*                'e' is the minimum among 'a' to 'e'.
	* 
	* when 'e' is pushed onto the stack, I note down that 'e' is currently
	* the minimum. 'f', 'g', and 'h' are all greater than 'e'. I don't need
	* to remember or find out where 'f', 'g', and 'h' belong to in the overall
	* order, because as long as 'e' is on the stack, 'f', 'g' and 'h' don't matter.
	* And the only way to get 'e' off the stack is to get 'f', 'g' and 'h' off
	* the stack first. Assume that 'i' is less than 'e'. Now 'i' matters.
	* Just put 'i' into another linked-list (assuming 'e' was already put into there).
	* Whenever I need to pop an element off the stack, I check if that element is currently
	* the minimum, (i.e. if it is the head of the linked-list), if so, I need to remove the
	* head of the linked-list. Now the linked-list automatically gives me the minimum of 
	* the remaining elements because the ones that don't matter are never put into the linked-list.
	* The said linked-list actually can be another stack.
	* 
	* In the above example, if 'i' is equal to 'e', I also need to put 'i' into the linked-list.
	* Or I can make every element in the linked-list a pair, the first member of the pair is the value,
	* the second member of the pair is its occurrence count. There is pros and cons in both approaches.
	* If there are a lot of duplicated minimums, the pair approach is better. If there are few duplicates,
	* Many pairs just have their second member being 1, which is also a waste.
	*/

	//verified 0ms beats 100%
	class MinStack
	{
		stack<int> st;
		forward_list<int> minList;
	public:
		MinStack() : st{}, minList{}
		{

		}

		void push(int val)
		{
			st.push(val);
			if (minList.empty())
			{
				minList.emplace_front(val);
			}
			else
			{
				if (minList.front() >= val) // the equal here is important
				{
					minList.emplace_front(val);
				}
			}
		}

		void pop()
		{
			int val = st.top();
			if (val == minList.front())
			{
				minList.pop_front();
			}
			st.pop();
		}

		int top()
		{
			return st.top();
		}

		int getMin()
		{
			return minList.front();
		}

	};

}