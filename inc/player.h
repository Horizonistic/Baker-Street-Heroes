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
    class Player;
}

DECLARE_SMART(bsh::Player, spPlayer);

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
    
        Player(b2World &world, b2Body *body, oxygine::ResAnim *resAnim);
        ~Player() override;
        
        void beginTouchingGround();
        void endTouchingGround();
        
        std::vector< b2Vec2 > getData();
    
    protected:
        void doUpdate(const oxygine::UpdateState &us) override;
    
    private:
        b2World &_world;
        b2Body *_body;
//        b2Body *expiriment;
    
        bool _canJump;
        moveState _moveState;
    
        oxygine::Resources _resources;
        oxygine::spSprite _sprite;
    };
}

#endif //GAME_PLAYER_H
