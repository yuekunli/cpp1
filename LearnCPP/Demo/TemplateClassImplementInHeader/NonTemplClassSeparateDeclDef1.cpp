#pragma once

#include "NonTemplClassSeparateDeclDef1.h"

namespace NonTemplClassSeparateDeclDef1 {


A::A(int value) :
	_val(value)
{

}

int A::size()
{
	return _val;
}

}