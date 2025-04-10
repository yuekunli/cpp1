#include<iostream>
#include<iomanip>
#include<random>
#include<vector>
#include<algorithm>
#include<functional>  // std::bind

namespace Merge_Two_Sorted_Arrays_With_Extra_Memory {

	using namespace std;

	int* merge(int a[], size_t len1, int b[], size_t len2)
	{

		int* c = nullptr;

		if (a == nullptr && b != nullptr)
		{
			c = new int[len2];
			std::memcpy(c, b, len2 * sizeof(b[0]));
			return c;

		}
		if (a != nullptr && b == nullptr)
		{
			c = new int[len1];
			std::memcpy(c, a, len1 * sizeof(a[0]));
			return c;
		}
		if (a == nullptr && b == nullptr)
			return nullptr;

		c = new int[len1+len2];

		size_t i = 0, j = 0, k = 0;
		while (i < len1 && j < len2)
		{
			if (a[i] <= b[j])
			{
				c[k] = a[i];
				i++;
			}
			else
			{
				c[k] = b[j];
				j++;
			}
			k++;
		}
		while (i < len1)
		{
			c[k] = a[i];
			i++;
			k++;
		}
		while (j < len2)
		{
			c[k] = b[j];
			j++;
			k++;
		}
		return c;
	}


	void Test_Merge_Two_Sorted_Arrays_With_Extra_Memory()
	{
		int a[] = { 1,4,5,8 };
		int b[] = { 3,7 };

		int* c = merge(a, sizeof(a)/sizeof(a[0]), b, sizeof(b)/sizeof(b[0]));

		size_t len = sizeof(c);
		size_t len1 = sizeof(a);
		size_t len2 = sizeof(b);

		cout << boolalpha << (len == len1 + len2) << '\n';

		for (size_t i = 0; i < 6; i++)
		{
			cout << c[i] << " ";
		}
		cout << "\n";
		delete[] c;
		int k;
		cin >> k;
	}
}

namespace Merge_Sort_Iterative {

	/*
	* array 'a': x  x  x  x  x  x  x  x  x  x  x  x  x  x
	* array 'b': x  x  x  x  x  x  x  x  x  x  x  x  x  x
	*                  |           |           |
	*                start        mid         end
	* 
	* a[start, mid] is sorted, a(mid, end] is sorted.
	* I want to merge these two portions of 'a'.
	* use b[start, end] as temporary space to do the merge.
	* copy the elements from 'b' to 'a', relinquish the space in 'b'.
	*/
	template<typename T>
	void merge(T* a, T* b, size_t start, size_t mid, size_t end)
	{
		size_t i = start, j = mid + 1, k = start;
		while (i <= mid && j <= end)
		{
			if (a[i] <= a[j])
			{
				b[k] = a[i];
				i++;
			}
			else
			{
				b[k] = a[j];
				j++;
			}
			k++;
		}
		while (i <= mid)
		{
			b[k] = a[i];
			i++; k++;
		}
		while (j <= end)
		{
			b[k] = a[j];
			j++;
			k++;
		}
		
		for (i = start; i <= end; i++)
			a[i] = b[i];
		
	}

