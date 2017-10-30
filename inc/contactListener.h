//
// Created by Jacob on 10/27/17.
//

#ifndef GAME_PLAYERCONTACTLISTENER_H
#define GAME_PLAYERCONTACTLISTENER_H

#include "../Box2D/Dynamics/b2WorldCallbacks.h"

namespace bsh
{
    class ContactListener: public b2ContactListener
    {
    public:
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
    };
}

#endif //GAME_PLAYERCONTACTLISTENER_H
