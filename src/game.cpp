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

bsh::Game::Game(): _world()
{}

bsh::Game::~Game()
= default;

void bsh::Game::init(b2World *world)
{
    this->_world = world;
    this->setSize(oxygine::getStage()->getSize());
    
    // Background
    oxygine::spSprite background = new oxygine::Sprite;
//    background->setResAnim();
    background->attachTo(this);
    
    
    // UI
    this->_ui = new oxygine::Actor;
    this->_ui->attachTo(this);
    this->_ui->setPriority(1);
    // Attach stuff to the UI here
    
}

void bsh::Game::loadLevel(std::string level)
{
    // Remove old level
    b2Body *body = this->_world->GetBodyList();
    while (body)
    {
        this->_world->DestroyBody(body);
        body = body->GetNext();
    }
    
    b2BodyDef* groundBodyDef;
    b2Body* groundBody;
    oxygine::Box9Sprite* spGroundBox;
    
    // Begin reading level file
    oxygine::file::buffer buffer;
    oxygine::file::read("levels.xml", buffer);
    
    pugi::xml_document document;
    document.load_buffer(&buffer.data[0], buffer.size());
    
    // todo: allow loading of different levels based on name
    // Player initialization
    for (const pugi::xpath_node player : document.select_node("bodies").node()
                                            .select_node("player").node()
                                            .select_nodes("sprite"))
    {
        oxygine::log::messageln(player.node().name());
    
        // Settings for the terrain
        b2Vec2 playerPosition = b2Vec2(0.0f, 0.0f);
        b2Vec2 playerSize = b2Vec2(0.0f, 0.0f);
        oxygine::ResAnim *resAnim;
    
        for (auto attributes : player.node().attributes())
        {
            // Iterate over all attributes terrain
            oxygine::log::messageln(attributes.name());
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
    
        this->_player = new Player(*this->_world, body, resAnim);
        
        oxygine::getStage()->addChild(this->_player);
    }
    
    
    
    // Iterate over all terrain
    for (const pugi::xpath_node sprite : document.select_node("bodies").node()
                                                 .select_node("terrain").node()
                                                 .select_nodes("sprite"))
    {
        oxygine::log::messageln(sprite.node().name());
        
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
            oxygine::log::messageln(attributes.name());
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
        
        spGroundBox = new oxygine::Box9Sprite();
        spGroundBox->setResAnim(resAnim);
        spGroundBox->setHorizontalMode(stretchModeHorizontal);
        spGroundBox->setVerticalMode(stretchModeVertical);
        
        spGroundBox->setPosition(bsh::convert(groundPosition));
        spGroundBox->setSize(bsh::convert(groundSize));
        oxygine::getStage()->addChild(spGroundBox);
    }
}

void bsh::Game::doUpdate(const oxygine::UpdateState &us)
{

}
