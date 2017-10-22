//
// Created by Jacob on 10/19/17.
//

#include "../inc/scene.h"

bsh::Scene::Scene()
{
    this->_view = new oxygine::Actor;
    this->_view->setSize(oxygine::getStage()->getSize());
}

void bsh::Scene::show()
{
    oxygine::getStage()->addChild(this->_view);
    
    // Fade in
    this->_view->setAlpha(0);
    this->_view->addTween(oxygine::Actor::TweenAlpha(255), 300);
    
    // Call overloaded method in inherited classes
    _show();
}

void bsh::Scene::hide()
{
    oxygine::spTween tween = this->_view->addTween(oxygine::Actor::TweenAlpha(0), 300);
    
    tween->detachWhenDone();
    
    
    // todo: test this function
    tween->addDoneCallback(CLOSURE(this, &bsh::Scene::_hidden));
}

void bsh::Scene::changeScene(spScene next)
{
    this->hide();
    next->show();
}

oxygine::spActor bsh::Scene::getView() const
{
    return this->_view;
}

void bsh::Scene::_hidden(oxygine::Event *ev)
{
    HiddenEvent hiddenEvent(this);
    dispatchEvent(&hiddenEvent);
}
