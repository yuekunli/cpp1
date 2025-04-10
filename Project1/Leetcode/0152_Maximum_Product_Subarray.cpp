#include<vector>
#include<numeric>
#include<string>
#include<iostream>
#include<sstream>

namespace _0152_Maximum_Product_Subarray {

	using namespace std;

	/*
	* If a segment has odd numbers of negative number, I need to exlcude one.
	* Which one to exclude in order to produce the maximum product?
	* Do I need to consider excluding the ones in the middle?
	* No, I only need to consider the ones on both ends.
	* 
	* + + + - + + + - + + - + + - + + + - + + + +
	*       1       2     3     4       5
	* 
	* There are 5 negative numbers. The more number I multiple the bigger the absolute of the product.
	* If I consider excluding the 3rd negative number, neither half is longer than when the segment I get when I exclude 1st or 5th.
	*/

	class Solution
	{
	public:
		int maxProduct(vector<int>& n)
		{
			size_t i = 0;
			size_t len = n.size();
			bool hasZero = false;
			size_t segment_start, segment_end;
			bool newSegmentStarted = false;
			int maxProduct = INT_MIN;
			while (i < len)
			{
				if (n[i] == 0)
					hasZero = true;

				if (n[i] != 0 && !newSegmentStarted)
				{
					segment_start = i;
					newSegmentStarted = true;
				}
				if ((n[i] == 0 || i == len-1) && newSegmentStarted)
				{
					if (n[i] == 0)
					{
						segment_end = i - 1;
					}
					else // i is pointing at the last element
						segment_end = i;

					newSegmentStarted = false;

					//**** process a segment ****

					int negativeNumberCount = 0;
					size_t firstNegativeIndex = -1, lastNegativeIndex;
					for (size_t j = segment_start; j <= segment_end; j++)
					{
						if (n[j] < 0)
						{
							negativeNumberCount++;
							if (firstNegativeIndex == -1)
								firstNegativeIndex = j;
							lastNegativeIndex = j;
						}
					}
					if (negativeNumberCount % 2 == 0)
					{
						int product = accumulate(n.begin() + segment_start, n.begin() + segment_end + 1, 1, multiplies<int>());
						maxProduct = max(maxProduct, product);
					}
					else
					{
						/*
						* This segment is divided into 3 parts, the left and right part has all positive numbers,
						* the middle part has mixed positive and negative numbers.
						* Note that any of the 3 parts can be missing, for example, the first number in the segment is negative, there won't be a left part.
						* In extreme case, this segment only has 1 negative number, so all 3 parts are missing.
						*
						*      + + + + + + + + + +  -  . . . . . . .  . . . . . . .  - + + + + + + + + + + + +
						*      |<-- left part -->|  |  |<----   middle    -------->  | |<----   right     -->|
						*                           |                                |
						*                       first negative                      last negative
						*
						* 
						* basically there are 4 points to compare, namely seg_start, first_neg, last_neg, seg_end
						* 
						*/
						int leftProduct = 1, midProduct, rightProduct = 1;
						int maxProductOfThisSegment;

						if (segment_start<firstNegativeIndex)
							leftProduct = accumulate(n.begin() + segment_start, n.begin() + firstNegativeIndex, 1, multiplies<int>());
						if (firstNegativeIndex < lastNegativeIndex)
							midProduct = accumulate(n.begin() + firstNegativeIndex + 1, n.begin() + lastNegativeIndex, 1, multiplies<int>());
						if (lastNegativeIndex < segment_end)
							rightProduct = accumulate(n.begin() + lastNegativeIndex + 1, n.begin() + segment_end + 1, 1, multiplies<int>());


						if (firstNegativeIndex != lastNegativeIndex) // as long as there is middle part, I don't care left or right part, if they are missing, I can pretend their products are 1 respectively
							maxProductOfThisSegment = max(leftProduct * n[firstNegativeIndex] * midProduct, midProduct * n[lastNegativeIndex] * rightProduct);
						else if (segment_start < firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex < segment_end)  // no middle, but both left and right exist
							maxProductOfThisSegment = max(leftProduct, rightProduct);
						else if (segment_start == firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex < segment_end) // no left or middle, but right exists
							maxProductOfThisSegment = rightProduct;
						else if (segment_start < firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex == segment_end) // no middle or right, but left exists
							maxProductOfThisSegment = leftProduct;
						else if (segment_start == firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex == segment_end) // none of left, middle or right, just 1 negative number
							maxProductOfThisSegment = n[firstNegativeIndex];


						maxProduct = max(maxProduct, maxProductOfThisSegment);
					}
				}
				i++;
			}
			if (maxProduct < 0 && hasZero)  // example: [-2, 0, -1], the best answer that every segment can produce is negative, but there are zeros in the array.
											// or the array has all zeros, the majority of the code is always skipped, maxProduct is never changed, it remains INT_MIN, but there are zeros in the array
				return 0;
			else
				return maxProduct;
		}
	};

