//
// Created by Jacob on 10/19/17.
//

#include "../inc/game.h"
#include "../inc/player.h"
#include "../inc/resources.h"

#include "../Box2D/Collision/Shapes/b2PolygonShape.h"
#include "../Box2D/Dynamics/b2Body.h"
#include "../Box2D/Dynamics/b2Fixture.h"
#include "../Box2D/Collision/Shapes/b2CircleShape.h"

// todo: be wary of class size; it seems to be doing too much with too long of functions
// todo: maybe add ability to switch sprites on the fly?
// This could require a different level.xml files per theme.
// Only sprite switching is necessary, so it might be possible to iterate over all sprites
// that are attached to the game instance and change them that way.  It might also require
// straight up removing them all and then reattaching them all the the b2Bodies.

bsh::Game::Game(): _world()
{}

bsh::Game::~Game()
= default;

void bsh::Game::init(b2World *world)
{
    this->_world = world;
    this->setSize(oxygine::getStage()->getSize());
    
    // todo: determine background sprite from level data
    // Background
//    oxygine::spSprite background = new oxygine::Sprite;
//    background->setResAnim();
//    background->attachTo(this);
    
    // todo: add UI class??
    // todo: add basic health capability and UI
    // todo: add pause menu and return to main menu capability
    // UI
//    this->_ui = new oxygine::Actor;
//    this->_ui->attachTo(this);
//    this->_ui->setPriority(1);
    // Attach stuff to the UI here
    
}

