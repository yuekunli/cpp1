#include<iostream>
#include<functional>
#include<stack>

using namespace std;

namespace Post_Order_Traverse_Iterative {

	class TreeNode
	{
	public:
		int value;
		class TreeNode* left;
		class TreeNode* right;

		TreeNode(int _value, class TreeNode* _left, class TreeNode* _right) :value(_value), left(_left), right(_right)
		{

		}
		TreeNode() :value(0), left(nullptr), right(nullptr)
		{

		}
	};

	void print_node_value(TreeNode* n)
	{
		cout << n->value << "  ";
	}

	void post_order_traverse(TreeNode* root, function<void(TreeNode*)> f)
	{
		stack<TreeNode*> s;

		bool firstSeen = true;

		TreeNode* n = root;

		while (true)
		{
			if (firstSeen)
			{
				if (n->left != nullptr)
				{
					s.push(n);
					n = n->left;
					continue;
				}
				else if (n->right != nullptr)
				{
					s.push(n);
					n = n->right;
					continue;
				}
				else
				{
					f(n);
					if (s.size() == 0)
					{
						break;
					}
					else
					{
						TreeNode* p = s.top();
						if (n == p->left)
						{
							n = p->right;
							continue;
						}
						else
						{
							n = s.top();
							s.pop();
							firstSeen = false;
							continue;
						}
					}
				}
			}
			else
			{
				f(n);
				if (s.size() == 0)
				{
					break;
				}
				else
				{
					TreeNode* p = s.top();
					if (n == p->left)
					{
						n = p->right;
						firstSeen = true;
						continue;
					}
					else
					{
						n = s.top();
						s.pop();
						firstSeen = false;
						continue;
					}
				}
			}
		}
	}

	void Test_Post_Order_Traverse_Iterative()
	{
		TreeNode tree[7];
		for (size_t i = 0; i < 7; i++)
			tree[i].value = i;

		tree[0].left = &tree[1];
		tree[0].right = &tree[2];
		tree[1].left = &tree[3];
		tree[1].right = &tree[4];
		tree[2].left = &tree[5];
		tree[2].right = &tree[6];

		post_order_traverse(&tree[0], print_node_value);
		cout << "\n\n";
	}
}