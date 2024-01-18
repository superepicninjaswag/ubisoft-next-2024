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
    T &Get(unsigned short idToGet);
    size_t Size();
    EntityDescriptor MirrorToEntityDescriptor(unsigned short index);
    bool Delete(EntityDescriptor entityToDelete);
    bool Has(EntityDescriptor entityToCheck);

    template <typename... Args>
    void Add(EntityDescriptor idToAdd, Args&&... args);
};


template<typename T>
inline ComponentPool<T>::ComponentPool() : _sparse(IDManager::ENTITY_LIMIT), _mirror(IDManager::ENTITY_LIMIT)
{
    _dense.reserve(IDManager::ENTITY_LIMIT);
}

template <typename T>
template <typename... Args>
void ComponentPool<T>::Add(EntityDescriptor idToAdd, Args&&... args)
{
    _dense.emplace_back(std::forward<Args>(args)...);
    _sparse[idToAdd.id] = componentsInPool;
    _mirror[componentsInPool] = idToAdd;
    componentsInPool++;
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
EntityDescriptor ComponentPool<T>::MirrorToEntityDescriptor(unsigned short index)
{
    return _mirror[index];
}

template<typename T>
inline bool ComponentPool<T>::Delete(EntityDescriptor entityToDelete)
{
    if (Has(entityToDelete))
    {
        if (componentsInPool == 1)
        {
            _dense.pop_back();
            _mirror[0].id = 0;
            _mirror[0].version = 0;
        }
        else
        {
            unsigned short indexOfEntityToDelete = _sparse[entityToDelete.id];
            unsigned short indexOfLastEntity = componentsInPool - 1;
            unsigned short idOfLastEntity = _mirror[indexOfLastEntity].id;

            std::swap(_dense[indexOfLastEntity], _dense[indexOfEntityToDelete]);
            std::swap(_mirror[indexOfLastEntity], _mirror[indexOfEntityToDelete]);

            _dense.pop_back();

            _sparse[idOfLastEntity] = indexOfEntityToDelete;
            _mirror[indexOfLastEntity].id = 0;
            _mirror[indexOfLastEntity].version = 0;
        }

        componentsInPool--;
        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
inline bool ComponentPool<T>::Has(EntityDescriptor entityToCheck)
{
    return (entityToCheck.id != 0) && (_mirror[_sparse[entityToCheck.id]] == entityToCheck);
}
