#include "cmd.h"


bool CMD::is_show_fps = false;

CMD::Capture CMD::capture = { false, 1, "", "./capture", true };

bool CMD::is_enhance = false;

//std::string CMD::cam_params_path = "../../conf/params_NO24.yml";
std::string CMD::cam_params_path = "D:/MyProjects/Vision/conf/params_NO14.yml";
//std::string CMD::cam_params_path = "params_NO24.yml";

bool CMD::is_rectify = false;
