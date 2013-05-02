FILE(REMOVE_RECURSE
  "../srv_gen"
  "../src/hector_nav_msgs/srv"
  "CMakeFiles/ROSBUILD_gensrv_cpp"
  "../srv_gen/cpp/include/hector_nav_msgs/GetRecoveryInfo.h"
  "../srv_gen/cpp/include/hector_nav_msgs/GetDistanceToObstacle.h"
  "../srv_gen/cpp/include/hector_nav_msgs/GetSearchPosition.h"
  "../srv_gen/cpp/include/hector_nav_msgs/GetRobotTrajectory.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
