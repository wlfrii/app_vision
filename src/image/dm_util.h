/**
 * @file disparity_map.h
 *
 * @author Longfei Wang (longfei.wang@sjtu.edu.cn)
 *
 * @brief This file is designed for achieve Stereo Vision Disparity Map
 * Algorithm.
 *
 * References
 * [1] R. A. Hamzah and H. Ibrahim, “Literature Survey on Stereo Vision
 *     Disparity Map Algorithms,” Journal of Sensors, vol. 2016, pp. 1–23,
 *     2016, doi: 10.1155/2016/8742920.
 *
 * @version 0.1
 *
 * @date 2021-08-31
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef DISPARITY_MAP_UTILITY_H_LF
#define DISPARITY_MAP_UTILITY_H_LF
#include <cstdint>

#ifndef SAFE_DELETE
#define SAFE_DELETE(P) {if(P) delete[](P);(P)=nullptr;}
#endif

/**
 * @brief dm is the abbreviation of disparity map.
 */
namespace dm_util
{
    void censusTransform(const uint8_t* src, uint32_t* census, uint16_t width, uint16_t height, uint8_t rw = 2, uint8_t rh = 2);

    uint8_t hammingDistance(uint32_t x, uint32_t y);

    enum AggregationPath
    {
        AGGREGATION_PATH_L2R    = 0,  //!< left --> right
        AGGREGATION_PATH_R2L    = 1,  //!< right --> right
        AGGREGATION_PATH_T2B    = 2,  //!< top --> bottom
        AGGREGATION_PATH_B2T    = 3,  //!< bottom --> top
        AGGREGATION_PATH_LT2RB  = 4,  //!< left top --> right bottom
        AGGREGATION_PATH_RB2LT  = 5,  //!< right bottom --> left top
        AGGREGATION_PATH_RT2LB  = 6,  //!< right top --> left bottom
        AGGREGATION_PATH_LB2RT  = 7   //!< left bottom --> right top
    };

    /* Aggregation Path
     * 1. Left      <->   Right
     * 2. Up        <->   Down
     * 3. Left-Top  <->   Right-Down
     * 4. Right-Top <->   Left-Down
     *   ↘ ↓ ↙   5  3  7
     *   →   ←	 1     2
     *   ↗ ↑ ↖   8  4  6
     */

    void costAggregation(const uint8_t* src, const uint8_t* cost_init, uint16_t width, uint16_t height, uint16_t d_range, int P1, int P2, AggregationPath path, uint8_t* dst);


	
    void medianFilter(const float *src, float *dst, uint16_t width, uint16_t height, uint16_t radius = 1);


    void removeSpeckles(float *disparity_map, uint16_t width, uint16_t height, int32_t thresh, uint32_t min_speckle_area, float invalid_val);
}

#endif // DISPARITY_MAP_UTILITY_H_LF
