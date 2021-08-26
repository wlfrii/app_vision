/**
 * @file map_calculator.h
 * 
 * @author Longfei Wang (longfei.wang@sjtu.edu.cn)
 * 
 * @brief This class is designed for rectification.
 * 
 *   The input arguments should be an object of class CameraParameters. Then the 
 * map for rectifing the image will be calculated. 
 * Noteablely, the test for this class has been completed.
 *
 * Revise:
 * 2021.8.26. The interface for change the disparity is delete, since the disparity
 * variation will affect the perception in stereo vision, especially the depth
 * perception. And on another hand, the varied disparity will casue the predesigned
 * camera optical changed (after rectfication, the camera optical is correpongding
 * to the center of an image).
 * 
 * @version 2.0
 * 
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MAP_CALCULATOR_H
#define MAP_CALCULATOR_H
#include <opencv2/opencv.hpp>
#include "camera_parameters.h"


/**
 * @brief A class for creating the map for rectifying the image.
 */
class MapCalculator
{
public:
    MapCalculator(std::shared_ptr<CameraParameters> cam_params);

    ~MapCalculator() {}

#ifdef WITH_CUDA
    const cv::cuda::GpuMat& getGPUMapx() const { return _gpu_mapx; }
    const cv::cuda::GpuMat& getGPUMapy() const { return _gpu_mapy; }
#endif
    const cv::Mat& getCPUMapx() const { return _cpu_mapx; }
    const cv::Mat& getCPUMapy() const { return _cpu_mapy; }

    const std::shared_ptr<CameraParameters> getCamParams() const{
        return _camera_params;
    }

protected:
    // Initialize all the Mat
    void initMat();

    // Calculate the image map.
    void calcImageMap(std::shared_ptr<CameraParameters> cam_params, cv::Mat& mapx, cv::Mat& mapy);

    // Update the output map based on ROI
    void updateMap();

protected:
    std::shared_ptr<CameraParameters>  _camera_params;
    cv::Mat					_double_mapx;	//!< the map in x dimension
    cv::Mat					_double_mapy;	//!< the map in y dimension

#ifdef WITH_CUDA
    cv::cuda::GpuMat		_gpu_mapx;		//!< the roi region in 'm_double_mapx'
    cv::cuda::GpuMat		_gpu_mapy;		//!< the roi region in 'm_double_mapy'
#endif
    cv::Mat		_cpu_mapx;			//!< the roi region in 'm_double_mapx'
    cv::Mat		_cpu_mapy;			//!< the roi region in 'm_double_mapy'
};

#endif // MAP_CALCULATOR_H
