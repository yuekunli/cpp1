#include<vector>
#include<iostream>

namespace _0153_Find_Minimum_in_Rotated_Sorted_Array {

	using namespace std;

	/*
	* 5, 1, 2, 3, 4
	* 
	* Because (hi - lo) / 2 + lo gives me lower half, it's possible that mid points to index#0 and lo and hi don't overlap.
	* It's impossible mid points at the last element, but lo and hi don't overlap.
	* In the above example:
	* 1st round: lo: 5, hi: 4, mid: 2
	* 2nd round: lo: 5, hi: 1, mid: 5
	* When mid points at the last element, the minimum must be either the first or the last element.
	* When mid points at the first element, the minimum may be neither the first nor the last.
	* This is why condition (2) must have "&& (lo == hi)" part.
	* Also because of this, i.e. mid can point at the first and minimum is to its right, condition (3) uses <= instead of <
	*
	* Note that if there is no rotation, for example: 2 5 8 9 11 15
	* first round: mid points at the middle, by condition (3), "lo" moves up, next round "mid" moves to the right
	* This actually moves mid farther away from the minimum, but eventually "mid" is going to point at the last element
	* and "lo" and "hi" are going to overlap, execution will fall in condition (2).
	* 
	* A fast corner case check is this:
	*  at the beginning of the function:
	*	if (n[0] <= *n.rbegin())
	*   {
	*     return n[0];
	*   }
	* this takes care of "no rotation" and "single element array" cases.
	* 
	*/

	class Solution
	{
	public:
		int findMin(vector<int>& n)
		{
			size_t lo = 0;
			size_t hi = n.size() - 1;
			size_t mid;

			while (true)
			{
				mid = (hi - lo) / 2 + lo;

				if (mid != n.size() - 1 && mid != 0 && n[mid - 1] > n[mid] && n[mid] < n[mid + 1])   // ---- (1)
					return n[mid];

				else if ((mid == 0 || mid == n.size() - 1) && (lo == hi))   // ---- (2)
					return min(n[0], *n.rbegin());

				else if (n[0] <= n[mid])  // ---- (3)
					lo = mid + 1;
				else if (n[0] > n[mid])  // ---- (4)
					hi = mid - 1;
			}
		}
	};

	void Test_0153_Find_Minimum_in_Rotated_Sorted_Array()
	{
		Solution s;
		vector<int> nums{ 11, 13};
		cout << s.findMin(nums) << endl;
	}
}