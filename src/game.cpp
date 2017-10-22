//
// Created by Jacob on 10/19/17.
//

#include "../inc/game.h"
#include "../inc/player.h"

bsh::Game::Game()
= default;

bsh::Game::~Game()
= default;

void bsh::Game::init(b2World &world)
{
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
    
    
    // Player initialization
    this->_player = new Player(world);
}

void bsh::Game::doUpdate(const oxygine::UpdateState &us)
{

}
