#include <iostream>
#include "scoped_caller.h"

void hello(int a)
{
	std::cout << "hello " << a << std::endl;
}

double computation(double b)
{
	double ret = sqrt(sqrt(sqrt(b)));
	std::cout << ret << std::endl;
	return ret;
}

int main()
{
	ScopedCaller sc1(hello, 1);
	ScopedCaller sc2(computation, 48);
}