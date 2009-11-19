#include <iostream>

using namespace std;

//Creating typedef's for common types. You'll find this throughout many
//C++ software projects. The reason for this is to provide a level of
//abstraction above a given platform. For instance if you were working on 
//a 64-bit system with a version of C++ that treated a int as being 64-bit 
//instead of 32-bit, using these typedefs would allow your code to run
//as intended.
typedef unsigned char Char;
typedef unsigned char Byte;

typedef signed char Int8;
typedef unsigned char UInt8;

typedef signed short Int16;
typedef unsigned short UInt16;

typedef signed int Int32;
typedef unsigned int UInt32;

#if defined(GCC)
typedef signed long long Int64;
typedef unsigned long long UInt64;
#endif // defined(GCC)

//In cases where the size of the type is not important (i.e. most local 
//variables) 
#if defined(64BIT_PROCESSOR)
typedef signed long long Int;
typedef unsigned long long UInt;
#elif defined(32BIT_PROCESSOR)
typedef signed int Int;
typedef unsigned int UInt;
#endif


int main(int argc, char **argv)
{
	//TODO: Convert this to Unit Tests
	cout << "The sizes of our types: " << endl;
	cout << "%t sizeof(Char) == " << Char << endl;
	cout << "%t sizeof(Byte) == " << Byte << endl;
	cout << "%t sizeof(Int8) == " << Int8 << endl;
	cout << "%t sizeof(UInt8) == " << UInt8 << endl;
	cout << "%t sizeof(Int16) == " << Int16 << endl;
	cout << "%t sizeof(UInt16) == " << UInt16 << endl;
	cout << "%t sizeof(Int32) == " << Int32 << endl;
	cout << "%t sizeof(UInt32) == " << UInt32 << endl;
	cout << "%t sizeof(Int64) == " << Int64 << endl;
	cout << "%t sizeof(UInt64) == " << UInt64 << endl;
	cout << "%t sizeof(Int) == " << Int << endl;
	cout << "%t sizeof(UInt) == " << UInt << endl;


	return 0;
}
