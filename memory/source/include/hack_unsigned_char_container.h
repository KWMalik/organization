/// \file   hack_unsigned_char_container.h
/// \author Toby Banks
///
/// In practice I'd write a compile-time STL compliant array instead of using this hack.
/// This container only implements the interface required by the growth policies.
///

#ifndef _HACK_UNSIGNED_CHAR_CONTAINER_H_
#define _HACK_UNSIGNED_CHAR_CONTAINER_H_


/// \class  Hack_Unsigned_Char_Container 
/// \author Toby Banks
///
/// Well, I'm not proud of this one, but I've ran out of time that I have allocated
/// to this example code so here we go.
///
/// I'm not going to document this as it should be readily apparent.
///
class Hack_Unsigned_Char_Container 
{
public:
//    class iterator_class
//    {
//    public:
//        unsigned char&  operator *() { return *buffer; }
//
//        unsigned char * operator++(); { return buffer + 1; }
//
//        unsigned char * buffer; ///< A pointer to our buffer.
//    };
//
//    typedef iterator_class iterator;

    typedef unsigned char * iterator;

    iterator begin() 
    {
        //return _it;
        return buffer;
    }

    iterator end() 
    {
        //return (_it.buffer + 1);
        return buffer + 1;
    }

    void push_back(unsigned char *buffer_to_add)
    {
        buffer = buffer_to_add;
    }

protected:
   // iterator_class _it;
    unsigned char * buffer; ///< A pointer to our buffer.
};


#endif // _HACK_UNSIGNED_CHAR_CONTAINER_H_

