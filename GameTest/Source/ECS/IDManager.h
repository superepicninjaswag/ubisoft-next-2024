#pragma once

#include "EntityDescriptor.h"

class IDManager
{
private:
    std::vector<EntityDescriptor> _descriptors;
    int _availableForRecycling = 0;
    EntityDescriptor _next = EntityDescriptor(0, 0);
public:
    static const unsigned int ENTITY_LIMIT = 65536;
    IDManager();
    EntityDescriptor CreateId();
    void DeleteId(EntityDescriptor entityToDelete);
};
