//
// Created by Jacob on 10/27/17.
//

#include <contactListener.h>
#include <core/log.h>
#include <player.h>
#include <../Box2D/Dynamics/Contacts/b2Contact.h>

void bsh::ContactListener::BeginContact(b2Contact *contact)
{
    oxygine::log::messageln("Begin Contact");
    
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    // Make sure only one of the fixtures was a sensor
    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();
    if (!(sensorA ^ sensorB))
    {
        return;
    }
    
    auto player1 = (Player*) fixtureA->GetBody()->GetUserData();
    if (player1 != nullptr && player1->getEntityType() == oxygine::entityType::PLAYER)
    {
        oxygine::log::messageln("Begin A");
        player1->beginTouchingGround();
    }
    auto player2 = (Player*) fixtureB->GetBody()->GetUserData();
    if (player2 != nullptr && player2->getEntityType() == oxygine::entityType::PLAYER)
    {
        oxygine::log::messageln("Begin B");
        player2->beginTouchingGround();
    }
}

void bsh::ContactListener::EndContact(b2Contact *contact)
{
    oxygine::log::messageln("End Contact");
    
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    // Make sure only one of the fixtures was a sensor
    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();
    if (!(sensorA ^ sensorB))
    {
        return;
    }
    
    auto player = (Player*) fixtureA->GetBody()->GetUserData();
    if (player != nullptr && player->getEntityType() == oxygine::entityType::PLAYER)
    {
        oxygine::log::messageln("End A");
        player->endTouchingGround();
    }
    player = (Player*) fixtureB->GetBody()->GetUserData();
    if (player != nullptr && player->getEntityType() == oxygine::entityType::PLAYER)
    {
        oxygine::log::messageln("End B");
        player->endTouchingGround();
    }
}
