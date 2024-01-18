#include "stdafx.h"

#include "IDManager.h"

IDManager::IDManager()
{
    _descriptors.emplace_back(0, 0);    // The descriptor with id 0 is reserved to represent failure in CreateID()
}

EntityDescriptor IDManager::CreateId()
{
    if (_availableForRecycling > 0)
    {
        if (_availableForRecycling == 1)
        {
            EntityDescriptor descriptorToReturn = _next;
            std::swap(_next, _descriptors[_next.id]);
            _next = EntityDescriptor(0, 0);
            _availableForRecycling -= 1;
            return descriptorToReturn;
        }
        else
        {
            EntityDescriptor descriptorToReturn = _next;
            std::swap(_next, _descriptors[_next.id]);
            _availableForRecycling -= 1;
            return descriptorToReturn;
        }
    }
    else if (_descriptors.size() < ENTITY_LIMIT)
    {
        _descriptors.emplace_back(_descriptors.size() - 1, 0);
        return _descriptors.back();
    }
    else
    {
        return _descriptors[0];
    }
}

void IDManager::DeleteId(EntityDescriptor entityToDelete)
{
    return; //TODO: !!!!!!!
}
