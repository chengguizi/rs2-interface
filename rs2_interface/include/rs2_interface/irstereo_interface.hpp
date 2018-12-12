// ROS Node for Realsense D415 Streams
// Cheng Huimin, June 2018
//
// Class of the physical sensor for callback operations

// https://github.com/IntelRealSense/librealsense/wiki/Projection-in-RealSense-SDK-2.0
// - Left and right infrared images are rectified by default (Y16 format is not)

#ifndef IRSTEREO_INTERFACE_HPP
#define IRSTEREO_INTERFACE_HPP

#include <librealsense2/rs.hpp>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>

class IrStereoDriver {

public:
    typedef std::function<void(uint64_t, void*,void*, int, int, double, double, uint64_t, uint64_t)> callbackType; 
                        // time since epoch, left & right image data, width, height, hardware time left & right, hardware sequence left & right
    IrStereoDriver(std::string dev_name_str = "RealSense D415", int laser_power = 150);
    ~IrStereoDriver();
    void setOption(rs2_option option, float value);
    float getOption(rs2_option option);
    rs2::option_range getOptionRange(rs2_option option);
    void startPipe(int width, int height, int hz);
    rs2_intrinsics get_intrinsics() const; // only call this after pipe started
    rs2_extrinsics get_extrinsics_left_to_right() const;
    float get_baseline() const;
    void stopPipe();
    void registerCallback(callbackType cb);
private:
    void init();
    void process();

    std::string _dev_name_str;
    rs2::pipeline* _pipe;
    rs2::device* _dev;
    rs2::depth_stereo_sensor* _stereo;
    rs2::pipeline_profile* _profile;

    rs2_intrinsics _intrinsics;
    rs2_extrinsics _extrinsics_left_to_right;
    float _baseline;
    int _laser_power; // range 0 - 360, step 30. default 150
    int _w,_h;

    bool _isStreaming;

    std::thread _thread;

    std::vector<callbackType> _cblist;
};


#endif /* IRSTEREO_INTERFACE_HPP */
