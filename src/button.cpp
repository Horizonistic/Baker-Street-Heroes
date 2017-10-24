//
// Created by Jacob on 10/23/17.
//

#include "../inc/button.h"
#include "../inc/resources.h"

bsh::Button::Button()
{
    this->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &Button::onEvent));
    this->addEventListener(oxygine::TouchEvent::TOUCH_UP, CLOSURE(this, &Button::onEvent));
    this->addEventListener(oxygine::TouchEvent::CLICK, CLOSURE(this, &Button::onEvent));
}

void bsh::Button::setText(const std::string &text)
{
    if (!this->_text)
    {
        oxygine::TextStyle style;
        style.font = bsh::Res::ui.getResFont("main");
        style.vAlign = oxygine::TextStyle::VALIGN_MIDDLE;
        style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
        
        this->_text = oxygine::initActor(new oxygine::TextField,
                                         oxygine::arg_style = style,
                                         oxygine::arg_size = this->getSize(),
                                         oxygine::arg_attachTo = this);
    }
    
    this->_text->setText(text);
}

void bsh::Button::onEvent(oxygine::Event *ev)
{
    oxygine::TouchEvent *event = static_cast< oxygine::TouchEvent* >(ev);
    
    if (ev->type == oxygine::TouchEvent::TOUCH_DOWN)
        this->setColor(oxygine::Color::Red);
    
    if (ev->type == oxygine::TouchEvent::TOUCH_UP)
        this->setColor(oxygine::Color::White);
    
    if (ev->type == oxygine::TouchEvent::CLICK)
    {
        this->setScale(1.0f);
        this->addTween(oxygine::Actor::TweenScale(1.1f), 300, 1, true);
    }
}
