//
// Created by Jacob on 11/2/17.
//

#include <menuscene.h>
#include <button.h>
#include <resources.h>
#include <gamescene.h>

spMenuScene bsh::MenuScene::instance;

bsh::MenuScene::MenuScene(b2World &world): _world(world)
{
    // todo: actual main menu, with options to select character, settings, and levels
    // Have character selection under level selection
    spButton btn = oxygine::initActor(new Button,
                                      oxygine::arg_resAnim = bsh::Res::ui.getResAnim("buttons"),
                                      oxygine::arg_anchor = oxygine::Vector2(0.5f, 0.5f),
                                      oxygine::arg_attachTo = _view);
    
    // Align to top right
    btn->setX(_view->getWidth() / 2);
    btn->setY(_view->getHeight()/ 2);
    
    //handle click to menu
    btn->addEventListener(oxygine::TouchEvent::CLICK, CLOSURE(this, &MenuScene::onEvent));
    btn->setText("Load Level");
    btn->setName("loadLevel");
}

void bsh::MenuScene::onEvent(oxygine::Event *ev)
{
    if (std::string("loadLevel") == ev->currentTarget->getName())
    {
        changeScene(GameScene::instance);
//        this->_game->loadLevel("Test");
    }
}
