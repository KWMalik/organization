#ifndef _GROWTH_MODELS_H_
#define _GROWTH_MODELS_H_

#define Assert(...)

/// This just shows the required interface for this class.
class Interface_Growth_Model
{
public:
    bool Grow(int recommenednumberOfElementsToGrowBy) { return false; }
    bool Trim() { return false; }
};

/// 
class No_Growth
{
public:
    bool Grow(int recommenednumberOfElementsToGrowBy)
    {
        Assert(TJB, "No growth models can never grow");
        return false;
    }

    bool Trim()
    {
        Assert(TJB, "No growth models have no extra memory blocks so they can never grow.");
        return false;
    }
};

#endif //_GROWTH_MODELS_H_