	/*
	* if seg_length = x, left half (with x/2 elements) and right half (with the rest x/2 elements)
	* of a segment are already sorted.
	* I want to merge the two halves into one segment of length x.
	* 
	* seg_length starts at 2, which means the 1st pass only merges every two elements.
	* 
	* a b c d e f g h i g k m n p q r s t
	* 
	* 1st pass: segment length = 2, merge [a] and [b], [c] and [d], ... 
	* 2nd pass: segment length = 4, merge [ab] and [cd]
	* 
	* at any time, there could be a little tail
	* 
	* a couple different scenarios:
	* 
	* |______|______|  |______|______|  |______|______|  |___|
	*    seg_length      seg_length        seg_length    not enough
	* 
	* |______________|_______________|  |__________________.......|
	*         new seg_length             1 full segment of last iteration + a tail
	* 
	* in this case, just merge the 1 full segment of last iteration and the tail
	* 
	* 
	* 
	* |___________|  |___________|  |___________|
	* 
	* |__________________________|  |____________........|
	*   new seg_length               1 full segment of last iteration
	* 
	* in this case, don't need to deal with that segment of last iteration,
	* it's going to become a "tail" in the next iteration.
	* 
	* 
	* 
	* corner case is in the very 1st iteration, when there is one element left
	* 
	*      x  x     x  x     x  x      x
	*      |__|     |__|     |__|      |
	*                                start is here now.
	* 
	* hypothetically, the iteration *before* the first iteration is the iteration with a segment length of 1
	* which is trivial, so I start with segment length being 2. But the problem is that I can't tell whether
	* this last element is a "tail" or a full segment of last iteration.
	*/
	template<typename T>
	void merge_sort(T* a, size_t len)
	{
		if (len == 1)
			return;

		T* b = new T[len];

		size_t seg_len = 2; size_t tail_len = 0;
		while(len / seg_len >= 1)
		{
			// if there is at least 1 full segment at the beginning of this iteration
			// at least I can merge the left and right halves of it.
			size_t start = 0, end = start + seg_len - 1;
			while (end < len)
			{
				// I'm looking at a full segment
				merge<T>(a, b, start, start + seg_len/2 - 1, end);
				start += seg_len;
				end += seg_len;
			}

			if (seg_len == 2 && start == len - 1)
			{
				// the "merge" routine doesn't handle the corner case where start == mid == end
				// if it does handle it, I won't need this special handling right here.
				seg_len *= 2;
				tail_len = 1;
				continue;
			}

			/*
			*   0  1  2  3  4
			*            |
			*           start of tail = 5 -2 = len - tail_len
			*/
			if (start < len - tail_len)
			{
				// merge half of a segment and the tail
				merge<T>(a, b, start, start + seg_len/2 - 1, len - 1);
				tail_len = seg_len / 2 + tail_len;
			}

			seg_len *= 2;
		}

		if (tail_len != 0)
		{
			merge(a, b, 0, seg_len/2 - 1, len - 1);
		}

		delete[] b;
		
		return;
	}

	/*
	* The above implementation gets too entangled in the handling of the "tail".
	* This below implementation simplifies it.
	* Just keep counting sorted segments, if there are two of them, merge them.
	* if there is 1 segment and a tail shorter than a segment, merge them.
	* 
	* if at some point seg_length is 8, that means every 8 elements are sorted, I'm merging an 8-element segment with another 8-element segment now.
	* This is different than the seg_length definition in the previous implementation.
	* At any point, a segment means a segment that is already sorted. I'm using "sorted_seg_length" in the code to make it distinct.
	*/
	template<typename T>
	void merge_sort2(T* a, size_t len)
	{
		if (len == 1)
		{
			return;
		}

		T* b = new T[len];

		size_t sorted_seg_length = 1;
		size_t start, mid, end;

		while (true) // logarithm number of iterations
		{
			int number_of_merges = 0;
			start = 0;

			while (true) // linearly go over the array
			{
				mid = start + sorted_seg_length - 1;
				end = start + 2 * sorted_seg_length - 1;

				if (end < len)
				{
					merge<T>(a, b, start, mid, end);
					number_of_merges++;
					start = end + 1;
					continue;
				}

				if (mid >= len) // this check is necessary, see the case when there are 5 elements in total.
				{
					break;
				}

				if (end >= len)
				{
					merge<T>(a, b, start, mid, len - 1);
					number_of_merges++;
					break;
				}

			}

			if (number_of_merges == 1 && start == 0)
			{
				// example, array of 5 elements
				// after 1st iteration, I get: [ 2 elements ], [ 2 elements ], [x]
				// then sorted_seg_length increases to 2, I only do 1 merge, with 1 element left as a tail.
				// I get: [ 4 elements ] , [x]
				// obviously the job is not finished, but I only did 1 merge in the last iteration.
				// So I need another condition to determine if the sorting is done. Hence testing start == 0
				
				break;
			}

			sorted_seg_length *= 2;
		}

		delete[]b;
	}

