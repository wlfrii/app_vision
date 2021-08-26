#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <string>
#include "camera/camera_parameters.h"
#include "camera/map_calculator.h"

TEST_CASE("Testing camera parameters load function", "[cam params]")
{
    std::string path = "../data/cam_params_no56.sr";
    auto reader = CameraParamsReaderFactory::getInstance()->create(path);
    REQUIRE(reader);
    CHECK(reader->getFrameHeight() == 1080);
    CHECK(reader->getFrameWidth() == 1920);
    CHECK(reader->getCamFOV() == Approx(87.1586).margin(1e-3));
    CHECK(reader->getCamDistance() == Approx(3.9812).margin(1e-3));

    // LEFT CAMERA
    auto cam_params = reader->getStereoCameraParameters()->left;
    CHECK(cam_params->getK1() == Approx(0.08983414).margin(1e-7));
    CHECK(cam_params->getK2() == Approx(-0.07496931).margin(1e-7));
    CHECK(cam_params->getK3() == Approx(0.).margin(1e-7));
    CHECK(cam_params->getP1() == Approx(-0.00026013).margin(1e-7));
    CHECK(cam_params->getP2() == Approx(0.00049181).margin(1e-7));
    CHECK(cam_params->getFX() == Approx(1070.62299005).margin(1e-7));
    CHECK(cam_params->getFY() == Approx(1074.60243246).margin(1e-7));
    CHECK(cam_params->getCX() == Approx(964.75522298).margin(1e-7));
    CHECK(cam_params->getCY() == Approx(549.37214405).margin(1e-7));
    CHECK(cam_params->getNewCX() == Approx(960.).margin(1e-7));
    CHECK(cam_params->getNewCY() == Approx(540.).margin(1e-7));
    CHECK(cam_params->getNewFX() == Approx(1157.47646506).margin(1e-7));
    CHECK(cam_params->getNewFY() == Approx(1157.47646506).margin(1e-7));

    auto R = cam_params->getRectifyMat();
    CHECK(R.at<double>(0,0) == Approx(0.99977731).margin(1e-7));
    CHECK(R.at<double>(0,1) == Approx(-0.01118443).margin(1e-7));
    CHECK(R.at<double>(0,2) == Approx(0.01789140).margin(1e-7));
    CHECK(R.at<double>(1,0) == Approx(0.01118443).margin(1e-7));
    CHECK(R.at<double>(1,1) == Approx(0.99977731).margin(1e-7));
    CHECK(R.at<double>(1,2) == Approx(0.00020015).margin(1e-7));
    CHECK(R.at<double>(2,0) == Approx(-0.01789538).margin(1e-7));
    CHECK(R.at<double>(2,1) == Approx(0.).margin(1e-7));
    CHECK(R.at<double>(2,2) == Approx(0.99967976).margin(1e-7));

    auto roi = cam_params->getROI();
    CHECK(roi.x == 960);
    CHECK(roi.y == 540);
    CHECK(roi.width == 1920);
    CHECK(roi.height == 1080);
}

TEST_CASE("Testing mapx and mapy for rectification", "[mapcalculator]")
{
    std::string path = "../data/cam_params_no56.sr";
    auto reader = CameraParamsReaderFactory::getInstance()->create(path);
    auto cam_params = reader->getStereoCameraParameters();

    auto map_calculater = std::make_shared<MapCalculator>(cam_params->left, reader->getFrameWidth(), reader->getFrameHeight());

    auto mapx = map_calculater->getCPUMapx();
    auto mapy = map_calculater->getCPUMapy();

    REQUIRE(mapx.cols == reader->getFrameWidth());
    REQUIRE(mapx.rows == reader->getFrameHeight());
    REQUIRE(mapy.cols == reader->getFrameWidth());
    REQUIRE(mapy.rows == reader->getFrameHeight());

    CHECK(mapx.at<float>(0,0) == Approx(96.979715).margin(1e-5));
    CHECK(mapy.at<float>(0,0) == Approx(35.479649).margin(1e-5));
    CHECK(mapx.at<float>(100,100) == Approx(184.348933).margin(1e-5));
    CHECK(mapy.at<float>(100,100) == Approx(127.565260).margin(1e-5));
    CHECK(mapx.at<float>(500,500) == Approx(558.254077).margin(1e-5));
    CHECK(mapy.at<float>(500,500) == Approx(508.413533).margin(1e-5));
    CHECK(mapx.at<float>(800,1000) == Approx(1019.452769).margin(1e-5));
    CHECK(mapy.at<float>(800,1000) == Approx(793.593968).margin(1e-5));
    CHECK(mapx.at<float>(500,1440) == Approx(1439.837824).margin(1e-5));
    CHECK(mapy.at<float>(500,1440) == Approx(517.620123).margin(1e-5));
    CHECK(mapx.at<float>(300,1720) == Approx(1719.168547).margin(1e-5));
    CHECK(mapy.at<float>(300,1720) == Approx(327.029171).margin(1e-5));
    CHECK(mapx.at<float>(1079,1919) == Approx(1896.497215).margin(1e-5));
    CHECK(mapy.at<float>(1079,1919) == Approx(1077.475257).margin(1e-5));
}
