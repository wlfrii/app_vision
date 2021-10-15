#include "../display/displayer.h"
#include <opencv2/opencv.hpp>
#include <cstdio>

int main()
{
    std::string l_path = "./L_test.bmp";
    std::string r_path = "./R_test.bmp";
    cv::Mat l_image = cv::imread(l_path);
    cv::Mat r_image = cv::imread(r_path);
    printf("Image: w %d,h %d,c %d, continuous %d.\n", l_image.cols, l_image.rows, l_image.channels(), l_image.isContinuous());

    gpu::Displayer displayer_3d(960, 540);
    displayer_3d.initialize(); 
    cv::Mat tmp;
    cv::cvtColor(l_image,tmp,cv::COLOR_BGR2RGB);
    displayer_3d.updateImage(tmp.ptr<uchar>(0), tmp.cols, tmp.rows, tmp.channels(), false);
    cv::cvtColor(r_image,tmp,cv::COLOR_BGR2RGB);
    displayer_3d.updateImage(tmp.ptr<uchar>(0), tmp.cols, tmp.rows, tmp.channels(), true);
    
    gpu::Displayer displayer_2d(960, 540, gpu::Displayer::DISPLAY_2D);
    displayer_2d.initialize(); 
    displayer_2d.updateImage(tmp.ptr<uchar>(0), tmp.cols, tmp.rows, tmp.channels());

    while(!displayer_3d.shouldClose() && !displayer_2d.shouldClose())
    {
        displayer_3d.render();
        displayer_2d.render();
    }

    return 0;
}
