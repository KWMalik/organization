#include <iostream>

#include "threads.h"

using namespace std;

template
<
	template <class, class> class ThreadingPolicy = DEFAULT_THREADING
>
class Worker
{
public:
	//typedef ThreadingModel<MyAllocator, MutexPolicy> MyThreadingModel;

	Worker() : result(0) {}
	~Worker() {}

	void Execute()
	{
		result += 1;
	}

	int GetResult() const { return result; }

private:
	int result;
};



int main()
{
	cout << "Hello World!!" << endl;
	return 0;
}

