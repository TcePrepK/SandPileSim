// IDs
#define mat_id_empty 0
#define mat_id_sand 1
#define mat_id_water 2
#define mat_id_stone 3

// Required Ones
#include "./elements/element.cpp"
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
    case mat_id_sand:
        return new Sand();
    case mat_id_water:
        return new Water();
    case mat_id_stone:
        return new Stone();
    default:
        return new Element();
    }
}

// GUIs
void generateAllGUIs()
{
    Element *e;

    e = create_particle(SAND);
    generateGUI(SAND, e->color);
    delete e;

    e = create_particle(mat_id_water);
    generateGUI(mat_id_water, e->color);
    delete e;

    e = create_particle(mat_id_stone);
    generateGUI(mat_id_stone, e->color);
    delete e;
}