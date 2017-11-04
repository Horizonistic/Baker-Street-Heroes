//
// Created by Jacob on 10/19/17.
//

#include <button.h>
#include <global.h>
#include <gamescene.h>
#include <resources.h>

// todo: future: weather effects like rain

spGameScene bsh::GameScene::instance;

bsh::GameScene::GameScene(b2World &world): _world(world)
{
    this->_game = new Game();
    this->_game->init(&world);
    this->_game->attachTo(this->_view);
    this->_game->setClock(new oxygine::Clock);
    this->_game->getClock()->pause();
    this->_world.pauseWorld();
    
    oxygine::file::buffer buffer;
    oxygine::file::read("levels.xml", buffer);
    
    pugi::xml_document document;
    document.load_buffer(&buffer.data[0], buffer.size());
    
    // Setting up level loading menu and buttons from levels.xml
    oxygine::log::messageln("Loading menu buttons");
    int pos = 1;
    for (const pugi::xpath_node level : document.select_node("levels").node()
                                                 .select_nodes("level"))
    {
    
        // Get the size to know where to place the buttons
        int size = (int) document.select_node("levels").node().select_nodes("level").size();
        std::string name;
        std::string file;
        
        for (auto attributes : level.node().attributes())
        {
            if (strcasecmp(attributes.name(), "name") == 0)
            {
                name = attributes.as_string();
            }
            else if (strcasecmp(attributes.name(), "file") == 0)
            {
                file = attributes.as_string();
            }
        }
        spButton btn = oxygine::initActor(new Button,
                                          oxygine::arg_resAnim = bsh::Res::ui.getResAnim("buttons"),
                                          oxygine::arg_anchor = oxygine::Vector2(0.5f, 0.5f),
                                          oxygine::arg_attachTo = _view);
    
        // Align to top right
        btn->setX(_view->getWidth() / 2);
        btn->setY(((btn->getHeight()) * (pos / size)) + (btn->getHeight() / 2));
    
        //handle click to menu
        btn->addEventListener(oxygine::TouchEvent::CLICK, CLOSURE(this, &GameScene::onEvent));
        btn->setText(name);
        btn->setName(file);
//        btn->setUserData(&file);
        
        pos++;
    }
}

void bsh::GameScene::onEvent(oxygine::Event *ev)
{
    // Useful cast to get from void* to std::string*
    // Used for when I was trying to store std::string in the user data of oxygine::Object
    // spButton button = static_cast< spButton >(ev->);
    
    // Get last 4 digits of the name to check if it's an XML file or not
    // If it is then load the corresponding level
    auto str = ev->currentTarget->getName();
    auto suffix = str.substr(str.length() - 4);
    if (strcmp(suffix.c_str(), ".xml") == 0)
    {
        this->_game->loadLevel(str);
        this->_game->getClock()->resume();
        this->_world.unpauseWorld();
    }
}