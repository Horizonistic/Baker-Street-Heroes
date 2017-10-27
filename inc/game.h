//
// Created by Jacob on 10/19/17.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "oxygine-framework.h"
#include "../Box2D/Dynamics/b2World.h"
#include "unit.h"

namespace bsh
{
    class Game;
    class Player;
    class Unit;
}

DECLARE_SMART(bsh::Game, spGame);
DECLARE_SMART(bsh::Player, spPlayer);
//DECLARE_SMART(bsh::Unit, spUnit);

namespace bsh
{
    class Game: public oxygine::Actor
    {
    public:
        Game();
        ~Game();
        
        void init(b2World *world);
        void loadLevel(std::string level);

    protected:
        friend class Unit;
    
        b2World *_world;
        oxygine::spActor _ui;
        spPlayer _player;
        std::vector< spUnit > _units;
        
        void doUpdate(const oxygine::UpdateState &us) override;
    };
}

#endif //GAME_GAME_H