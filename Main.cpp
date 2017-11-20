#include <stdio.h>
#include "Equation.hpp"

void main()
{
	Equation e("y = tan(x + 3) + (((2 * x) + 8))");
	if (!e.getErrorLog().empty())
		printf("%s\n", e.getErrorLog().c_str());
	getchar();
}