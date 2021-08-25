// Types
#include "elements/solids/solid.h"
#include "elements/liquids/liquid.h"

// World
#include "core/chunk.h"
#include "core/world.h"

// Elements
#include "elements/solids/moveables/sand.h"
#include "elements/solids/unmoveables/stone.h"
#include "elements/liquids/water.h"

// Create Particle
Element* create_particle(s32 id) {
	switch (id) {
	case SAND:
		return new Sand();
	case WATER:
		return new Water();
	case STONE:
		return new Stone();
	default:
		return new Element();
	}
}

// GUIs
void generateAllGUIs() {
	generateGUI(new Sand());
	generateGUI(new Water());
	generateGUI(new Stone());

	fixGUIs();
}