void bsh::Game::loadLevel(std::string level)
{
    // todo: allow definition of physical bodies with sprites, physical bodies without sprites, and just sprites
    // Remove old bodies
    b2Body *body = this->_world->GetBodyList();
    while (body)
    {
        this->_world->DestroyBody(body);
        body = body->GetNext();
    }
    // Remove old sprites
    this->removeChildren();
    
    // Load file to buffer
    oxygine::file::buffer buffer;
    oxygine::file::read(level, buffer);
    
    // Load buffer as XML document
    pugi::xml_document document;
    document.load_buffer(&buffer.data[0], buffer.size());
    
    // Player initialization
    // todo: allow player sprite choosing from menu
    // Menu will chose which character to use, while the level file will determine where the players spawn
    // Also need to allow amount of players selection, and figure out how to define multiple spawnpoints in a level file
    for (const pugi::xpath_node player : document.select_node("bodies").node()
                                            .select_node("player").node()
                                            .select_nodes("sprite"))
    {
        // Settings for the player
        b2Vec2 playerPosition = b2Vec2(0.0f, 0.0f);
        b2Vec2 playerSize = b2Vec2(0.0f, 0.0f);
        oxygine::ResAnim *resAnim;
    
        for (auto attributes : player.node().attributes())
        {
            // Iterate over all attributes terrain
            if (strcasecmp(attributes.name(), "name") == 0)
            {
                resAnim = bsh::Res::characters.getResAnim(attributes.as_string());
            }
            else if (strcasecmp(attributes.name(), "posX") == 0)
            {
                playerPosition.x = attributes.as_float();
            }
            else if (strcasecmp(attributes.name(), "posY") == 0)
            {
                playerPosition.y = attributes.as_float();
            }
            else if (strcasecmp(attributes.name(), "sizeX") == 0)
            {
                playerSize.x = attributes.as_float();
            }
            else if (strcasecmp(attributes.name(), "sizeY") == 0)
            {
                playerSize.y = attributes.as_float();
            }
        }
    
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(playerPosition.x, playerPosition.y);
        bodyDef.fixedRotation = true;
        b2Body *body = this->_world->CreateBody(&bodyDef);
    
        // Creating polygon
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(playerSize.x / 2, playerSize.y / 2, {playerSize.x / 2, playerSize.y / 2}, 0);
        
        // Create body's fixture (figure out more of what that means)
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 10.0f;
        fixtureDef.friction = 0.0f;
    
        body->CreateFixture(&fixtureDef);
        
        // Foot sensor sensor
        b2CircleShape circleShape;
        circleShape.m_radius = playerSize.x / 4.0f;
        circleShape.m_p = b2Vec2(playerSize.x / 2, playerSize.y);
        
        fixtureDef.shape = &circleShape;
        fixtureDef.isSensor = true;
        fixtureDef.filter.categoryBits = oxygine::entityType::PLAYER_SENSOR;
        fixtureDef.filter.maskBits = oxygine::entityType::TERRAIN;
        body->CreateFixture(&fixtureDef);
    
        oxygine::getStage()->removeChild(this->_player);
        this->_player = new Player(*this->_world, body, resAnim);
        oxygine::getStage()->addChild(this->_player);
    }
    
    
    
    // Iterate over all terrain
    b2BodyDef* groundBodyDef;
    b2Body* groundBody;
    oxygine::Box9Sprite* spGroundBox;
    for (const pugi::xpath_node sprite : document.select_node("bodies").node()
                                                 .select_node("terrain").node()
                                                 .select_nodes("sprite"))
    {
        // Settings for the terrain
        // todo: remove duplication of code between loading terrain and player
        b2Vec2 groundPosition = b2Vec2(0.0f, 0.0f);
        b2Vec2 groundSize = b2Vec2(0.0f, 0.0f);
        oxygine::ResAnim *resAnim;
        oxygine::Box9Sprite::StretchMode stretchModeHorizontal;
        oxygine::Box9Sprite::StretchMode stretchModeVertical;
        
        // Iterate over all attributes terrain
        for (auto attributes : sprite.node().attributes())
        {
            if (strcasecmp(attributes.name(), "name") == 0)
            {
                resAnim = bsh::Res::terrain.getResAnim(attributes.as_string());
            }
            else if (strcasecmp(attributes.name(), "stretchHor") == 0)
            {
                switch (attributes.as_int())
                {
                    case 1:
                        stretchModeHorizontal = oxygine::Box9Sprite::StretchMode::TILING;
                        break;
                    
                    case 2:
                        stretchModeHorizontal = oxygine::Box9Sprite::StretchMode::TILING_FULL;
                        break;
                    
                    case 3:
                        stretchModeHorizontal = oxygine::Box9Sprite::StretchMode::STRETCHING;
                        break;
                    
                    default:
                        stretchModeHorizontal = oxygine::Box9Sprite::StretchMode::STRETCHING;
                        break;
                }
                
            }
            else if (strcasecmp(attributes.name(), "stretchVer") == 0)
            {
                switch (attributes.as_int())
                {
                    case 1:
                        stretchModeVertical = oxygine::Box9Sprite::StretchMode::TILING;
                        break;
                    
                    case 2:
                        stretchModeVertical = oxygine::Box9Sprite::StretchMode::TILING_FULL;
                        break;
                    
                    case 3:
                        stretchModeVertical = oxygine::Box9Sprite::StretchMode::STRETCHING;
                        break;
                    
                    default:
                        stretchModeVertical = oxygine::Box9Sprite::StretchMode::STRETCHING;
                        break;
                }
            }
            else if (strcasecmp(attributes.name(), "posX") == 0)
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
        groundBody = this->_world->CreateBody(groundBodyDef);
        
        // This creates a polygon
        // This is the shape
        b2PolygonShape* groundBox = new b2PolygonShape;
        groundBox->SetAsBox(groundSize.x / 2, groundSize.y / 2, {groundSize.x / 2, groundSize.y / 2}, 0);
        groundBody->CreateFixture(groundBox, 0.0f);
        
        // Sprite
        spGroundBox = new oxygine::Box9Sprite();
        spGroundBox->setResAnim(resAnim);
        spGroundBox->setHorizontalMode(stretchModeHorizontal);
        spGroundBox->setVerticalMode(stretchModeVertical);
        spGroundBox->setPosition(bsh::convert(groundPosition));
        spGroundBox->setSize(bsh::convert(groundSize));
        spGroundBox->setEntityType(oxygine::TERRAIN);
        
        // Attach to Game instance
        this->addChild(spGroundBox);
    }
}

void bsh::Game::doUpdate(const oxygine::UpdateState &us)
{

}
