#pragma once


struct EntityDescriptor
{
	unsigned short id;
	unsigned short version;

	EntityDescriptor();
	EntityDescriptor(unsigned short assignedId, unsigned short assignedVersion);
	bool operator==(EntityDescriptor descriptorToCompare);
	bool isValid();
};