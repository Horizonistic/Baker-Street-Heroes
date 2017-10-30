//
// Created by Jacob on 10/11/17.
//

#ifndef GAME_GLOBAL_H
#define GAME_GLOBAL_H

#include <math/Vector2.h>
#include "../Box2D/Common/b2Math.h"
#include "player.h"

namespace bsh
{
    namespace constant
    {
        // This is the pixel/meter scale.  100.0f = 100 pixels per meter
        const float SCALE = 100.0f;
        const b2Vec2 GRAVITY = b2Vec2{0.0f, 25.0f};
    }
    
    // b2Vec2 is meters, and oxygine::Vector2 is pixels
    // Going from b2 to oxygine is meters to pixels
    // And vice versa, oxygine to b2 is pixels to meters
    static b2Vec2 convert(const oxygine::Vector2& pos)
    {
        return b2Vec2(pos.x / constant::SCALE, pos.y / constant::SCALE);
    }
    
    static oxygine::Vector2 convert(const b2Vec2& pos)
    {
        return oxygine::Vector2(pos.x * constant::SCALE, pos.y * constant::SCALE);
    }
}

#endif //GAME_GLOBAL_H
