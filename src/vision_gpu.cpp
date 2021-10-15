#include "vision_ext.h"
#ifdef WITH_CUDA
#include <libvisiongpu/gpu_algorithm_pipeline_manager.h>
#endif

void initGPUPipeline()
{
#ifdef WITH_CUDA
    // Initialize the AlgoPipelineManager first
    gpu::AlgoPipelineManager::getInstance()->intialize();
    gpu::AlgoPipelineManager::getInstance()->addAlgoNode(new gpu::AlgoNodeNonuniform());
    gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::NonuniformProperty>(2, 0.99));
    gpu::AlgoPipelineManager::getInstance()->addAlgoNode(new gpu::AlgoNodeGamma());
    gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::GammaProperty>(0.005));
    gpu::AlgoPipelineManager::getInstance()->addAlgoNode(new gpu::AlgoNodeImageAdjust());
    gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::ImageAdjustProperty>(50, 60));
#endif
}
