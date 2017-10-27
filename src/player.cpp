//
// Created by Jacob on 10/10/17.
//

#include "core/log.h"
#include <iostream>
#include "../inc/player.h"
#include "../Box2D/Box2D.h"

bsh::Player::Player(b2World &world, b2Body *body) : _world(world)
{
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
    
    b2Vec2 velocity = this->velocity;
    
    bool moving = false;
    
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
    if (data[SDL_SCANCODE_W])
    {
        moving = true;
        velocity.y -= speed;
    }
    if (data[SDL_SCANCODE_S])
    {
        moving = true;
        velocity.y += speed;
    }
    if (data[SDL_SCANCODE_SPACE])
    {
        moving = true;
        b2Vec2 vel = _body->GetLinearVelocity();
        vel.y = -10; //upwards - don't change x velocity
        _body->SetLinearVelocity(vel);
    }
    if (!moving)
    {
        this->_moveState = STOP;
    }
    
    b2Vec2 vel = this->_body->GetLinearVelocity();
    float force = 0;
    switch (_moveState)
    {
        case LEFT:
            if (vel.x > -5)
                force = -100;
            break;

        case STOP:
            force = vel.x * -20;
            break;
            
        case RIGHT:
            if (vel.x < 5)
                force = 100;
            break;
    }
    this->_body->ApplyForce(b2Vec2(force, 0), _body->GetWorldCenter(), true);
    
    // Old way
//    this->body->ApplyForceToCenter(velocity, true);
    
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

std::vector< b2Vec2 > bsh::Player::getData()
{
    std::vector< b2Vec2 > vector = std::vector< b2Vec2 >();
    vector.push_back(this->_body->GetLinearVelocity());
    vector.push_back(this->_body->GetPosition());
    vector.push_back(convert(this->_sprite->getPosition()));
    return vector;
}
