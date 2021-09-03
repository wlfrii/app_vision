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
#include <lib_instrument/lib_instrument.h>
#include <vector>
#include <Eigen/Dense>


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

        testProcess(temp, cam_id);

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

void ImageProcessor::testProcess(cv::Mat &image, uint8_t cam_id)
{
    // --- 2021.8.25 Testing ---
    if(props[cam_id]->map_calculator){
        //is_test = false;
//        printf("do testing\n");

        Instrument instrument;
        instrument.setInstrumentType(INSTRUMENT_TYPE_LAPAROSCOPE);
        std::vector<Eigen::Vector3f> ws_clouds = instrument.getWSClouds();

        auto cam_params = props[cam_id]->map_calculator->getCamParams();
        auto A = cam_params->getIntrinsic();

        Eigen::Vector3f cam_pt;
        double t = 3.9812;

        for(auto& pt:ws_clouds){
            int k = cam_id == 0 ? 1 : -1;
            cam_pt = pt + k * Eigen::Vector3f(t/2, 0, 0);
            cam_pt /= cam_pt[2];

            int u = static_cast<int>(A.at<double>(0,0)*cam_pt[0] + A.at<double>(0,2));
            int v = static_cast<int>(A.at<double>(1,1)*cam_pt[1] + A.at<double>(1,2));

            if(u >= 0 && u <= 1919 && v >= 0 && v <= 1079){
                int r = 30;
                for(int uu = u-r; uu<=u+r; uu++){
                    if(uu < 0 || uu > 1920){
                        continue;
                    }
                    for(int vv = v-r; vv<v+r; vv++){
                        if(vv < 0 || vv > 1080){
                            continue;
                        }
                        image.at<cv::Vec3b>(v, u)[0] = 255;
                        image.at<cv::Vec3b>(v, u)[1] = 255;
                        image.at<cv::Vec3b>(v, u)[2] = 255;
                    }
                }
            }
        }
    }
    // -------------------------
}





