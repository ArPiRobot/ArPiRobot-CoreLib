package arpirobot.core.action;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

import com.sun.jna.Pointer;

public class ActionSeries extends DestroyableObject {

    public ActionSeries(Action[] actions, Action finishedAction){
        verifyNotDestroyed();
        Pointer[] actionsInternal = new Pointer[actions.length];
        for(int i = 0; i < actions.length; ++i){
            actionsInternal[i] = actions[i]._ptr();
        }
        ptr = Bridge.arpirobot.ActionSeries_create(Bridge.ptrArrayToPtr(actionsInternal), actionsInternal.length, 
                finishedAction._ptr());
    }

    @Override
    protected void doDestroy() {
        verifyNotDestroyed();
        Bridge.arpirobot.ActionSeries_destroy(ptr);
    }
    
}
