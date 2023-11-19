#pragma once

#include <vector>

class IDManager
{
private:
    std::vector<uint32_t> _ids;
    uint32_t _nextNewID = 0;
    int _recyclable = 0;
public:
    IDManager();
    uint32_t CreateId();
};
