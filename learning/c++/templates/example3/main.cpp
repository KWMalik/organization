
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

#if 0
struct MyStruct
{
	MyStruct(int _a, int _b) : a(_a), b(_b) {}
	int a;
	int b;
};
#else
struct MyStruct
{
	MyStruct(int _a, int _b) : a(_a), b(_b) {}

	//Necessary operator to compile this class
	bool operator<(const MyStruct &that) const
	{
		return a < that.a;
	}

	int a;
	int b;
};
#endif 

ostream& operator<<(ostream & stm, const MyStruct &my)
{
	stm << "MyStruct(a = " << my.a << ", " << my.b << ")";
}



int main(int argc, char **argv)
{
	int a = 5;
	int b = 10;
	char c = 5;
	char d = 5;

	//Note that this compiles to a seperate function in the exe.
	int result1 = Maximum(a, b);
	cout << "The result of Maximum(" << a << ", " << b << ") is " << result1 << endl;

	//Note that this compiles to a seperate function in the exe.
	int result2 = Maximum(c, d);
	cout << "The result of Maximum(" << c << ", " << d << ") is " << result2 << endl;

	//Note that this compiles to a seperate function in the exe.
	float floatresult = Maximum<float>(a, b);
	cout << "The result of Maximum(" << a << ", " << b << ") is " << floatresult << endl;

	//The following will only work with MyStruct::operator< being defined.
	MyStruct str_a(5, 10);
	MyStruct str_b(8, 16);

	MyStruct str_result = Maximum(str_a, str_b);
	cout << "The result of Maximum(" << str_a << ", " << str_b << ") is " << str_result << endl;

	return 0;
}

