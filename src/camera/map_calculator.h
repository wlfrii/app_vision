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


/** @brief Create the map tor rectify the image
  This class is designed as a virtual class
*/
class MapCalculator
{
public:
    MapCalculator(std::shared_ptr<CameraParameters> cam_params, uint16_t image_width, uint16_t image_height);

    ~MapCalculator() {}

    /** @brief Update the map for rectification.
    */
    void updateMap(int disparity);
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
    /** @brief Initialize all the Mat
    */
    void initMat();

    /** @brief Calculate the image map.
      the map include map(x&y) in both 2D and 3D, however 3D map is same as 2D actually.
    */
    void calcImageMap(std::shared_ptr<CameraParameters> cam_params, cv::Mat& mapx, cv::Mat& mapy);

protected:
    std::shared_ptr<CameraParameters>  _camera_params;
    cv::Mat					_double_mapx;	//!< the map in x dimension
    cv::Mat					_double_mapy;	//!< the map in y dimension

    uint16_t    _image_width;
    uint16_t    _image_height;
#ifdef WITH_CUDA
    cv::cuda::GpuMat		_gpu_mapx;		//!< the roi region in 'm_double_mapx'
    cv::cuda::GpuMat		_gpu_mapy;		//!< the roi region in 'm_double_mapy'
#endif
    cv::Mat		_cpu_mapx;			//!< the roi region in 'm_double_mapx'
    cv::Mat		_cpu_mapy;			//!< the roi region in 'm_double_mapy'
};

#endif // MAP_CALCULATOR_H
