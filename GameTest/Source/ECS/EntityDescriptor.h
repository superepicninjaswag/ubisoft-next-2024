#pragma once


struct EntityDescriptor
{
	unsigned short id;
	unsigned short version;

	EntityDescriptor(unsigned short assignedId, unsigned short assignedVersion);
};