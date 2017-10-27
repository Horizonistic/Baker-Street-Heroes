//
// Created by Jacob on 10/23/17.
//

#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#include "oxygine-framework.h"

namespace bsh
{
    class Res
    {
    public:
        static oxygine::Resources ui;
        static oxygine::Resources terrain;
        static oxygine::Resources characters;
    
        Res() {};
        static void loadUI();
        static void loadTerrain();
        static void loadCharacters();
        static void free();
    };
}

#endif //GAME_RESOURCES_H