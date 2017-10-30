//
// Created by Jacob on 10/10/17.
//

#include "core/log.h"
#include <iostream>
#include "../inc/player.h"
#include "../Box2D/Box2D.h"

bsh::Player::Player(b2World &world, b2Body *body) : _world(world), _canJump(true)
{
    this->setEntityType(oxygine::entityType::PLAYER);
    this->_body = body;
    this->_sprite = new oxygine::Sprite();
    
    _resources.loadXML("characters.xml");
    oxygine::ResAnim *player = _resources.getResAnim("defaultPlayerAnim");
    this->_sprite->setResAnim(player);
    this->_sprite->attachTo(this);
    this->_sprite->addTween(oxygine::TweenAnim(player), 1000, -1);
    this->_sprite->setScale(0.01f * bsh::constant::SCALE);
    
    b2Vec2 position = this->_body->GetPosition();
    this->setPosition(bsh::convert(position));
    
    this->_body->SetUserData(this);
//    this->_body->SetUserData((void*) constant::_entityType::PLAYER);
    // When setting sprite position, it's based off of what it's attached to
    // In this case, it's attached to this->, an actor
//    this->sprite->setPosition(game::convert(position));
    
    // Creating a dynamic body
    // Creating the definition, setting position and type
    
}

bsh::Player::~Player()
{
    this->_resources.free();
}

void bsh::Player::doUpdate(const oxygine::UpdateState &us)
{
    float speed = 1000.0f * (us.dt / 1000.0f);
    const Uint8* data = SDL_GetKeyboardState(0);
    
    
    b2Vec2 vel = this->_body->GetLinearVelocity();
    bool moving = false;
    
    // Determine which keys are pressed and set move state accordingly
    if (data[SDL_SCANCODE_A])
    {
        moving = true;
        this->_moveState = LEFT;
    }
    if (data[SDL_SCANCODE_D])
    {
        moving = true;
        this->_moveState = RIGHT;
    }
    if (data[SDL_SCANCODE_SPACE] && this->_canJump)
    {
        moving = true;
        vel.y = -10; //upwards - don't change x velocity
//        _body->SetLinearVelocity(vel);
    }
    if (!moving)
    {
        this->_moveState = STOP;
    }
    
    float force = 0;
    switch (_moveState)
    {
        case LEFT:
            if (vel.x > -5)
                vel.x = -5;
            break;

        case STOP:
            force = vel.x * -100;
            vel.x = 0;
            break;
            
        case RIGHT:
            if (vel.x < 5)
                vel.x = 5;
            break;
    }
    this->_body->SetLinearVelocity(vel);
//    this->_body->ApplyForce(b2Vec2(force, 0), _body->GetWorldCenter(), true);
    
    // Get position of body after applying force
    b2Vec2 position = this->_body->GetPosition();
    
    // Apply physics to the actor
    this->setPosition(convert(position).x, convert(position).y);
    this->setRotation(this->_body->GetAngle());
    
    // Apply physics to the sprite
    // Actually don't do that, see comment in constructor
//    this->sprite->setPosition(this->getPosition());
//    this->sprite->setRotation(this->body->GetAngle());
}

void bsh::Player::beginTouchingGround()
{
    oxygine::log::messageln("Begin touching ground");
    this->_canJump = true;
}

void bsh::Player::endTouchingGround()
{
    oxygine::log::messageln("End touching ground");
    this->_canJump = false;
}

std::vector< b2Vec2 > bsh::Player::getData()
{
    std::vector< b2Vec2 > vector = std::vector< b2Vec2 >();
    vector.push_back(this->_body->GetLinearVelocity());
    vector.push_back(this->_body->GetPosition());
    vector.push_back(convert(this->_sprite->getPosition()));
    return vector;
}
