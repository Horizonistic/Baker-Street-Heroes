//
// Created by Jacob on 10/10/17.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <oxygine-framework.h>
#include "../Box2D/Dynamics/b2World.h"
#include <global.h>



namespace bsh
{
    class Player: public oxygine::Actor
    {
    public:
        enum moveState {
            STOP,
            LEFT,
            RIGHT,
        };
    
        explicit Player(b2World &world);
        ~Player() override;
        std::vector< b2Vec2 > getData();
    
    protected:
        void doUpdate(const oxygine::UpdateState &us) override;
    
    private:
        b2World &_world;
        b2Body *_body;
    
        b2Vec2 velocity;
        moveState _moveState;
    
        oxygine::Resources _resources;
        oxygine::spSprite _sprite;
    };
}

DECLARE_SMART(bsh::Player, spPlayer);

#endif //GAME_PLAYER_H