	/*
	* simplify it even more. Simplify the termination condition.
	* if the very last merge is between a sorted segment (length of x) and a tail (shorter than x), then the next iteration is supposed to have a segment length 2x.
	* but the whole array must be shorter than 2x, so comparing segment length and array length can determine when to stop.
	* If it just so happens that the very last merge is between two full segments, then in the next iteration, segment length is 2x which is equal to array length.
	* Still, the segment length is not less than array length, that is the indicator to stop.
	*/
	template<typename T>
	void merge_sort3(T* a, size_t len)
	{
		T* b = new T[len];

		size_t sorted_seg_length = 1;
		size_t start, mid, end;
		start = 0;
		while (sorted_seg_length < len)
		{
			while (start < len)
			{
				mid = start + sorted_seg_length - 1;
				end = start + 2 * sorted_seg_length - 1;

				if (end < len) // as long as end < len, I have two sorted segments, I can merge them
				{
					merge(a, b, start, mid, end);
					start = end + 1;
					continue;
				}
				else if (mid < len-1) // else if mid < len-1, mid doesn't fall on the last element,
					                  // I have a sorted segment and a tail (which is also sorted), I can merge them
				{
					merge(a, b, start, mid, len - 1);
					start = len - 1 + 1; // this makes the inner loop to stop, as if moving "start" to the next element after this merge
					continue;
				}
				else
				{
					break;
				}
			}
			sorted_seg_length *= 2;
			start = 0;
		}
		
		delete[]b;
	}


	void Test_Merge_Sort_Iterative_auto_test()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> intDist(1, 200);

		for (size_t len = 1; len <= 1000; len++)
		{
			std::vector<int>arr(len);
			std::generate(arr.begin(), arr.end(), std::bind(intDist, std::ref(gen)));

			int* a = new int[len];
			int* original_copy = new int[len];
			for (size_t i = 0; i < len; i++)
			{
				a[i] = arr[i];
				original_copy[i] = arr[i];
			}
			std::sort(arr.begin(), arr.end());

			merge_sort3<int>(a, len);

			for (size_t i = 0; i < len; i++)
			{
				if (arr[i] != a[i])
				{
					for (size_t i = 0; i < len; i++)
						std::cout << original_copy[i] << "  ";
					std::cout << std::endl;
					throw std::runtime_error("failed");
				}
			}
			
			delete[] a;
			delete[] original_copy;
		}
		std::cout << "all pass" << std::endl;
	}

	
	template<size_t N>
	void Test_Merge_Sort_Iterative_specific_case()
	{
		int a[N] = { 137,  152,  130,  175,  5 };
		merge_sort2<int>(a, N);
		for (size_t i = 0; i < N; i++)
			std::cout << a[i] << "  ";

		std::cout << std::endl;
	}

	void Test_Merge_Sort_Iterative()
	{
		Test_Merge_Sort_Iterative_auto_test();
		//Test_Merge_Sort_Iterative_specific_case<5>();
	}
}


namespace Merge_In_Place {

	using namespace std;

	void test()
	{
		vector<int>a{ 1,2,3,4,5,6,7 };
		rotate(a.begin(), a.begin() + 2, a.end());
		for (size_t i = 0; i < 7; i++)
			cout << a[i] << "  ";
		cout << endl;
	}


