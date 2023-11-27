#include "stdafx.h"

#include "IDManager.h"

IDManager::IDManager()
{
    _ids = std::vector<uint32_t>(65536);
}

uint32_t IDManager::CreateId()
{
    return _nextNewID++;

    /*
    if (_recyclable == 0)
    {
        // TODO: evaluate if you need versioning
        _ids[_nextNewId] = 0xFFFF0000 & (_nextNewId << 4);
    }
    else
    {
        // TODO: Implement recycling
    }  
    */  
}