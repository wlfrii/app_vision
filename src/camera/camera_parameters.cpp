#include "camera_parameters.h"
#include "../util/define.h"
#include <fstream>
#include <vector>
#include <string>

// matrix_name  expected_type  width  height
#define ASSERT_MATRIX_SIZE_TYPE(matrix, row, col)   \
    assert(matrix.size() == cv::Size(col, row));

CameraParameters::CameraParameters(const cv::Mat &intrinsic, const cv::Mat &dist_coeffs,
    const cv::Mat &R_rectify, const cv::Rect &roi, const cv::Mat &new_intrinsic)
    : _A(intrinsic)
    , _D(dist_coeffs)
    , _R(R_rectify)
    , _ROI(roi)
    , _Anew(new_intrinsic)
{
    ASSERT_MATRIX_SIZE_TYPE(intrinsic, 3, 3);
    ASSERT_MATRIX_SIZE_TYPE(dist_coeffs, 5, 1);
    ASSERT_MATRIX_SIZE_TYPE(new_intrinsic, 3, 3);
    ASSERT_MATRIX_SIZE_TYPE(R_rectify, 3, 3);
    assert(roi.width > 0 && roi.height > 0);
}

CameraParameters::CameraParameters(const CameraParameters& rhs)
{
    _A = rhs._A;
    _D = rhs._D;
    _R = rhs._R;
    _ROI = rhs._ROI;
    _Anew = rhs._Anew;
}

#if LINUX
CameraParameters::CameraParameters(USBCameraParammeters &params)
    : _A(params._A.to_cvMat())
    , _D(params._D.to_cvMat())
    , _R(params._R.to_cvMat())
    , _ROI(params._ROI.to_cvRect())
    , _Anew(params._Anew.to_cvMat())
{
}
#endif

CameraParameters::_precision_ CameraParameters::getFX() const
{
    return _A.at<_precision_>(0, 0);
}
CameraParameters::_precision_ CameraParameters::getFY() const
{
    return _A.at<_precision_>(1, 1);
}
CameraParameters::_precision_ CameraParameters::getCX() const
{
    return _A.at<_precision_>(0, 2);
}
CameraParameters::_precision_ CameraParameters::getCY() const
{
    return _A.at<_precision_>(1, 2);
}
CameraParameters::_precision_ CameraParameters::getNewFX() const
{
    return _Anew.at<_precision_>(0, 0);
}
CameraParameters::_precision_ CameraParameters::getNewFY() const
{
    return _Anew.at<_precision_>(1, 1);
}
CameraParameters::_precision_ CameraParameters::getNewCX() const
{
    return _Anew.at<_precision_>(0, 2);
}
CameraParameters::_precision_ CameraParameters::getNewCY() const
{
    return _Anew.at<_precision_>(1, 2);
}
CameraParameters::_precision_ CameraParameters::getK1() const
{
    return _D.at<_precision_>(0, 0);
}
CameraParameters::_precision_ CameraParameters::getK2() const
{
    return _D.at<_precision_>(1, 0);
}
CameraParameters::_precision_ CameraParameters::getK3() const
{
    return _D.at<_precision_>(2, 0);
}
CameraParameters::_precision_ CameraParameters::getP1() const
{
    return _D.at<_precision_>(3, 0);
}
CameraParameters::_precision_ CameraParameters::getP2() const
{
    return _D.at<_precision_>(4, 0);
}
const cv::Mat& CameraParameters::getIntrinsic() const
{
    return _A;
}
const cv::Mat& CameraParameters::getDistCoeffs() const
{
    return _D;
}
const cv::Mat& CameraParameters::getRectifyMat() const
{
    return _R;
}
const cv::Mat& CameraParameters::getNewIntrinsic() const
{
    return _Anew;
}
const cv::Rect& CameraParameters::getROI() const
{
    return _ROI;
}

uint16_t CameraParameters::getFrameWidth() const
{
    return _ROI.width;
}

uint16_t CameraParameters::getFrameHeight() const
{
    return _ROI.height;
}

