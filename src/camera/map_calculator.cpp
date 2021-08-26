#include "map_calculator.h"
#include "../def/define.h"


MapCalculator::MapCalculator(std::shared_ptr<CameraParameters> cam_params)
    : _camera_params(cam_params)
{

    initMat();
    calcImageMap(_camera_params, _double_mapx, _double_mapy);
    updateMap();
}


void MapCalculator::initMat()
{
    int image_width = _camera_params->getFrameWidth();
    int image_height = _camera_params->getFrameHeight();

    _double_mapx.create(image_height*2, image_width*2, CV_32FC1);
    _double_mapy.create(image_height*2, image_width*2, CV_32FC1);

#ifdef WITH_CUDA
    _gpu_mapx.create(image_height, image_width, CV_32FC1);
    _gpu_mapy.create(image_height, image_width, CV_32FC1);
#endif
    _cpu_mapx.create(image_height, image_width, CV_32FC1);
    _cpu_mapy.create(image_height, image_width, CV_32FC1);
}


void MapCalculator::calcImageMap(std::shared_ptr<CameraParameters> cam_params, cv::Mat& mapx, cv::Mat& mapy)
{
    using precision = CameraParameters::_precision_;

    // distortion parameters and rectification
    const precision k1 = cam_params->getK1();
    const precision k2 = cam_params->getK2();
    const precision k3 = cam_params->getK3();
    const precision p1 = cam_params->getP1();
    const precision p2 = cam_params->getP2();
    const cv::Mat R = cam_params->getRectifyMat();

    const precision fx = cam_params->getFX();
    const precision fy = cam_params->getFY();
    const precision cx = cam_params->getCX();
    const precision cy = cam_params->getCY();

    // since the size of image is doubled, we double the new cx and cy
    const precision cx_new = 2 * cam_params->getNewCX();
    const precision cy_new = 2 * cam_params->getNewCY();
    const precision fx_new = cam_params->getNewFX();
    const precision fy_new = cam_params->getNewFY();

    int height = mapx.rows;
    int width = mapx.cols;

    // calculate the map, starting from row 1 to end.
    for (int v = 1; v <= height; ++v)
    {
        precision pos[3] = { 0 };
        float *mapx_row = mapx.ptr<float>(v - 1);
        float *mapy_row = mapy.ptr<float>(v - 1);

        for (int u = 1; u <= width; ++u)
        {
            precision uc = (u - cx_new) / fx_new;
            precision vc = (v - cy_new) / fy_new;
            pos[0] = R.at<precision>(0, 0) * uc + R.at<precision>(0, 1) * vc + R.at<precision>(0, 2);
            pos[1] = R.at<precision>(1, 0) * uc + R.at<precision>(1, 1) * vc + R.at<precision>(1, 2);
            pos[2] = R.at<precision>(2, 0) * uc + R.at<precision>(2, 1) * vc + R.at<precision>(2, 2);
            precision xc = pos[0] / pos[2];
            precision yc = pos[1] / pos[2];

            precision r2 = xc * xc + yc * yc;
            precision _2xy = 2 * xc * yc;
            precision kr = 1 + r2 * (k1 + r2 * (k2 + r2 * k3));
            precision ud = xc * kr + p1 * _2xy + p2 * (r2 + 2 * xc * xc);
            precision vd = yc * kr + p2 * _2xy + p1 * (r2 + 2 * yc * yc);
            mapx_row[u - 1] = static_cast<float>(fx * ud + cx);
            mapy_row[u - 1] = static_cast<float>(fy * vd + cy);
        }
    }
}

void MapCalculator::updateMap()
{
    cv::Rect roi = _camera_params->getROI();

    _cpu_mapx = _double_mapx(roi).clone();
    _cpu_mapy = _double_mapy(roi).clone();
#ifdef WITH_CUDA
    _gpu_mapx.upload(_cpu_mapx);
    _gpu_mapy.upload(_cpu_mapy);
#endif
}
