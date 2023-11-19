#include "stdafx.h"

#include "GetModelNumber.h"

int GetModelNumber(Pool<MeshResourceComponent> &resources, std::string model)
{
    for (int i = 0; i < resources.Size(); i++)
    {
        if (resources._dense[i].model == model)
        {
            return resources.MirrorIdToEntityId(i);
        }
    }

    return -1;
}