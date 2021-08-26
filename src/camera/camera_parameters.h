/**
 * @file camera_parameters.h
 *
 * @author Longfei Wang (longfei.wang@sjtu.edu.cn)
 *
 * @brief This file including several class is designed to handle camera parameters.
 *
 * class CameraParamters --- is designed to store the necessary camera parameters.
 *
 * class StereoCameraParameters --- includeing two 'CameraParamters' objecs is
 *                                  designed for stereo camera.
 *
 * class CameraParamsReader --- is the base class for load camera parameters.
 *          |_ class CameraParamsYMLReader --- is for loading YML file
 *          |_ class CameraParamsCSVReader --- is for loading csv(sr) file
 *
 * class CameraParamsReaderFactory --- is used to create an CameraParamsReader object.
 *
 * @version 2.0
 *
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once
#include <string>
#include <memory>
#include <cstdint>
#include <opencv2/opencv.hpp>


/**
 * @brief Store the parameters for monocular.
 */
class CameraParameters
{
public:
    using _precision_ = double;

    CameraParameters() = default;

    CameraParameters(const cv::Mat &intrinsic, const cv::Mat &distortion,
        const cv::Mat &rectification, const cv::Rect &roi, const cv::Mat &new_intrinsic);
    CameraParameters(const CameraParameters& rhs);
#if LINUX
    CameraParameters(USBCameraParammeters &params);
#endif

    _precision_ getFX() const;	    //!< the focal length in x direction
    _precision_ getFY() const;	    //!< the focal length in y direction
    _precision_ getCX() const;	    //!< the x position of camera optical center in the image
    _precision_ getCY() const;	    //!< the y position of camera optical center in the image
    _precision_ getNewFX() const;	//!< the new focal length in x direction
    _precision_ getNewFY() const;	//!< the new focal length in y direction
    _precision_ getNewCX() const;	//!< the new x position of camera optical center in the image
    _precision_ getNewCY() const;  //!< the new y position of camera optical center in the image
    _precision_ getK1() const;     //!< radial distortion k1
    _precision_ getK2() const;     //!< radial distortion k2
    _precision_ getK3() const;     //!< radial distortion k3
    _precision_ getP1() const;     //!< tangentia distortion p1
    _precision_ getP2() const;     //!< tangentia distortion p2

    const cv::Mat&  getIntrinsic() const;       //!< the intrinsic mat 3x3
    const cv::Mat&  getDistCoeffs() const;      //!< the dist_coeffs mat 1x5
    const cv::Mat&  getRectifyMat() const;      //!< the rectification mat 3x3
    const cv::Mat&  getNewIntrinsic() const;    //!< the new_intrinsic mat 3x3
    const cv::Rect& getROI() const;             //!< the roi in the image map [x,y,width,height]

    uint16_t getFrameWidth() const;
    uint16_t getFrameHeight() const;

    std::string getInfo() const;

private:
    cv::Mat         _A;                      //!< intrinsic parameters 3x3
    cv::Mat         _D;                      //!< distortion parameters 1x5
    cv::Mat         _R;                      //!< rectification parameters 3x3
    cv::Rect        _ROI;                    //!< region of interest [x,y,width,height]
    cv::Mat         _Anew;                   //!< new intrinsic parameters 3x3
};


/**
 * @brief Store the parameters for monocular.
 */
class StereoCameraParameters
{
public:
    StereoCameraParameters(){}
    StereoCameraParameters(std::shared_ptr<CameraParameters> left, std::shared_ptr<CameraParameters> right)
        : left(left), right(right)
    {}

    std::string getInfo() const;

    std::shared_ptr<CameraParameters> left;
    std::shared_ptr<CameraParameters> right;
};


/**
 * @brief The tool for read parameters of monocular or binocular.
 */
class CameraParamsReader
{
public:
    CameraParamsReader() {}
    virtual ~CameraParamsReader() {}

    /** Set camera parameters path, return true means the path is valid. */
    virtual std::shared_ptr<CameraParameters> getCameraParameters(uint8_t) const {
        return nullptr;
    }

    virtual std::shared_ptr<StereoCameraParameters> getStereoCameraParameters() const {
        return nullptr;
    }

    /* these fucntions must be called */
    double   getCamDistance() const { return _cam_distance; }
    double   getCamFOV() const { return _cam_fov; }
protected:
    bool _is_valid_path;

    double _cam_distance;
    double _cam_fov;
};

/**
 * @brief Read camera paramters from YML file.
 */
class CameraParamsYMLReader : public CameraParamsReader
{
public:
    CameraParamsYMLReader(const std::string &cam_params_path);
    ~CameraParamsYMLReader();

    std::shared_ptr<CameraParameters> getCameraParameters(uint8_t index) const override;
    std::shared_ptr<StereoCameraParameters> getStereoCameraParameters() const override;

private:
    /* WARNING!!!, this initialization must be done in the intializer list,
      or there maybe exist a situation that fs is initialized twice */
    cv::FileStorage _fs;
};

/**
 * @brief Read camera paramters from csv(sr) file.
 */
class CameraParamsCSVReader : public CameraParamsReader
{
public:
    CameraParamsCSVReader(const std::string &cam_params_path);
    ~CameraParamsCSVReader() {}

    std::shared_ptr<CameraParameters> getCameraParameters(uint8_t index) const override;
    std::shared_ptr<StereoCameraParameters> getStereoCameraParameters() const override;

private:
    std::shared_ptr<StereoCameraParameters> _stereo_cam_params;
};


/**
 * @brief The CameraParamsReaderFactory class is designed for construct CameraParamsReader
 */
class CameraParamsReaderFactory
{
protected:
    CameraParamsReaderFactory(){}

public:
    static CameraParamsReaderFactory* getInstance();

    std::unique_ptr<CameraParamsReader> create(std::string cam_params_path);
};
