#include "stdafx.h"

#include "EntityDescriptor.h"

EntityDescriptor::EntityDescriptor() : id(0), version(0) {}

EntityDescriptor::EntityDescriptor(unsigned short assignedId, unsigned short assignedVersion)
{
	id = assignedId;
	version = assignedVersion;
}

bool EntityDescriptor::operator==(EntityDescriptor descriptorToCompare)
{
	return (id == descriptorToCompare.id) && (version == descriptorToCompare.version);
}

bool EntityDescriptor::isValid()
{
	return id != 0;
}

