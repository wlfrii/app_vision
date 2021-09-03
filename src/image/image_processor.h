#ifndef IMAGE_PROCESSOR_H_LF
#define IMAGE_PROCESSOR_H_LF
#include <opencv2/opencv.hpp>
#include <atomic>
#include <memory>
//#include <libutility/buffer/ring_buffer.h>
#include "../def/ring_buffer.h"
#include <array>
#include "../camera/map_calculator.h"
#include "../def/define.h"

class ImageProcessor
{
protected:
    ImageProcessor();
    ~ImageProcessor();
public:
    static ImageProcessor* getInstance();

    void setMapCalculator(std::shared_ptr<MapCalculator> map_calculator, uint8_t cam_id = vision::LEFT_CAMERA);

    bool processImage(const cv::Mat &input, cv::Mat &output, uint8_t cam_id = vision::LEFT_CAMERA);

private:
	void processImageOnCPU(const cv::Mat& image, uint8_t cam_id);
#ifdef WITH_CUDA
	bool processImageOnGPU(const cv::Mat& image, uint8_t cam_id);
    bool uploadImage(const cv::Mat & image, uint8_t cam_id);
#endif
    bool downloadImage(cv::Mat &image, uint8_t cam_id);

    void testProcess(cv::Mat& image, uint8_t cam_id);

private:
    struct Prop{
        Prop(std::shared_ptr<MapCalculator> map)
            : map_calculator(map)
            , read_flag(true)
        {}
        std::shared_ptr<MapCalculator> map_calculator;
        cv::Mat             processed_image;
        Ringbuffer<cv::Mat, 2> image_buffer;

        /* Atomic   ---   template<typename T> struct atomic
         * Objects of atomic types contain a value of a particular type(T)
         * The main characteristic of atomic objects is that access to this contained value from
         * different threads cannot cause data races. */
        std::atomic<bool>   read_flag;
    };
    std::array<std::shared_ptr<Prop>, vision::MAX_CAMERA_NUMBER> props;
};

#endif // IMAGE_PROCESSOR_H_LF
