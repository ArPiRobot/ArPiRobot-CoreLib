package arpirobot.core.action;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

public class ActionSeries extends DestroyableObject {

    public ActionSeries(Action[] actions, Action finishedAction){
        long[] actionsInternal = new long[actions.length];
        for(int i = 0; i < actions.length; ++i){
            actionsInternal[i] = actions[i]._ptr();
        }
        ptr = Bridge.arpirobot.ActionSeries_create(actionsInternal, actionsInternal.length, finishedAction._ptr());
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.ActionSeries_destroy(ptr);
    }
    
}
