
import sys

# debugpy is not able to handle breakpoints in threads created in the C++ corelib
# Since these threads are where most user code runs, this essentially makes debugging
# python robot programs useless. Thus, we need to be able to debug such threads.
# Fortunately, debugpy has a debug_this_thread() function, however since
# the user code callbacks may run on any thread in the corelib's threadpool,
# we have to call this each time a callback function (that may be on a thread
# other than the main thread) is run. Thus, this function can be used in any
# such callbacks.
def _enable_debugpy_this_thread():
    if not 'debugpy' in sys.modules:
        return
    try:
        import debugpy
        debugpy.debug_this_thread()
    except:
        pass
