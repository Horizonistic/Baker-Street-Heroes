//
// Created by Jacob on 10/19/17.
//

#include <button.h>
#include <global.h>
#include <gamescene.h>
#include <resources.h>

spGameScene bsh::GameScene::instance;

bsh::GameScene::GameScene(b2World &world): _world(world)
{
    this->_game = new Game();
    this->_game->init(&world);
    this->_game->attachTo(this->_view);
    this->_game->setClock(new oxygine::Clock);
    this->_game->getClock()->pause();
    this->_world.pauseWorld();
    
    spButton btn = oxygine::initActor(new Button,
                                      oxygine::arg_resAnim = bsh::Res::ui.getResAnim("buttons"),
                                      oxygine::arg_anchor = oxygine::Vector2(0.5f, 0.5f),
                                      oxygine::arg_attachTo = _view);
    
    // Align to top right
    btn->setX(_view->getWidth() - btn->getWidth() / 2);
    btn->setY(btn->getHeight() / 2);
    
    //handle click to menu
    btn->addEventListener(oxygine::TouchEvent::CLICK, CLOSURE(this, &GameScene::onEvent));
    btn->setText("Load");
    btn->setName("load");
    
    
}

void bsh::GameScene::onEvent(oxygine::Event *ev)
{
    if (std::string("load") == ev->currentTarget->getName())
    {
        this->_game->loadLevel("test");
        spButton btn2 = oxygine::initActor(new Button,
                                                                          oxygine::arg_resAnim = bsh::Res::ui.getResAnim("buttons"),
                                                                          oxygine::arg_anchor = oxygine::Vector2(0.5f, 0.5f),
                                                                          oxygine::arg_attachTo = _view);
    
        btn2->setX(_view->getWidth() - btn2->getWidth() / 2);
        btn2->setY(btn2->getHeight() / 2);
    
        //handle click to menu
        btn2->addEventListener(oxygine::TouchEvent::CLICK, CLOSURE(this, &GameScene::onEvent));
        btn2->setText("Start");
        btn2->setName("startClock");
    }
    if (std::string("startClock") == ev->currentTarget->getName())
    {
        this->_game->getClock()->resume();
        this->_world.unpauseWorld();
    }
}