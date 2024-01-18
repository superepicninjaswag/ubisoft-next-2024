#pragma once


#include "EntityDescriptor.h"

template <typename T>
class ComponentPool
{
private:
    unsigned short componentsInPool = 0;
    std::vector<unsigned short> _sparse;
    std::vector<EntityDescriptor> _mirror;
public:
    std::vector<T> _dense;

    ComponentPool();
    template <typename... Args>
    void Add(EntityDescriptor idToAdd, Args&&... args);
    T &Get(unsigned short idToGet);
    size_t Size();
    EntityDescriptor MirrorIdToEntityId(unsigned short index);
};


template<typename T>
inline ComponentPool<T>::ComponentPool() : _sparse(IDManager::ENTITY_LIMIT)
{
    _dense.reserve(IDManager::ENTITY_LIMIT);
    _mirror.reserve(IDManager::ENTITY_LIMIT);
}

template <typename T>
template <typename... Args>
void ComponentPool<T>::Add(EntityDescriptor idToAdd, Args&&... args)
{
    _mirror.push_back(idToAdd);
    _dense.emplace_back(std::forward<Args>(args)...);
    _sparse[idToAdd.id] = componentsInPool++;
}

template <typename T>
T &ComponentPool<T>::Get(unsigned short idToGet)
{
    return _dense[_sparse[idToGet]];
}

template <typename T>
size_t ComponentPool<T>::Size()
{
    return _dense.size();
}

template <typename T>
EntityDescriptor ComponentPool<T>::MirrorIdToEntityId(unsigned short index)
{
    return _mirror[index];
}