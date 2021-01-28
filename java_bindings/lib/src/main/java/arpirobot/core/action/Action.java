package arpirobot.core.action;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;
import arpirobot.core.device.BaseDevice;


/**
 * Generic action class. User actions should inherit this class and implement the four pure virtual methods
 */
public abstract class Action extends DestroyableObject {
    private final Bridge.void_void_func_ptr _begin = () -> begin();
    private final Bridge.void_void_func_ptr _process = () -> process();
    private final Bridge.void_bool_func_ptr _finish = (arg) -> finish(arg);
    private final Bridge.bool_void_func_ptr _shouldContinue = () -> shouldContinue();

    public Action(){
        ptr = Bridge.arpirobot.Action_create(_begin, _process, _finish, _shouldContinue);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.Action_destroy(ptr);
    }

    /**
     * Use this action to lock a set of devices. 
     * This is the same as calling Action::lockDevice once for each device individually.
     * @param devices A vector of devices to lock
     */
    public void lockDevices(BaseDevice[] devices){
        verifyNotDestroyed();
        long[] devicesInternal = new long[devices.length];
        for(int i = 0; i < devices.length; ++i){
            devicesInternal[i] = devices[i]._ptr();
        }
        Bridge.arpirobot.Action_lockDevices(ptr, devicesInternal, devicesInternal.length);
    }

    /**
     * Use this action to lock a device. When a device is locked by this action, 
     * any action previously locking it will be stopped.
     * @param device The device to lock
     */
    public void lockDevice(BaseDevice device){
        verifyNotDestroyed();
        Bridge.arpirobot.Action_lockDevice(ptr, device._ptr());
    }

    /**
     * @return true if the action has been started, but has not finished or been stopped.
     */
    public boolean isRunning(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Action_isRunning(ptr);
    }

    /**
     * Run when the action is started.
     */
    protected abstract void begin();

    /**
     * Run periodically after the action is started, but before it finishes.
     */
    protected abstract void process();

    /**
     * Run when the action is stopped.
     * @param wasInterrupted Will be true if the action did not stop on its own (see Action::shouldContinue)
     */
    protected abstract void finish(boolean wasInterrupted);

    /**
     * Called each time after Action::process. When this returns false the action will stop.
     * @return false if the action should stop. Else true.
     */
    protected abstract boolean shouldContinue();
}
