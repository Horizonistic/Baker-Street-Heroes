//
// Created by Jacob on 10/19/17.
//

#include <global.h>

#include "../inc/unit.h"
#include "../inc/game.h""

bsh::Unit::Unit(): _game(0), _dead(false)
{}

void bsh::Unit::init(const b2Vec2 &pos, Game* game)
{
    this->_game = game;
    this->_view = new oxygine::Actor;
    this->_view->attachTo(game);
    this->_view->setPosition(bsh::convert(pos));
    
    _game->_units.push_back(this);
    
    _init();
}

const b2Vec2 &bsh::Unit::getPositionn() const
{
    return bsh::convert(this->_view->getPosition());
}

void bsh::Unit::update(const oxygine::UpdateState &us)
{
    _update(us);
}
