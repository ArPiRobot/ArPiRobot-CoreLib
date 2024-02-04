#pragma once

/**
 * Format used for reading frames from the camera
 */
enum class InputFormat{
    MJPEG = 0,              // Supported by many cameras (often supporting higher framerates)
    RAW_YUY2 = 1            // Should be supported by most (all?) USB webcams
};
