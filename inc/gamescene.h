//
// Created by Jacob on 10/19/17.
//

#ifndef GAME_GAMESCENE_H
#define GAME_GAMESCENE_H

#include "../inc/scene.h"
#include "../inc/game.h"

namespace bsh
{
    class GameScene;
}

DECLARE_SMART(bsh::GameScene, spGameScene)

namespace bsh
{
    class GameScene : public Scene
    {
    public:
        static spGameScene instance;
        GameScene(b2World &world);
    
    private:
        b2World &_world;
        spGame _game;
        oxygine::spActor _button;
        
        void onEvent(oxygine::Event* ev);
    };
}

#endif //GAME_GAMESCENE_H