	/**
	* This solution only deals with arrays without zero.
	* The idea is the same with the first solution.
	* When there are odd number of negative numbers, the max product is either the subarray from left to the element before the last negative
	* or the subarray from the right to the element after the first negative element.
	* X X X X X X X - X X X X X X X X X X X X X X X X X X X - X X X X X X X X
	*               |                                       |
	*             first negative                           last negative
	* |---------------------------------------------------|
	*                 |-----------------------------------------------------|
	* Max product comes from either one of these two segments.
	*/
	class SolutionArrayNoZero
	{
	public:
		int maxProduct(vector<int>& n)
		{
			if (n.size() == 1)
			{
				return n[0];
			}

			int productLeftToRight = 0;
			int productRightToLeft = 0;
			int lastPositiveProduct = 0;
			int currentProduct = 1;
			size_t l = n.size();

			for (size_t i = 0; i < l; ++i)
			{
				currentProduct *= n[i];
				if (currentProduct > lastPositiveProduct)
				{
					lastPositiveProduct = currentProduct;
				}
			}

			productLeftToRight = lastPositiveProduct;

			// 1 <= n.length <= 2 * 10^4
			// because of this constraint, I can use "int" to index the vector

			lastPositiveProduct = 0;
			currentProduct = 1;

			for (int i = (int)(n.size()-1); i >= 0; --i)
			{
				currentProduct *= n[i];
				if (currentProduct > lastPositiveProduct)
				{
					lastPositiveProduct = currentProduct;
				}
			}
			productRightToLeft = lastPositiveProduct;

			return max(productLeftToRight, productRightToLeft);
		}
	};


	/**
	* Build on top of SolutionArrayNoZero.
	* Further simplify that solution and deal with zeros
	* verified 0ms beats 100%
	*/
	class Solution3
	{
	public:
		int maxProduct(vector<int> n)
		{
			size_t l = n.size();
			if (l == 1)
			{
				return n[0];
			}

			size_t firstNonZero = 0;
			while (firstNonZero < l && n[firstNonZero] == 0)
			{
				++firstNonZero;
			}

			if (firstNonZero == l)
			{
				return 0;
			}

			int currentProduct = 1;
			int maxLeftToRight = 0, maxRightToLeft = 0;
			int finalAnswer = 0;
			size_t segmentStart = firstNonZero + 1;
			for (size_t i = firstNonZero; i < l; ++i)
			{
				if (n[i] != 0)
				{
					currentProduct *= n[i];
					maxLeftToRight = max(maxLeftToRight, currentProduct);
				}
				
				if (n[i] == 0 || i == l-1)
				{
					currentProduct = 1;
					// where to start calculation from right to left?
					// it depends on whether 'i' is pointing at a zero.
					// If it is pointing at a zero, then start from the one to its left
					// If it is pointing at the last element and that element is not zero,
					// start from that element because that element is the real segment end.
					int j = 0;
					if (n[i] == 0)
					{
						j = i - 1;
					}
					if (n[i] != 0 && i == l - 1)
					{
						j = i;
					}
					while( j >= segmentStart)
					{
						currentProduct *= n[j];
						maxRightToLeft = max(maxRightToLeft, currentProduct);
						--j;
					}

					finalAnswer = max(finalAnswer, max(maxRightToLeft, maxLeftToRight));
					currentProduct = 1;
					maxLeftToRight = maxRightToLeft = 0;
					segmentStart = i + 1;
					// more logical action is to find the next non-zero element, in case multiple zeros in a row
					// but setting segmentStart to i + 1 (albeit it can still be a zero) also works
				}
			}
			return finalAnswer;
		}
	};


	void Test_0152_Maximum_Product_Subarray()
	{
		Solution solu1;
		//SolutionArrayNoZero solu2;
		Solution3 solu3;

		string s;
		vector<int>nums;

		while (true)
		{
			s.clear();
			cout << "array:  ";
			getline(cin, s);
			if (s.size() == 0)
				return;
			stringstream ss(s);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << "Solution1 answer: " << solu1.maxProduct(nums) << "\n";
			//cout << "SolutionArrayNoZero answer: " << solu2.maxProduct(nums) << "\n\n";
			cout << "Solution3 answer: " << solu3.maxProduct(nums) << "\n\n";
		}
	}
}