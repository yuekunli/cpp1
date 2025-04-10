#include<utility>
#include<iterator>
#include<vector>

namespace TrailingReturnType1 {

template <typename It>
auto slide(It first, It last, It pos) -> std::pair<It, It>
{
	if (pos < first)
	{
		return { pos, std::rotate(pos, first, last) };
	}
	if (last < pos)
	{
		return { std::rotate(first, last, pos) , pos };
	}
	return { first, last };
}

// Convert the above function to one that doesn't use auto and trailing return type

template <typename It>
std::pair<typename std::iterator_traits<It>::pointer, typename std::iterator_traits<It>::pointer> slide2(It first, It last, It pos) // I'm returning the iterator, is it the same as the "pointer" type in iterator_traits? --It's not, but compile may succeed if this function is never called.
{
	if (pos < first)
	{
		return { pos, std::rotate(pos, first, last) };
	}
	if (last < pos)
	{
		return { std::rotate(first, last, pos) , pos };
	}
	return { first, last };
}


template <typename It>
std::pair<It, It> slide3(It first, It last, It pos)  // it turns out the simplest way is good enough
{
	if (pos < first)
	{
		return { pos, std::rotate(pos, first, last) };
	}
	if (last < pos)
	{
		return { std::rotate(first, last, pos) , pos };
	}
	return { first, last };
}



template <typename It>
std::pair<typename std::iterator_traits<It>::value_type, typename std::iterator_traits<It>::value_type> slide4(It first, It last, It pos)   // this is definitely ill-formed, compile may succeed if this function is never called.
{
	if (pos < first)
	{
		return { pos, std::rotate(pos, first, last) };
	}
	if (last < pos)
	{
		return { std::rotate(first, last, pos) , pos };
	}
	return { first, last };
}




void Test_TrailingReturnType1()
{
	std::vector<char> v = {'a', 'b', 'c', 'd', 'e', 'f', 'g' , 'h', 'i'};

	//slide(&(v[1]), &(v[4]), &(v[7]));
	slide3(v.begin() + 1, v.begin() + 4, v.begin() + 7);
}


}