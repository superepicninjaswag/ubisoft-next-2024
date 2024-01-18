#include "stdafx.h"

#include "EntityDescriptor.h"

EntityDescriptor::EntityDescriptor(unsigned short assignedId, unsigned short assignedVersion)
{
	id = assignedId;
	version = assignedVersion;
}

bool EntityDescriptor::operator==(EntityDescriptor descriptorToCompare)
{
	return (id == descriptorToCompare.id) && (version == descriptorToCompare.version);
}

