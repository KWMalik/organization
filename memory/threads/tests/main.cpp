#include <iostream>

#include "threads.h"

using namespace std;

template<class ThreadingPolicy>
class Worker : public ThreadingPolicy
{
public:
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

	typedef vector<Worker> Container;
	Container workers;

	for(Container::iterator it = workers.begin(); it != workers.end(); ++it)
	{
		it->Execute();
	}


	return 0;
}

