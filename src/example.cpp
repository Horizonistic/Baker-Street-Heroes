#include <gamescene.h>
#include <iostream>
#include "oxygine-framework.h"

#include "../inc/player.h"
#include "../inc/scene.h"
#include "../inc/resources.h"

#include "../Box2D/Collision/Shapes/b2PolygonShape.h"
#include "../Box2D/Dynamics/b2Body.h"
#include "../Box2D/Dynamics/b2Fixture.h"
#include "Box2DDebugDraw.h"

using namespace oxygine;

//declare spMainActor as intrusive_ptr holder of MainActor
//typedef oxygine::intrusive_ptr<MainActor> spMainActor;
//you could use DECLARE_SMART preprocessor definition it does the same:
//DECLARE_SMART(MainActor, spMainActor)

//typedef oxygine::intrusive_ptr<game::Player> spPlayer;

// All Box2D globals (like world)
Box2DDraw *_debugDraw;
b2World* world;

void example_preinit()
{
}

//called from main.cpp
void example_init()
{
    if (_debugDraw)
    {
        _debugDraw->detach();
        _debugDraw = 0;
        return;
    }
    _debugDraw = new Box2DDraw();
    _debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
    _debugDraw->attachTo(getStage());
    _debugDraw->setWorld(bsh::constant::SCALE, world);
    _debugDraw->setPriority(1);
    
    bsh::Res::loadUI();
    bsh::Res::loadTerrain();
    bsh::Res::loadCharacters();
    
    // Creating the world, because it's just that easy
    b2Vec2 gravity(bsh::constant::GRAVITY);
    world = new b2World(gravity);

    bsh::GameScene::instance = new bsh::GameScene(*world);
    bsh::GameScene::instance->show();
    
}

//called each frame from main.cpp
void example_update() {
    // timeStep, velocityIterations, positionIterations
    // todo: get physics to work with any framerate
    // This will require a way to apply force based on us.dt in doUpadate in classes like bsh::Player
    world->Step(IVideoDriver::_stats.duration / 1000.0f, 6, 2);
    
}

//called each frame from main.cpp
void example_destroy()
{
    bsh::Res::free();
}