std::string CameraParameters::getInfo() const
{

    auto createInfo = [](const cv::Mat& mat, std::string name) -> std::string{
        char info[512];
        sprintf(info, "%s\n%f,%f,%f\n%f,%f,%f\n%f,%f,%f", name.c_str(),
                mat.at<_precision_>(0,0),mat.at<_precision_>(0,1),mat.at<_precision_>(0,2),
                mat.at<_precision_>(1,0),mat.at<_precision_>(1,1),mat.at<_precision_>(1,2),
                mat.at<_precision_>(2,0),mat.at<_precision_>(2,1),mat.at<_precision_>(2,2));
        return info;
    };
    auto A = createInfo(_A, "A");
    auto R = createInfo(_R, "R");
    auto Anew = createInfo(_Anew, "Anew");

    char D[128], ROI[128];
    sprintf(D, "D\n%f,%f,%f,%f,%f", _D.at<_precision_>(0,0), _D.at<_precision_>(1,0),
            _D.at<_precision_>(2,0), _D.at<_precision_>(3,0), _D.at<_precision_>(4,0));
    sprintf(ROI, "ROI\n%d,%d,%d,%d", _ROI.x, _ROI.y, _ROI.width, _ROI.height);

    std::string info = "Camera Parameters:\n "
            + A + '\n' + D + '\n' + R + "\n" + Anew + '\n' + ROI;
    return info;
}


std::string StereoCameraParameters::getInfo() const
{
    auto left_info = left->getInfo();
    auto right_info = right->getInfo();
    std::string info = "Left ---- " + left_info + "\nRight ---- " + right_info;
    return info;
}

/* ------------------------------------------------------------------------ */
/*                          CameraParamsYMLReader                           */
/* ------------------------------------------------------------------------ */

CameraParamsYMLReader::CameraParamsYMLReader(const std::string &cam_params_path)
    : _fs(cv::FileStorage(cam_params_path, cv::FileStorage::READ))
{
    if (!_fs.isOpened())
    {
        printf("CameraParamsReader: Cannot open the camera parameters file!\n");
        _fs.release();
        _is_valid_path = false;
    }
    _is_valid_path = true;

//    _fs["image_width"] >> _frame_width;
//    _fs["image_height"] >> _frame_height;
}

CameraParamsYMLReader::~CameraParamsYMLReader()
{
    _fs.release();
}

#define GET_DATA_BY_NAME(file, variable, index)				\
{															\
	std::string tmp = #variable + std::to_string(index);	\
    file[tmp] >> variable;									\
}

std::shared_ptr<CameraParameters> CameraParamsYMLReader::getCameraParameters(uint8_t index) const
{
    if (!_is_valid_path)
        return nullptr;

    cv::Mat A;
    cv::Mat D;
    cv::Mat R;
    cv::Rect roi;
    cv::Mat Anew;
    int camera_index = (index == vision::LEFT_CAMERA ? 1 : 2);
    GET_DATA_BY_NAME(_fs, A, camera_index);
    GET_DATA_BY_NAME(_fs, D, camera_index);
    GET_DATA_BY_NAME(_fs, R, camera_index);
    GET_DATA_BY_NAME(_fs, roi, camera_index);

    if (D.rows == 1)
        D = D.t();
    _fs["Anew"] >> Anew;
    if (Anew.empty())
        Anew = A;
    if (roi.empty())
        roi = cv::Rect(0, 0, 1, 1);

	auto check = [](cv::Mat& mat) {
        if (mat.type() != CV_64F) {
            cv::Mat tmp(mat.size(), CV_64F);
            mat.convertTo(tmp, CV_64F);
			mat = tmp;
		}
	}; 
	check(A);
	check(D);
	check(R);
	check(Anew);

    return std::make_shared<CameraParameters>(A, D, R, roi, Anew);
}

std::shared_ptr<StereoCameraParameters> CameraParamsYMLReader::getStereoCameraParameters() const
{
    if (!_is_valid_path)
        return nullptr;

    std::shared_ptr<CameraParameters> params[2];
    for(auto i = 0; i < 2;i++)
    {
        params[i] = getCameraParameters(i);
    }
    return std::make_shared<StereoCameraParameters>(params[0], params[1]);
}



std::vector<double> praseData(const std::string& data)
{
    std::vector<double> ret;
    int cpos = 0;
    while(true){
        std::string val_str;
        auto pos = data.find(',', cpos);
        if(pos != std::string::npos){
            val_str = data.substr(cpos, pos - cpos);
            ret.push_back(std::stold(val_str));
            cpos = pos + 1;
        }
        else{
            val_str = data.substr(cpos, data.length() - cpos);
            ret.push_back(std::stold(val_str));
            break;
        }
    }
    return ret;
};

