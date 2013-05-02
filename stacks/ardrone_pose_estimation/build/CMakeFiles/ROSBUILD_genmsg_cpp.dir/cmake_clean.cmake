FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/ardrone_pose_estimation/msg"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/ardrone_pose_estimation/Pose.h"
  "../msg_gen/cpp/include/ardrone_pose_estimation/KeyCode.h"
  "../msg_gen/cpp/include/ardrone_pose_estimation/Navdata.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
