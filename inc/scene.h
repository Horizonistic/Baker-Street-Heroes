//
// Created by Jacob on 10/19/17.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "oxygine-framework.h"
#include <vector>

namespace bsh
{
    class Scene;
}

DECLARE_SMART(bsh::Scene, spScene);

namespace bsh
{
    class Scene : public oxygine::EventDispatcher
    {
    public:
        //declare own Event type
        //it would be fired when scene would completely hidden
        class HiddenEvent: public oxygine::Event
        {
        public:
            //define unique int ID with makefourcc 'SHid' = SceneHidden
            enum
            {
                EVENT = makefourcc('S', 'H', 'i', 'd')
            };
        
            HiddenEvent(Scene* scene_): oxygine::Event(EVENT), scene(scene_) {};
            Scene* scene;
        };
        
        Scene();
        void show();
        void hide();
        void changeScene(spScene next);
        oxygine::spActor getView() const;
        
    protected:
        virtual void _show() {}
        virtual void _hide() {}
        
        void _hidden(oxygine::Event* ev);
        oxygine::spActor _view;
    };
}
#endif //GAME_SCENE_H
