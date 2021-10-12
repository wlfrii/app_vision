#include "../display/displayer.h"
#include <opencv2/opencv.hpp>
#include <cstdio>

int main()
{
    std::string path = "../data/WechatIMG13.png";
    std::string path2 = "../data/WechatIMG14.png";
    cv::Mat image = cv::imread(path);
    cv::Mat image2 = cv::imread(path2);
    printf("Image: w %d,h %d,c %d, continuous %d.\n", image.cols, image.rows, image.channels(), image.isContinuous());


    gpu::Displayer displayer_3d(960, 540);
    displayer_3d.initialize(); 
    cv::Mat tmp;
    cv::cvtColor(image,tmp,cv::COLOR_BGR2RGB);
    displayer_3d.updateImage(tmp.ptr<uchar>(0), image.cols, image.rows, image.channels(), false);
    cv::cvtColor(image2,tmp,cv::COLOR_BGR2RGB);
    displayer_3d.updateImage(tmp.ptr<uchar>(0), image.cols, image.rows, image.channels(), true);
    
    gpu::Displayer displayer_2d(960, 540, gpu::Displayer::DISPLAY_2D);
    displayer_2d.initialize(); 
    displayer_2d.updateImage(tmp.ptr<uchar>(0), image.cols, image.rows, image.channels());

    while(!displayer_3d.shouldClose() && !displayer_2d.shouldClose())
    {
        displayer_3d.render();
        displayer_2d.render();
    }

    return 0;
}