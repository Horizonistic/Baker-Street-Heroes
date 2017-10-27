//
// Created by Jacob on 10/23/17.
//

#include "../inc/resources.h"

oxygine::Resources bsh::Res::ui;
oxygine::Resources bsh::Res::terrain;
oxygine::Resources bsh::Res::characters;

void bsh::Res::loadUI()
{
    bsh::Res::ui.loadXML("ui.xml");
}

void bsh::Res::loadTerrain()
{
    bsh::Res::terrain.loadXML("world.xml");
}

void bsh::Res::loadCharacters()
{
    bsh::Res::characters.loadXML("characters.xml");
}

void bsh::Res::free()
{
    bsh::Res::ui.free();
    bsh::Res::terrain.free();
}
