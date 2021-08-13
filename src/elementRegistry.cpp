// Types
#include "./elements/solids/solid.cpp"
#include "./elements/liquids/liquid.cpp"

// World
#include "./core/chunk.cpp"
#include "./core/world.cpp"

// Elements
#include "./elements/solids/moveables/sand.cpp"
#include "./elements/solids/unmoveables/stone.cpp"
#include "./elements/liquids/water.cpp"

// Create Particle
Element *create_particle(s32 id)
{
    switch (id)
    {
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
void generateAllGUIs()
{
    generateGUI(new Sand());
    generateGUI(new Water());
    generateGUI(new Stone());

    fixGUIs();
}