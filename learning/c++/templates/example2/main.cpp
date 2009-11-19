
#include <iostream>

using namespace std;

template<typename T>
const T& Maximum(const T& x, const T& y)	
{
	if(y < x)
	{
		return x;
	}
	return y;
}

int main(int argc, char **argv)
{
	int a = 5;
	int b = 10;

	int result = Maximum(a, b);
	printf("The result of Maximum(%d, %d) is %d\n", a, b, result);

	float floatresult = Maximum<float>(a, b);
	printf("The result of Maximum(%f, %f) is %f\n", (float)a, (float)b, floatresult);

	return 0;
}
