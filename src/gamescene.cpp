//
// Created by Jacob on 10/19/17.
//

#include "../inc/gamescene.h"

spGameScene bsh::GameScene::instance;

bsh::GameScene::GameScene(b2World &world): _world(world)
{
    this->_game = new Game();
    this->_game->init(world);
    this->_game->attachTo(_view);
    this->_game->setClock(new oxygine::Clock);
    // todo: create new game instance
    
    // todo: create new button for menu
}

void bsh::GameScene::onEvent(oxygine::Event *ev)
{

}