	/*
	* a: x  x  x  x  x  x  x  x  x  x  x  x  x  x
	*    |        |        |     |              |
	*    start    |       mid    |              end
	*             |              |
	*             m1             m2
	* 
	* [start, mid] are sorted
	* (mid, end] are sorted
	* find the median between start and mid, call it m1.
	* find the insertion point of m1 between mid and end, call it m2.
	*/
	template<typename T>
	void merge_in_place(vector<T>&a, size_t start, size_t mid, size_t end)
	{
		if (end - start == 0)
			return;

		if (end - start == 1)
		{
			if (a[start] <= a[end])
				return;
			swap(a[start], a[end]);
			return;
		}

		size_t m1 = start + (mid - start) / 2;

		// binary search m1 in (mid, end]

		size_t lo = mid + 1, hi = end, m2 = lo + (hi - lo) / 2;
		
		if (a[m1] <= a[mid + 1])
		{
			/*
			*    1   2   3   9   10       5   6   7   8
			*    |       |       |                    |
			*   start   m1      mid                  end
			* 
			*                \_________/
			*                 suppose to rotate this part, but don't need to
			* 
			* mid is less than the entire second half, there is nothing in the 2nd half that needs to be moved to the left.
			* don't need to rotate, or pretend to do a rotate, then I get:
			*    1  2  3 _____     9  10  5  6  7  8
			*    \___________/     \_______________/
			*    
			* recursively merge these two parts, 1st part is already good, only need to work on the second part.
            *   
			*/
			if (m1 == mid)
			{
				/*
				* a b c d e
				* |
				* start
				* mid
				* m1
				*
				* start, mid, m1 all point to the same, i.e. the first part has only 1 element
				* I don't have anything in the first part that is after m1.
				* i.e. I don't have anyting in the first part which I need to merge with the 2nd part.
				*/
				return;
			}
			/*
			* "m1+1": must use +1, must not use "m1" instead.
			* 15,    37,    23,    28
			* |      |
			* start  mid
			* m1
			*
			* start, m1 both point to the same element.
			* If I use "m1" instead of "m1+1", recursion ends up in dead loop
			*/
			merge_in_place(a, m1+1, mid, end);
			return;
		}

		if (a[end] <= a[m1])
		{
			rotate(a.begin() + m1, a.begin() + mid + 1, a.begin() + end + 1);

			/*
			*   0 1 2 3 4 5 6 7 8 9 10 11
			*   |     |     |          |
			* start   m1   mid         end
			* 
			* 
			*   0 1 2 7 8 9 10 11 3 4 5 6
			*       |           |
			*      2 = 3-1      the index of this is 7 = start + m1 + (end - mid) - 1    
			* 
			*/

			/*
			* special case: start == mid == m1, the first part has only 1 element,
			* this element is rotated to the end, the job is done.
			*/
			if (m1 == start)
				return;

			merge_in_place(a, start, m1 - 1, m1-1 + (end - mid));
			return;
		}
		
		
		while (lo < hi)
		{
			if (a[m2] <= a[m1] && a[m1] <= a[m2 + 1])
				break;
			else if (a[m1] < a[m2])
			{
				hi = m2 - 1;
			}
			else // a[m2] < a[m1]
			{
				lo = m2 + 1;
			}
			m2 = lo + (hi - lo) / 2;
		}

		rotate(a.begin() + m1, a.begin() + mid + 1, a.begin() + m2 + 1);

		/*
		*  a b c d     e f g h i   j k l m n   p q r s t
		*  |           |       |           |           |
		*  start       m1     mid          m2          end
		*  0           4       8           13          18
		* 
		*  a b c d   j k l m n   e f  g  h  i   p  q  r  s  t
		*  0 1 2 3   4 5 6 7 8   9 10 11 12 13  14 15 16 17 18
		* 
		*/

		/*
		* handle special case where first part or second part has only 1 element.
		* 
		* if start, mid, m1 all point to the same element, that element will be placed in between 'n' and 'p'
		* after rotation. The first part, after rotation, only has [j,n], so I don't need to run another
		* merge_in_place for such first part.
		*/
		
		size_t first_part_end = (m1 - 1) + (m2 - mid);

		if (start < m1)
		{
			merge_in_place(a, start, m1 - 1, first_part_end);
		}

		/*
		* if mid+1 == m2 == end, this is actually the a[m1] greater than or less than the entire 2nd part case,
		* which should've been handled before.
		*/
		merge_in_place(a, first_part_end+1, first_part_end+(mid-m1)+1, end);
	}

	/*
	* The systematic way of considering corner cases:
	* 
	* |---------------------------|-----------------------------|
	* start       m1              mid          m2              end
	*             \______________/ \__________/
	*                segment 1        segment2
	* 
	* Eventually, I need to move segment1 to the right and move segment2 to the left.
	* consider corner cases based on weather there is segment1 or segment2, in another word, weather I need to move something to the right or left.
	*     |  segment 1    |   segment 2  |
	* ------------------------------------------------
	*         exist            exist             normal case
	*       not exist          exist             first half is really short, 'start', 'm1' and 'mid' all point to the same element
	*         exist           not exist          m1 is less than the entire second half
	*       not exist         not exist          first half is really short, and m1 is less than the entire second half
	* 
	* Note that "segment 1 not exist" means there is only 1 element in the first half, even if there are only 2 elements,
	* the pointers will be like this:
	*    X       X       X       X    .....
	*    |       |
	*   start   mid      |-> start 2nd half
	*    m1
	* 
	*/

