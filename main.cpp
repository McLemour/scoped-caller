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

void test()
{
	ScopedCaller sc1;

	auto f = sc1.Do(hello, 1);

	std::thread t([&f]() {
		f.get();
	});

	t.detach();
}

int main()
{
	test();

	std::cin.get();
}