//
// Created by Jacob on 10/23/17.
//

#ifndef GAME_BUTTON_H
#define GAME_BUTTON_H

#include "oxygine-framework.h"

namespace bsh
{
    class Button;
}

DECLARE_SMART(bsh::Button, spButton);

namespace bsh
{
    class Button : public oxygine::Sprite
    {
    public:
        Button();
        
        void setText(const std::string &text);
        
    private:
        oxygine::spTextField _text;
        
        void onEvent(oxygine::Event *ev);
    };
}

#endif //GAME_BUTTON_H