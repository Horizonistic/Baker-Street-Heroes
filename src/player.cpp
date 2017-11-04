//
// Created by Jacob on 10/10/17.
//

#include "core/log.h"
#include <iostream>
#include <resources.h>
#include "../inc/player.h"
#include "../Box2D/Box2D.h"

bsh::Player::Player(b2World &world, b2Body *body, oxygine::ResAnim *resAnim) :
        _world(world),
        _canJump(false)
{
    this->setEntityType(oxygine::entityType::PLAYER);
    this->_body = body;
    this->_sprite = new oxygine::Sprite();
    
    this->_sprite->setResAnim(resAnim);
    this->_sprite->attachTo(this);
//    this->_sprite->addTween(oxygine::TweenAnim(resAnim), 1000, -1);
    this->_sprite->setScale(0.005f * bsh::constant::SCALE);
    
    this->setPosition(bsh::convert(this->_body->GetPosition()));
    
    this->_body->SetUserData(this);
    
    /*
     * Needed for expiriment with attack hitboxes
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(5, 5);
    bodyDef.fixedRotation = true;
    b2Body *body2 = this->_world.CreateBody(&bodyDef);
    
    // Creating polygon
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(4 / 2, 4 / 2, {4 / 2, 4 / 2}, 0);
    
    // Create body's fixture (figure out more of what that means)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.0f;
    
    body2->CreateFixture(&fixtureDef);
    this->expiriment = body2;
     */

    // When setting sprite position, it's based off of what it's attached to
    // In this case, it's attached to this->, an actor
//    this->sprite->setPosition(game::convert(position));
}

bsh::Player::~Player()
{
    this->_resources.free();
}

void bsh::Player::doUpdate(const oxygine::UpdateState &us)
{

    /*
     * Experimental stuff for attack hitboxes
    int a = 5;
    int b = 5;
    int r = 2;

     (std::pow(x - b, 2) + std::pow(y - a, 2))
    for (int x = 0; x < 10; x++)
    {
         oxygine::log::messageln(std::to_string(std::pow(x - b, 2) + std::pow(y - a, 2)).c_str()); //= pow(r, 2);
    }
     */

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
//        moving = true;
        vel.y = -constant::PLAYER_JUMP_FORCE; //upwards - don't change x velocity
//        _body->SetLinearVelocity(vel);
    }
    if (!moving)
    {
        this->_moveState = STOP;
    }
    
    float force = 0;
    oxygine::ResAnim *resAnim;
    switch (_moveState)
    {
        // If moving left
        case LEFT:
            this->_sprite->setFlippedX(true); // Facing to the left
            
            resAnim = bsh::Res::characters.getResAnim("playerRunning1x1");
            if (strcasecmp(this->_sprite->getResAnim()->getName().c_str(), resAnim->getName().c_str()) != 0)
            {
                this->_sprite->setResAnim(resAnim);
                this->_sprite->removeTweens();
                this->_sprite->addTween(oxygine::TweenAnim(resAnim), constant::PLAYER_ANIM_SPEED, -1);
            }
            
            if (vel.x >= -constant::MAX_PLAYER_X_VEL)
            {
                force = -(constant::MAX_PLAYER_X_VEL * constant::PLAYER_FORCE_MULTIPLIER);
            }
            break;

            // If not left or right
        case STOP:
            resAnim = bsh::Res::characters.getResAnim("playerStanding1x1");
            this->_sprite->setResAnim(resAnim);
            this->_sprite->removeTweens();
            this->_sprite->addTween(oxygine::TweenAnim(resAnim), constant::PLAYER_ANIM_SPEED, -1);
        
            if (strcasecmp(this->_sprite->getResAnim()->getName().c_str(), resAnim->getName().c_str()) != 0)
            {
                this->_sprite->setResAnim(resAnim);
            }
            
            vel.x = 0;
            break;
            
            // If holding right
        case RIGHT:
            this->_sprite->setFlippedX(false); // Facing to the right
        
            resAnim = bsh::Res::characters.getResAnim("playerRunning1x1");
            if (strcasecmp(this->_sprite->getResAnim()->getName().c_str(), resAnim->getName().c_str()) != 0)
            {
                this->_sprite->setResAnim(resAnim);
                this->_sprite->removeTweens();
                this->_sprite->addTween(oxygine::TweenAnim(resAnim), constant::PLAYER_ANIM_SPEED, -1);
            }
            
            if (vel.x <= constant::MAX_PLAYER_X_VEL)
            {
                force = constant::MAX_PLAYER_X_VEL * constant::PLAYER_FORCE_MULTIPLIER;
            }
            break;
    }
//    this->_body->SetLinearVelocity(vel);
    if (force)
    {
        this->_body->ApplyForce(b2Vec2(force, 0), _body->GetWorldCenter(), true);
    }
    
    //  Make sure player isn't moving over maximum speed
    if (vel.x <= -constant::MAX_PLAYER_X_VEL)
    {
        vel.x = -constant::MAX_PLAYER_X_VEL + 0.0001f;
    }
    else if (vel.x >= constant::MAX_PLAYER_X_VEL)
    {
        vel.x = constant::MAX_PLAYER_X_VEL - 0.0001f;
    }
    this->_body->SetLinearVelocity(vel);
    
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
