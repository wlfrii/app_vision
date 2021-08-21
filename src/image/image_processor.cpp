#include "image_processor.h"
#include "../camera/camera_parameters.h"
#include "../camera/map_calculator.h"
#include "../def/ptr_define.h"
#include "../def/cmd.h"
#include "../def/define.h"
#include <memory>
#ifdef WITH_CUDA
#include <libvisiongpu/gpu_algorithm_pipeline_manager.h>
#endif


ImageProcessor::ImageProcessor()
{
    for(auto i = 0; i < vision::MAX_CAMERA_NUMBER; i++){
        props[i] = std::make_shared<Prop>(nullptr);
    }
}

ImageProcessor::~ImageProcessor()
{
}

ImageProcessor* ImageProcessor::getInstance()
{
    static ImageProcessor processor;
    return &processor;
}

void ImageProcessor::setMapCalculator(std::shared_ptr<MapCalculator> map_calculator, uint8_t cam_id)
{
    if(int(cam_id) < vision::MAX_CAMERA_NUMBER){
        props[cam_id].reset();
        props[cam_id] = std::make_shared<Prop>(map_calculator);
    }
}

bool ImageProcessor::processImage(const cv::Mat &input, cv::Mat &output, uint8_t cam_id/* = vision::LEFT_CAMERA*/)
{
    if (props[cam_id]->processed_image.empty())
        props[cam_id]->processed_image.create(input.size(), CV_8UC4);

#ifdef WITH_CUDA
    processImageOnGPU(input, cam_id);
#else
    processImageOnCPU(input, cam_id);
#endif

    if(!downloadImage(output, cam_id))
        return false;

    return true;
}

void ImageProcessor::processImageOnCPU(const cv::Mat& image, uint8_t cam_id)
{
    if (props[cam_id]->map_calculator == nullptr || !CMD::is_rectify) {
		cv::cvtColor(image, props[cam_id]->processed_image, cv::COLOR_BGR2BGRA);
    }
    else {
        cv::Mat temp;
        const auto& mapx = props[cam_id]->map_calculator->getCPUMapx();
        const auto& mapy = props[cam_id]->map_calculator->getCPUMapy();
        cv::remap(image, temp, mapx, mapy, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
        cv::cvtColor(temp, props[cam_id]->processed_image, cv::COLOR_BGR2BGRA);
    }
	// Put image in buffer
    if (!props[cam_id]->processed_image.empty()){
		props[cam_id]->image_buffer.insert(props[cam_id]->processed_image);
    }
}

#ifdef WITH_CUDA
bool ImageProcessor::processImageOnGPU(const cv::Mat& image, uint8_t cam_id)
{
	if (!uploadImage(image, cam_id)) {
		printf("%s: %s: %d, cannot upload the image to GPU.", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool ImageProcessor::uploadImage(const cv::Mat &image, uint8_t cam_id)
{
    /** atomic::load
     * Returns the contained value.
     * typedef enum memory_order {
     *     memory_order_relaxed,   //
     *     memory_order_consume,   // consume
     *     memory_order_acquire,   // acquire
     *     memory_order_release,   // release
     *     memory_order_acq_rel,   // acquire/release
     *     memory_order_seq_cst    // sequentially consistent
     * } memory_order;*/
    bool ret = props[cam_id]->read_flag.load(std::memory_order_relaxed);
    if (ret && !props[cam_id]->image_buffer.isFull())
    {
        if (!props[cam_id]->processed_image.empty()){
            props[cam_id]->image_buffer.insert(props[cam_id]->processed_image);
        }
        /* atomic::store
         * Replaces the contained value with new value. */
        props[cam_id]->read_flag.store(false, std::memory_order_relaxed);
#ifdef WITH_CUDA
        ret = gpu::AlgoPipelineManager::getInstance()->process(image, props[cam_id]->processed_image, props[cam_id]->read_flag, gpu::TreeType(cam_id));
#endif
    }
    return ret;
}
#endif

bool ImageProcessor::downloadImage(cv::Mat &image, uint8_t cam_id)
{
    if (props[cam_id]->image_buffer.isEmpty())
        return false;
    props[cam_id]->image_buffer.remove(image);

    return true;
}





