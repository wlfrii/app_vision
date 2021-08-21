#include "map_calculator.h"
#include "camera_parameters.h"

MapCalculator::MapCalculator(std::shared_ptr<CameraParameters> cam_params, uint16_t image_width, uint16_t image_height)
    : camera_params(cam_params)
    , image_width(image_width)
    , image_height(image_height)
{
    initMat();
    double_mapx.create(image_height*2, image_width*2, CV_32FC1);
    double_mapy.create(image_height*2, image_width*2, CV_32FC1);
    calcImageMap(camera_params, double_mapx, double_mapy);

    updateMap(0);
}

void MapCalculator::updateMap(int disparity)
{
    cv::Rect roi = camera_params->roi();
    /* Although there we designate as a monocular map, we keep the disparity adjustment */
    roi.x -= disparity;

    cpu_mapx = double_mapx(roi).clone();
    cpu_mapy = double_mapy(roi).clone();
#ifdef WITH_CUDA
    gpu_mapx.upload(cpu_mapx);
    gpu_mapy.upload(cpu_mapy);
#endif
    // suppose single image size is w*h
    //int cols = cpu_mapx.cols;
    //int rows = cpu_mapx.rows;
}


void MapCalculator::initMat()
{
#ifdef WITH_CUDA
    gpu_mapx.create(image_height, image_width, CV_32FC1);
    gpu_mapy.create(image_height, image_width, CV_32FC1);
#endif
    cpu_mapx.create(image_height, image_width, CV_32FC1);
    cpu_mapy.create(image_height, image_width, CV_32FC1);
}


void MapCalculator::calcImageMap(std::shared_ptr<CameraParameters> cam_params, cv::Mat& mapx, cv::Mat& mapy)
{
    // distortion parameters and rectification
    const float k1 = cam_params->getK1();
    const float k2 = cam_params->getK2();
    const float k3 = cam_params->getK3();
    const float p1 = cam_params->getP1();
    const float p2 = cam_params->getP2();
    const cv::Mat R = cam_params->rectifyMat();

    /* in configuration file, cx and cy is the average of cx1/cx2 and cy1/cy2,
     since the size of image is doubled, we double the new cx and cy */
    const float cx_new = 2 * cam_params->getNewCX();
    const float cy_new = 2 * cam_params->getNewCY();

    int height = mapx.rows;
    int width = mapx.cols;

    // calculate the map, starting from row 1 to end.
    for (int y = 0; y < height; ++y)
    {
        float pos[3] = { 0 };
        float *mapx_row = mapx.ptr<float>(y);
        float *mapy_row = mapy.ptr<float>(y);

        for (int x = 0; x < width; ++x)
        {
            float u = (x - cx_new) / cam_params->getNewFX();
            float v = (y - cy_new) / cam_params->getNewFY();
            pos[0] = R.at<float>(0, 0) * u + R.at<float>(0, 1) * v + R.at<float>(0, 2);
            pos[1] = R.at<float>(1, 0) * u + R.at<float>(1, 1) * v + R.at<float>(1, 2);
            pos[2] = R.at<float>(2, 0) * u + R.at<float>(2, 1) * v + R.at<float>(2, 2);
            u = pos[0] / pos[2];
            v = pos[1] / pos[2];

            float r2 = u * u + v * v;
            float _2uv = 2 * u * v;
            float kr = 1 + r2 * (k1 + r2 * (k2 + r2 * k3));
            float ud = u * kr + p1 * _2uv + p2 * (r2 + 2 * u * u);
            float vd = v * kr + p2 * _2uv + p1 * (r2 + 2 * v * v);
            mapx_row[x] = cam_params->getFX() * ud + cam_params->getCX();
            mapy_row[x] = cam_params->getFY() * vd + cam_params->getCY();
        }
    }
}