CameraParamsCSVReader::CameraParamsCSVReader(const std::string &cam_params_path)
{
    std::ifstream ifile(cam_params_path, std::ios_base::in);
    if(!ifile.is_open()){
        _is_valid_path = false;
    }

    std::string linedata;
    auto getData = [&ifile, &linedata](std::string name) -> double{
        std::getline(ifile, linedata);
        int pos = linedata.find(name);
        auto val_str = linedata.substr(pos + name.size() + 1, linedata.size());
        return std::stold(val_str);
    };
    auto getMat3 = [&ifile, &linedata]() ->cv::Mat{
        cv::Mat mat(3, 3, CV_64FC1);
        for(int i = 0; i < 3; i++){
            std::getline(ifile, linedata);
            std::vector<double> data = praseData(linedata);
            for(int j = 0; j < 3; j++){
                mat.at<double>(i, j) = data[j];
            }
        }
        return mat;
    };
    auto getD = [&ifile, &linedata]() -> cv::Mat{
        cv::Mat mat(5, 1, CV_64FC1);
        std::getline(ifile, linedata);
        std::vector<double> data = praseData(linedata);
        for(int j = 0; j < 5; j++){
            mat.at<double>(j, 0) = data[j];
        }
        return mat;
    };
    auto getROI = [&ifile, &linedata]() -> cv::Rect{
        std::getline(ifile, linedata);
        std::vector<double> data = praseData(linedata);
        cv::Rect roi(data[0], data[1], data[2], data[3]);
        return roi;
    };
    int frame_width = getData("width");
    int frame_height = getData("height");
    _cam_fov = getData("fov");
    _cam_distance = getData("t");

    cv::Mat A1, A2, Anew, D1, D2, R1, R2;
    cv::Rect roi1, roi2;
    while(std::getline(ifile, linedata)){
        if(linedata == "A1"){
            A1 = getMat3();
        }
        if(linedata == "A2"){
            A2 = getMat3();
        }
        if(linedata == "Anew"){
            Anew = getMat3();
        }
        if(linedata == "D1"){
            D1 = getD();
        }
        if(linedata == "D2"){
            D2 = getD();
        }
        if(linedata == "R1"){
            R1 = getMat3();
        }
        if(linedata == "R2"){
            R2 = getMat3();
        }
        if(linedata == "roi1"){
            roi1 = getROI();
        }
        if(linedata == "roi2"){
            roi2 = getROI();
        }
    }
    _stereo_cam_params = std::make_shared<StereoCameraParameters>(
                std::make_shared<CameraParameters>(A1, D1, R1, roi1, Anew),
                std::make_shared<CameraParameters>(A2, D2, R2, roi2, Anew));
    _is_valid_path = true;
}

std::shared_ptr<CameraParameters> CameraParamsCSVReader::getCameraParameters(uint8_t index) const
{
    if (!_is_valid_path)
        return nullptr;
    return index == vision::LEFT_CAMERA ?
                _stereo_cam_params->left : _stereo_cam_params->right;
}

std::shared_ptr<StereoCameraParameters> CameraParamsCSVReader::getStereoCameraParameters() const
{
    if (!_is_valid_path)
        return nullptr;
    return _stereo_cam_params;
}


/* ------------------------------------------------------------------------ */
/*                      CameraParamsReaderFactory                           */
/* ------------------------------------------------------------------------ */

CameraParamsReaderFactory* CameraParamsReaderFactory::getInstance()
{
    static CameraParamsReaderFactory factory;
    return &factory;
}

std::unique_ptr<CameraParamsReader> CameraParamsReaderFactory::create(std::string cam_params_path)
{
    int pos = cam_params_path.rfind(".");
    std::string suffix = cam_params_path.substr(pos + 1, cam_params_path.length());

    if(suffix == "yml"){
        DEBUG("load yml file\n");
        return std::make_unique<CameraParamsYMLReader>(cam_params_path);
    }
    else {
        DEBUG("load csv/sr file\n");
        return std::make_unique<CameraParamsCSVReader>(cam_params_path);
    }
}

