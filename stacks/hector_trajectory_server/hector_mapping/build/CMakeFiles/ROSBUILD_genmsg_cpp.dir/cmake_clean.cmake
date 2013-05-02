FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/hector_mapping/msg"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/hector_mapping/HectorDebugInfo.h"
  "../msg_gen/cpp/include/hector_mapping/HectorIterData.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
