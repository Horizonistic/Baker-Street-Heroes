#include <gamescene.h>
#include <iostream>
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
    
    
    // Begin reading level file
    oxygine::file::buffer buffer;
    oxygine::file::read("levels.xml", buffer);
    
    pugi::xml_document document;
    document.load_buffer(&buffer.data[0], buffer.size());
    
    pugi::xml_node xml_node;
    
    // Iterate over all sprites
    for (const pugi::xpath_node sprite : document.select_node("static").node().select_nodes("sprite"))
    {
        std::cout << sprite.node().name() << std::endl;
    
        b2Vec2 groundPosition = b2Vec2(0.0f, 0.0f);
        b2Vec2 groundSize = b2Vec2(0.0f, 0.0f);
    
        // Iterate over all attributes of the sprite
        for (auto attributes : sprite.node().attributes())
        {
            oxygine::log::messageln(attributes.name());
            if (strcasecmp(attributes.name(), "posX") == 0)
            {
                groundPosition.x = attributes.as_float();
            }
            else if (strcasecmp(attributes.name(), "posY") == 0)
            {
                groundPosition.y = attributes.as_float();
            }
            else if (strcasecmp(attributes.name(), "sizeX") == 0)
            {
                groundSize.x = attributes.as_float();
            }
            else if (strcasecmp(attributes.name(), "sizeY") == 0)
            {
                groundSize.y = attributes.as_float();
            }
        }
        // Creating the ground floor
        // This creates a static body, immovable,
        // This is the properties
        groundBodyDef = new b2BodyDef;
        groundBodyDef->position.Set(groundPosition.x, groundPosition.y);
        groundBody = world->CreateBody(groundBodyDef);
        
        // This creates a polygon
        // This is the shape
        b2PolygonShape* groundBox = new b2PolygonShape;
        groundBox->SetAsBox(groundSize.x / 2, groundSize.y / 2, {groundSize.x / 2, groundSize.y / 2}, 0);
        groundBody->CreateFixture(groundBox, 0.0f);
//        groundBox->SetAsBox(20.0f, 1.0f, {30.0f, 0.0f}, 0);
//        groundBody->CreateFixture(groundBox, 0.0f);
        
        spGroundBox = new Box9Sprite();
//        spGroundBox->setResAnim(terrain);

        spGroundBox->setPosition(bsh::convert(groundPosition));
        spGroundBox->setSize(bsh::convert(groundSize));
        getStage()->addChild(spGroundBox);
    }
    
    
    
    
//    oxygine::ResAnim *terrain = resources.getResAnim("tempterrain");
    
    
    
    
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
