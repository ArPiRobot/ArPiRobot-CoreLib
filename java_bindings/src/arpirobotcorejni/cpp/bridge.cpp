#include <com_arpirobot_Bridge.h>

#include <arpirobot/bridge.hpp>

extern "C" {


JNIEXPORT jlong JNICALL Java_com_arpirobot_Bridge_BaseRobot_1create(JNIEnv *env, jclass cls, jobject javaRobotObj){
    jclass objclass = env->GetObjectClass(javaRobotObj);
    jmethodID robotStarted = env->GetMethodID(objclass, "robotStarted", "()V");
}

JNIEXPORT void JNICALL Java_com_arpirobot_Bridge_BaseRobot_1destroy(JNIEnv *env, jclass cls, jlong ptr){

}

JNIEXPORT void JNICALL Java_com_arpirobot_Bridge_BaseRobot_1start(JNIEnv *env, jclass cls, jlong ptr){

}

JNIEXPORT void JNICALL Java_com_arpirobot_Bridge_BaseRobot_1feedWatchdog(JNIEnv *env, jclass cls, jlong ptr){

}


} // extern "C"
