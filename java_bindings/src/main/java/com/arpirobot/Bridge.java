package com.arpirobot;

import java.io.File;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

import com.arpirobot.core.BaseRobot;

public class Bridge {

    static {
        try {
            URL url = Bridge.class.getResource("/libarpirobotcorejni.so");
            File tempDir = Files.createTempDirectory("nativelib-extract").toFile();
            tempDir.deleteOnExit();
            File tempNativeLibFile = new File(tempDir, "libarpirobotcorejni.so");
            tempNativeLibFile.deleteOnExit();
            InputStream in = url.openStream();
            Files.copy(in, tempNativeLibFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
            System.load(tempNativeLibFile.getAbsolutePath());
        } catch (Exception e) {
            throw new RuntimeException("Unable to load JNI bridge library.");
        }
        
    }

    ////////////////////////////////////////////////////////////////////////////
    /// BaseRobot Bridge
    ////////////////////////////////////////////////////////////////////////////
    static native long BaseRobot_create(BaseRobot javaRobotObj);
    static native void BaseRobot_destroy(long ptr);
    static native void BaseRobot_start(long ptr);
    static native void BaseRobot_feedWatchdog(long ptr);
}
