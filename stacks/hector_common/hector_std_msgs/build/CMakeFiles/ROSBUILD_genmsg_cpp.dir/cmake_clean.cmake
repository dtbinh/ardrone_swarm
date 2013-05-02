FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/hector_std_msgs/msg"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/hector_std_msgs/Float32Array.h"
  "../msg_gen/cpp/include/hector_std_msgs/UInt16Array.h"
  "../msg_gen/cpp/include/hector_std_msgs/UInt16.h"
  "../msg_gen/cpp/include/hector_std_msgs/Float32.h"
  "../msg_gen/cpp/include/hector_std_msgs/Float64.h"
  "../msg_gen/cpp/include/hector_std_msgs/Float64Array.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
