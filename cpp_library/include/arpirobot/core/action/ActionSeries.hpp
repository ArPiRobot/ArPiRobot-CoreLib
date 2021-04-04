#pragma once

#include <arpirobot/core/action/Action.hpp>

#include <vector>

namespace arpirobot{
    /**
     * \class ActionSeries ActionSeries.hpp arpirobot/core/action/ActionSeries.hpp
     * 
     * A special action that will run a sequential set of actions (one at a time)
     */
    class ActionSeries : public Action{
    public:
        /**
         * @param actions A vector of actions to run sequentially
         * @param finishedAction An action to transition to once other actions are complete
         */
        ActionSeries(std::vector<Action*> actions, Action *finishedAction);

    protected:
        void begin();

        void process();

        void finish(bool interrupted);

        bool shouldContinue();
    
    private:
        std::vector<Action*> actions;
        Action *finishedAction;

        int index = 0;
    };

}