#include<iostream>

namespace _0951_Flip_Equivalent_Binary_Trees {
	using namespace std;

	struct TreeNode
	{
		int val;
		TreeNode* left;
		TreeNode* right;
		TreeNode()
			:val(0),
			left(nullptr),
			right(nullptr)
		{}

		TreeNode(int x)
			:val(x),
			left(nullptr),
			right(nullptr)
		{}

		TreeNode(int x, TreeNode* left, TreeNode* right)
			:val(x),
			left(left),
			right(right)
		{}
	};

	//accepted 0ms beats 100%
	class Solution
	{
	public:
		bool flipEquiv(TreeNode* root1, TreeNode* root2)
		{
			if (root1 == nullptr && root2 == nullptr)
				return true;

			if ((root1 == nullptr && root2 != nullptr) || (root1 != nullptr && root2 == nullptr)) // only the very 1st iteration needs this, subsequent calls won't need to because the way the two "if" conditions set up below
				return false;

			if (root1->val != root2->val) // Don't forget to check two roots' values
				return false;

			if 
			(
				(
					(root1->left != nullptr && root2->left != nullptr && root1->left->val == root2->left->val) 
					|| 
					(root1->left == nullptr && root2->left == nullptr)
				) 
				
				&&
				
				(
					(root1->right != nullptr && root2->right != nullptr && root1->right->val == root2->right->val) 
					||
					(root1->right == nullptr && root2->right==nullptr)
				)
			)
			{
				return flipEquiv(root1->left, root2->left) && flipEquiv(root1->right, root2->right);
			}

			else if
			(
				(
					(root1->left != nullptr && root2->right != nullptr && root1->left->val == root2->right->val)
					||
					(root1->left == nullptr && root2->right == nullptr)
				)

				&&

				(
					(root1->right != nullptr && root2->left != nullptr && root1->right->val == root2->left->val)
					||
					(root1->right == nullptr && root2->left == nullptr)
				)
			)
			{
				return flipEquiv(root1->left, root2->right) && flipEquiv(root1->right, root2->left);
			}
			return false;
		}
	};
}