	template<typename T>
	void merge_in_place2(vector<T>& a, size_t start, size_t mid, size_t end)
	{
		if (end - start == 0)
			return;

		if (end - start == 1)
		{
			if (a[start] <= a[end])
				return;
			swap(a[start], a[end]);
			return;
		}

		size_t m1 = start + (mid - start) / 2;

		if (m1 == mid && a[m1] <= a[mid + 1])  // segment1 not exist && segment2 not exist
		{
			return;
		}

		if (m1 < mid && a[m1] <= a[mid + 1])  // segment1 exists but segment2 not exist
		{
			/*
			*     start      m1           mid                   end
			*     |------------------------|--------------------|
			*                 \___________/
			*                  move this part to the right, nothing to move to the left
			*/
			merge_in_place2(a, m1 + 1, mid, end);
			return;
		}

		// need to find m2
		auto it = std::lower_bound(a.begin() + mid + 1, a.begin() + end + 1, a[m1]);
		size_t m2 = std::distance(a.begin() + mid + 1, it) + mid + 1;

		if (m1 == mid) // segment1 not exist, but segment2 exists
		{
			/*
			* The danger in this case:
			* segment 1 not exist means there is only 1 element in segment 1:
			*    x  (empty) | ............  |
			*    |                 |
			*  start              m2
			*    m1
			*   mid
			* 
			* after an imaginary rotation, i get:
			* 
			*   x  ....... (empty) .........
			*                      |
			*                     m2
			*  then I'm supposed to solve the segment before (empty),
			* but the next iteration will find the entire second half less than m1,
			* then I move the entire second half to the left, with nothing to move to the right
			*      x (empty) a b c d
			*  =>  x a b c d (empty)
			*  solve "x a b c d"
			* 
			* "x a b c d" is essentially equal to "x (empty) a b c d"
			* this means I'm going into dead loop.
			* 
			* therefore the real solution in this case is not recursively solving the subproblem,
			* instead, just find the position of the only element in the first half and rotate.
			*/
			
			//merge_in_place2(a, start, mid, m2 - 1);

			rotate(a.begin() + start, a.begin() + mid + 1, it);
			return;
		}

		rotate(a.begin() + m1 + 1, a.begin() + mid+1, a.begin() + m2);

		// normal case: need to solve two sub-problems

		size_t segment1_size = mid - m1;
		size_t segment2_size = m2 - mid - 1;
		merge_in_place2(a, start, m1, m1 + segment2_size);


		// one last little corner case:
		size_t original_second_half_size = end - mid;
		if (segment2_size == original_second_half_size)
		{
			/*
			* one last little corner case:
			* |------------------------|------------------| |
			* start      m1            mid              end m2
			*            \____________/\_________________/
			*             segment1       segment2
			* 
			* the entire 2nd half is being moved to the left
			* after rotation:
			* 
			* |--------------------------------|-------------|
			* start      m1\___________________/\____________/
			*                segment2             segment1
			* I don't need to solve the 2nd sub-problem, so just return.
			*/
			return;
		}

		merge_in_place2(a, m1 + segment2_size + 1, m2 - 1, end);
		return;
	}

	void auto_test()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> intDist(1, 200);

		for (size_t len = 1; len <= 100; len++)
		{
			std::vector<int>arr(len);
			std::generate(arr.begin(), arr.end(), std::bind(intDist, std::ref(gen)));
			
			std::vector<int>fully_sorted(arr);
			sort(fully_sorted.begin(), fully_sorted.end());

			size_t mid = (len-1) / 2;
			std::sort(arr.begin(), arr.begin()+mid+1);
			std::sort(arr.begin() + mid + 1, arr.end());
			
			std::vector<int>two_halves_sorted(arr);

			merge_in_place2<int>(arr, 0, mid, len - 1);
		
			for (size_t i = 0; i < len; i++)
			{
				if (arr[i] != fully_sorted[i])
				{
					for (size_t i = 0; i < len; i++)
						std::cout << two_halves_sorted[i] << ", ";
					std::cout << std::endl;

					throw std::runtime_error("error");
				}
			}
		}
		std::cout << "all pass" << std::endl;
	}

	void Test_Merge_In_Place()
	{
		//vector<int> a{ 10, 20, 30, 40, 50, 60, 13, 15, 22, 43, 47, 66 };
		//merge_in_place(a, 0, 5, 11);
		
		//vector<int> a{ 10, 20, 15, 17 };
		//merge_in_place(a, 0, 1, 3);

		//vector<int> a{ 30, 64, 99, 50, 69, 165 };
		//merge_in_place(a, 0, 2, 5);

		//vector<int> a{ 30, 44, 86, 170, 184, 27, 117, 150, 158, 164 };
		//merge_in_place(a, 0, 4, 9);

		//vector<int> a{ 1, 8, 9, 5, 6 };
		//merge_in_place2(a, 0, 2, 4);

		auto_test();
	}
}