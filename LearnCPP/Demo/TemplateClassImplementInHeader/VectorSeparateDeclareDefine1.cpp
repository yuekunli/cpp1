//#include "LibHeaders.h"
#include<vector>
#include "VectorSeparateDeclareDefine1.h"

namespace VectorSeparateDeclareDefine1 {



template <typename Object>
Vector<Object>::Vector(int initSize) :
	theSize(initSize),
	theCapacity(initSize + SPARE_CAPACITY)
{
	objects = new Object[theCapacity];
}


template <typename Object>
Vector<Object>::Vector(const Vector& rhs) :
	theSize(rhs.theSize),
	theCapacity(rhs.theCapacity),
	objects(nullptr)
{
	objects = new Object[theCapacity];
	for (int k = 0; k < theSize; ++k)
	{
		objects[k] = rhs.objects[k];
	}
}


template <typename Object>
Vector<Object>& Vector<Object>::operator=(const Vector& rhs)
{
	Vector copy = rhs;
	std::swap(*this, copy);
	return (*this);
}


template <typename Object>
Vector<Object>::~Vector()
{
	delete[] objects;
}


template <typename Object>
Vector<Object>::Vector(Vector&& rhs) :
	theSize(rhs.theSize),
	theCapacity(rhs.theCapacity),
	objects(rhs.objects)
{
	rhs.objects = nullptr;
	rhs.theSize = 0;
	rhs.theCapacity = 0;
}


template <typename Object>
Vector<Object>& Vector<Object>::operator=(Vector&& rhs)
{
	std::swap(theSize, rhs.theSize);
	std::swap(theCapacity, rhs.theCapacity);
	std::swap(objects, rhs.objects);   // why not delete old objects?

	return *this;
}


template <typename Object>
void Vector<Object>::resize(int newSize)
{
	if (newSize > theCapacity)
	{
		reserve(newSize * 2);
	}
	theSize = newSize;
}


template <typename Object>
void Vector<Object>::reserve(int newCapacity)
{
	if (newCapacity < theSize)
	{
		return;
	}
	Object* newArray = new Object[newCapacity];
	for (int k = 0; k < theSize; ++k)
	{
		newArray[k] = std::move(objects[k]);
	}
	theCapacity = newCapacity;
	std::swap(objects, newArray);
	delete[] newArray;
}


template <typename Object>
Object& Vector<Object>::operator[](int index)
{
	return objects[index];
}


template <typename Object>
const Object& Vector<Object>::operator[] (int index) const
{
	return objects[index];
}


template <typename Object>
bool Vector<Object>::empty() const
{
	return size() == 0;
}


template <typename Object>
int Vector<Object>::size() const
{
	return theSize;
}


template <typename Object>
int Vector<Object>::capcity() const
{
	return theCapacity;
}


template <typename Object>
void Vector<Object>::push_back(const Object& x)
{
	if (theSize == theCapacity)
	{
		reserve(2 * theCapacity + 1);
	}
	objects[theSize++] = x;
}


template <typename Object>
void Vector<Object>::push_back(Object&& x)
{
	if (theSize == theCapacity)
	{
		reserve(2 * theCapacity + 1);
	}
	objects[theSize++] = std::move(x);
}


template <typename Object>
void Vector<Object>::pop_back()
{
	theSize--;
}


template <typename Object>
const Object& Vector<Object>::back() const
{
	return objects[theSize - 1];
}


template <typename Object>
typename Vector<Object>::iterator Vector<Object>::begin()
{
	return &objects[0];
}


template <typename Object>
typename Vector<Object>::const_iterator Vector<Object>::begin() const
{
	return &objects[0];
}


template <typename Object>
typename Vector<Object>::iterator Vector<Object>::end()
{
	return &objects[size()];
}


template <typename Object>
typename Vector<Object>::const_iterator Vector<Object>::end() const
{
	return &objects[size()];
}



}