
#include <iostream>
#include <typeinfo>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;


class Vector
{
public:
	Vector() : x(0x0001), y(0x0010), z(0x0100), w(0x1000) {}

private:
	int x, y, z, w;

};

template<class T>
struct IsRecordable { enum { Recordable = 0 }; };

template<> 
struct IsRecordable<Vector> { enum { Recordable = 1 }; };

template<> 
struct IsRecordable<int> { enum { Recordable = 1 }; };

template<> 
struct IsRecordable<float> { enum { Recordable = 1 }; };



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	if(IsRecordable<char>::Recordable)
	{
		cout << "A char is recordable" << endl;
	}
	else
	{
		cout << "A char is NOT recordable" << endl;
	}
	
	if(IsRecordable<Vector>::Recordable)
	{
		cout << "A Vector is recordable" << endl;
	}
	else
	{
		cout << "A Vector is NOT recordable" << endl;
	}


	if(IsRecordable<int>::Recordable)
	{
		cout << "A int is recordable" << endl;
	}
	else
	{
		cout << "A int is NOT recordable" << endl;
	}


	if(IsRecordable<float>::Recordable)
	{
		cout << "A float is recordable" << endl;
	}
	else
	{
		cout << "A float is NOT recordable" << endl;
	}

	return 0;
}





