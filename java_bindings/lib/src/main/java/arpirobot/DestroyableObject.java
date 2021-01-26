package arpirobot;

import arpirobot.core.log.Logger;

/**
 * For internal use only.
 * 
 * In java, there is no guaranteed destructor. This could lead to memory leaks since underlying objects
 * are dynamically allocated. As such, a user callable destroy() method is provided. If an object will
 * go out of scope, the user can call this function. If it is not called, finalize may be called by the GC
 * in which case a warning will be printed.
 */
public abstract class DestroyableObject {
    protected long ptr = 0;

    /**
     * USER CODE SHOULD NEVER USE THIS!!!
     */
    public long _ptr(){
        return ptr;
    }

    @Override
    @SuppressWarnings( "deprecation" )
    protected void finalize() throws Throwable {
        if(ptr != 0){
            destroy();
        }
        Logger.logWarning("destroy was not called before object went out of scope.");
    }

    public void destroy(){
        if(ptr != 0){
            doDestroy();
            ptr = 0;
        }
    }

    protected abstract void doDestroy();

    protected void verifyNotDestroyed(){
        if(ptr == 0)
            throw new RuntimeException("Cannot run functions on an object after destroy.");
    }
}
