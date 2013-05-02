FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/ardrone_pose_estimation/msg"
  "CMakeFiles/ROSBUILD_gencfg_cpp"
  "../cfg/cpp/ardrone_pose_estimation/onlinegainConfig.h"
  "../docs/onlinegainConfig.dox"
  "../docs/onlinegainConfig-usage.dox"
  "../src/ardrone_pose_estimation/cfg/onlinegainConfig.py"
  "../docs/onlinegainConfig.wikidoc"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gencfg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
