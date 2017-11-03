//
// Created by Jacob on 11/2/17.
//

#ifndef GAME_MENUSCENE_H
#define GAME_MENUSCENE_H

#include "../Box2D/Dynamics/b2World.h"
#include <scene.h>

namespace bsh
{
    class MenuScene;
}

DECLARE_SMART(bsh::MenuScene, spMenuScene)

namespace bsh
{
    class MenuScene : public Scene
    {
    public:
        static spMenuScene instance;
        MenuScene(b2World &world);
    
    private:
        const b2World &_world;
        oxygine::spActor _button;
        
        void onEvent(oxygine::Event* ev);
    };
}

#endif //GAME_MENUSCENE_H