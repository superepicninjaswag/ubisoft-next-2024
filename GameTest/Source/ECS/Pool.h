#pragma once

#include <vector>
#include <utility>

template <typename T>
class Pool
{
private:
    std::vector<int> _sparse;
    std::vector<int> _mirror;
    int nextAvailableIndex = 0;
public:
    std::vector<T> _dense;

    Pool(): _sparse(65536), _dense(), _mirror(){}
    template <typename... Args>
    void Add(int id, Args&&... args);
    T *Get(int id);
    size_t Size();
    int MirrorIdToEntityId(int index);
};

template <typename T>
template <typename... Args>
void Pool<T>::Add(int id, Args&&... args)
{
    _mirror.push_back(id);
    _dense.emplace_back(std::forward<Args>(args)...);
    _sparse[id] = nextAvailableIndex++;
}

template <typename T>
T *Pool<T>::Get(int id)
{
    if (id < 0 || id >= _sparse.size())
    {
        // Invalid ID
        return nullptr;
    }
    else if (_mirror[_sparse[id]] == id)
    {
        // Found it
        return &_dense[_sparse[id]];
    }
    else
    {
        // Entity doesn't have the component
        return nullptr;
    }
}

template <typename T>
size_t Pool<T>::Size()
{
    return _dense.size();
}

template <typename T>
int Pool<T>::MirrorIdToEntityId(int index)
{
    return _mirror[index];
}