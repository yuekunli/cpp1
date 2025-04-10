#include<iostream>

namespace Rotate_In_Place {

	template<typename T, typename U=size_t>
	void swap(T(&arr)[], U start, U end)
	{
		U mid = start + (end - start) / 2;
		if (mid == start)
		{
			std::swap(arr[start], arr[end]);
		}
		else
		{
			for (size_t i = start, j = 1; i <= mid; i++, j++)
			{
				std::swap(arr[i], arr[mid + j]);
			}
		}
	}

	/*
	* direction: true:  left
	*            false: right
	* 
	* 
	* It doesn't matter too much which way I'm trying to rotate.
	* Initially use the direction to decide the left part length and right part length.
	* For example, steps is 7, if I left rotate, count 7 from left, these 7 are the left part, the rest is the right part.
	* If I right rotate, count 7 from right side, these 7 are the right part, the rest is the left part.
	* Once the left part and right part is decided, the direction doesn't matter much.
	* I always try to move the shorter part toward the longer part.
	* 
	* a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q
	* |-----left----|---------------right-------------|
	* 
	* If I'm left rotating 5 steps, I move the left part toward right.
	* If I'm right rotating 12 steps, I move right part toward left, but because left is shorter, it's equivalent to moving left toward right.
	* 
	*/
	template<typename T>
	void rotate(T(&arr)[], size_t len, size_t steps, bool direction)
	{
		size_t left_length = direction ? steps : len - steps;
		size_t right_length = len - left_length;

		size_t focus_partition_start = 0;
		size_t focus_partition_end = len - 1;

		while (left_length > 0 && right_length > 0)
		{
			if (left_length <= right_length)
			{
				swap(arr, focus_partition_start, focus_partition_start + 2 * left_length - 1);
				right_length -= left_length;
				focus_partition_start += left_length;
			}
			else
			{
				swap(arr, focus_partition_end - 2 * right_length + 1, focus_partition_end);
				left_length -= right_length;
				focus_partition_end -= right_length;
			}
		}
	}


	template<typename T>
	void print_array(T(&arr)[], size_t len)
	{
		for (size_t i = 0; i < len; i++)
			std::cout << arr[i] << "  ";

		std::cout << '\n';
	}

	void Test_Rotate_In_Place()
	{
		int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		char b[10] = { 'a', 'b', 'c', 'd', 'e', 'f' , 'g', 'h', 'k', 'm'};
		float c[10] = { 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.1f };

		print_array<int>(a, 10);
		rotate<int>(a, 10, 4, true);
		print_array<int>(a, 10);

		print_array<char>(b, 10);
		rotate<char>(b, 10, 2, false);
		print_array<char>(b, 10);

		print_array<float>(c, 10);
		rotate<float>(c, 10, 7, true);
		print_array<float>(c, 10);

	}
}