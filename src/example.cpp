#include <gamescene.h>
#include "oxygine-framework.h"

#include "../inc/player.h"
#include "../inc/scene.h"

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

b2BodyDef* groundBodyDef;
b2Body* groundBody;
oxygine::Box9Sprite* spGroundBox;

oxygine::Resources resources;


void example_preinit()
{
}

//called from main.cpp
void example_init()
{
    
    resources.loadXML("world.xml");
    // Creating the world, because it's just that easy
    b2Vec2 gravity(bsh::constant::GRAVITY);
    world = new b2World(gravity);
    
        // Creating the ground floor
    // This creates a static body, immovable,
    // This is the properties
    b2Vec2 groundPosition = b2Vec2(0.0f, 5.0f);
    groundBodyDef = new b2BodyDef;
    groundBodyDef->position.Set(groundPosition.x, groundPosition.y);
    groundBody = world->CreateBody(groundBodyDef);
    
    
    // This creates a polygon
    // This is the shape
    b2Vec2 groundSize = b2Vec2(20.0f, 5.0f);
    
    b2PolygonShape* groundBox = new b2PolygonShape;
    groundBox->SetAsBox(groundSize.x / 2, groundSize.y / 2, {groundSize.x / 2, groundSize.y / 2}, 0);
    groundBody->CreateFixture(groundBox, 0.0f);
    groundBox->SetAsBox(20.0f, 1.0f, {30.0f, 0.0f}, 0);
    groundBody->CreateFixture(groundBox, 0.0f);
    
    
    oxygine::ResAnim *terrain = resources.getResAnim("tempterrain");
    
    spGroundBox = new Box9Sprite();
    spGroundBox->setResAnim(terrain);
    
//    spGroundBox->setAnchor(game::convert(groundPosition));
    spGroundBox->setPosition(bsh::convert(groundPosition));
    spGroundBox->setSize(bsh::convert(groundSize));
    getStage()->addChild(spGroundBox);
    
    
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
    
    bsh::GameScene::instance = new bsh::GameScene(*world);
    
    
    //load xml file with resources definition

    //lets create our client code simple actor
    //spMainActor was defined above as smart intrusive pointer (read more: http://www.boost.org/doc/libs/1_60_0/libs/smart_ptr/intrusive_ptr.html)
//    spMainActor actor = new MainActor;

    //and add it to Stage as child
//    getStage()->addChild(actor);
}


//called each frame from main.cpp
void example_update() {
    // timeStep, velocityIterations, positionIterations
    world->Step(1.0f / 60.0f, 6, 2);
}

//called each frame from main.cpp
void example_destroy()
{
    //free previously loaded resources
//    gameResources.free();
}
