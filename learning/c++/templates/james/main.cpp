
#include <iostream>
#include <typeinfo>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;


class FrameBuffer
{


	vector<> vec;
};


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// A CircularBuffer class.
class CircularBuffer
{
public:
	explicit CircularBuffer(unsigned int size) : bufferSize(size), index(0), startIndex(0)
	{
		buffer = new char[bufferSize];
		memset(buffer, 0xdeadbeef, sizeof(char) * bufferSize);
	}

	~CircularBuffer()
	{
		delete [] buffer;
		buffer = 0;
	}




	template<class T>
	void Append(const T& item)
	{

		//logic to manipulate currentIndex and start index.
		if(WillWrapAroundCircularBuffer(item))
		{
			//WE need to wrap our write around to the beginning.
			//memcpy((buffer + index), &item, sizeof(T));
			//index += sizeof(T);
			printf("ERROR ERROR.. FINISH CODING THE CIRCULAR BUFFER\n");
		}
		else
		{
			memcpy((buffer + index), (void*)&item, sizeof(T));
			index += sizeof(T);

			printf("index = %d, startIndex = %d, sizeof(T) = %d\n", index, startIndex, sizeof(T));
		}


	}

	//Note that this trivial example will only work if we don't fill the buffer.
	void Print() const
	{
		static int count = 0;
		printf("Printing a CircularBuffer: %d", count);
		++count;

		for(int i = startIndex; i < index; ++i)
		{
			if(i % 4 == 0 && i % 16 != 0) 
			{
				printf(" ");
			}
			if(i % 16 == 0)
			{
				printf("\n");	
			}

			printf("%02x", buffer[i]);
		}	
	}


protected:
	template<class T>
	inline bool WillWrapAroundCircularBuffer(const T& item) const
	{
		if(index + sizeof(T) > bufferSize)
		{
			return true;
		}
		return false;
	}

private:
	unsigned int bufferSize;
	int index;
	int startIndex;

	char * buffer;

};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


class ReplayManager
{
public:
	ReplayManager();

	struct TypeInfo
	{
		//typeid id;
		void *theClass;

		TypeInfo() {}
	};

	template<typename T>
	void Track(T & item)
	{
		typeid(item);

		TypeInfo typeInfo;
		typeinfo.id = typeid(item);
		typeinfo.theClass = &item;

		trackingList.push_back(typeInfo);
	}

	vector<TypeInfo> trackingList;
};

ReplayManager gReplayManager;



template<class Derived>
class Replayable
{
	explicit Replayable(Derived* base)
	{
		printf("\t\tReplayable Constructor\n");
		gReplayManager.Track(base);

	}
};



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
struct PODTag {};
struct EngineActorTag{};
struct CustomStorageTag{};


////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// This code represents three different classes that we seek to store
// replays for.

class Vector : public Replayable<Vector>
{
public:
	typedef PODTag ReplayTag;

	Vector() : x(1), y(2), z(3), w(4) {}
	unsigned int x, y , z, w;   //using unsigned int's instead of floats because they are easier to debug.

};

class EngineActor
{
public:
	typedef EngineActorTag ReplayTag;

	EngineActor() : uid(0), position() {}


	inline int GetUID() const { return uid; }
	inline const Vector& GetPosition() const { return position; }

private:
	int uid;
	Vector position;
};


class CustomData
{
public:
	typedef CustomStorageTag ReplayTag;

	CustomData() : a(0x00000001), b(0x00000010), c(0x00000100) {}

	void Record(CircularBuffer & buffer)
	{
		buffer.Append(a);
		buffer.Append(b);
		buffer.Append(c);
	}

private:
	unsigned int a, b, c;
};


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// The following code up to the next section is used to store there
// different kinds of actors. 
// 
// 
namespace Private
{
	template<class T>
	void Record(CircularBuffer & buffer, T& instanceToBeRecorded, PODTag &tag)
	{
		//The search algorithm must be based on only being able to iterate forward.
		cout << "Storing a PODTag structure" << endl;
		buffer.Append(instanceToBeRecorded);
	}

	template<class T>
	void Record(CircularBuffer & buffer, T& instanceToBeRecorded, EngineActorTag &tag)
	{
		//The search algorithm must be based on only 
		//being able to iterate forward.
		cout << "Storing a Engine Actor" << endl;
		EngineActor * actor = reinterpret_cast<EngineActor*>(&instanceToBeRecorded);

		//Store the UID
		buffer.Append(actor->GetUID());

		//Store the vector
		buffer.Append(actor->GetPosition());
	}

	template<class T>
	void Record(CircularBuffer & buffer, T& instanceToBeRecorded, CustomStorageTag &tag)
	{
		//The search algorithm must be based on only being able to iterate forward.
		cout << "Storing a Custom Container" << endl;
		instanceToBeRecorded.Record(buffer);
	}

} // namespace Private

static CircularBuffer buffer(1024*1024);



// The interface into the whoopla
template<class T>
inline void Record(T& instanceToBeRecorded)
{
	typedef typename T::ReplayTag Tag;
	Tag t;

	Private::Record(buffer, instanceToBeRecorded, t);

	//Print out the buffer for easy debugging.
	buffer.Print();
	printf("\n\n");
}



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	Vector myVector;
	//EngineActor myEngineActor;
	//CustomData myCustomData;

	Record(myVector);
	//Record(myEngineActor);
	//Record(myCustomData);
	
	return 0;
}





