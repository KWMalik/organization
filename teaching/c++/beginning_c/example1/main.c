
// The following line works like copy/paste does. It takes a header file 
// (a *.h file) and copy/pastes the contents into this file before compiling it.
#include <stdio.h>

// The following line tells the compiler to start a function. This function is 
// the main entry point into the program. This line is known as a function 
// definition. 
int main(int argv, char ** argc)
{
	// Below is the first line of code that will be executed by the program.
	// Well, actually in most cases it's not going to be the first line of code
	// but those are exceptions and we won't get into it now.
	// 
	// This line writes the string "Hello World!!!" out to stdio and then 
	// writes "\n" which causes a newline to be printed. 
	printf("Hello World!!!\n");

	// This returns the integer 0 from this function. Notice in the function 
	// definition "int main(...) the first int that appears is the "return type"
	// which means that a function has to return a integer value. 
	//
	// In this case we are returning a 0 integer.
	return 0;
}

