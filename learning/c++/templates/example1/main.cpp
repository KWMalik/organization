#include <iostream>

using namespace std;


/// \function Maximum
/// \author   Toby Banks
///
///
/// \brief Returns the largest of x and y.
/// \tparam T The type of the values we are looking to compare. The type T \
///           have a overloaded < operator for this function to compile.
/// \param x A value to compare to y.
/// \param y A value to compare to x.
/// \return The larger of x and y.
template<typename T>
const T& Maximum(const T& x, const T& y)	
{
	if(y < x)
	{
		return x;
	}
	return y;
}


/// This example is completely trivial. We determine the 
/// greater of 5 and 10.
void Example1()
{
	int a = 5;
	int b = 10;

	int result = Maximum(a, b);
	printf("The result of Maximum(%d, %d) is %d\n", a, b, result);
}

/// This example shows that templates can be used to convert types.
/// The parameters are integers being passed into Maximum, but get 
/// explicitly casted to floats.
void Example2()
{
	int a = 5;
	int b = 10;

	//The below function call has two effects that might not be apparent. 
	//The first is that it casts a and b to be floats. It also causes Maximum
	//to be compiled into the executable twice, once as Maximum<int> the 
	//other is Maximum<float>.
	float floatresult = Maximum<float>(a, b);
	printf("The result of Maximum(%f, %f) is %f\n", (float)a, (float)b, floatresult);
}

int main(int argc, char **argv)
{
	Example1();
	Example2();

	return 0;
}
