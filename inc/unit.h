//
// Created by Jacob on 10/19/17.
//

#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include <oxygine-framework.h>
#include "../Box2D/Common/b2Math.h"

namespace bsh
{
    class Game;
    class Unit;
}
DECLARE_SMART(bsh::Unit, spUnit);

namespace bsh
{
    class Unit: public oxygine::Object
    {
    public:
        Unit();
        
        void init(const b2Vec2 &pos, Game *game);
        
        const b2Vec2& getPositionn() const;
        
        void update(const oxygine::UpdateState &us);
        bool isDead() const {return this->_dead;}
        
    protected:
        virtual void _init() {}
        virtual void _update(const oxygine::UpdateState &us) {}
        
        oxygine::spActor _view;
        Game* _game;
        bool _dead;
    };
}

#endif //GAME_UNIT_H
