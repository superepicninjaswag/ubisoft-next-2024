#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"
#include "Contact.h"

void ResolveCollisions(ECS& ecs, std::vector<Contact>& contactQueue);