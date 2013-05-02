FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/hector_std_msgs/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/hector_std_msgs/msg/__init__.py"
  "../src/hector_std_msgs/msg/_Float32Array.py"
  "../src/hector_std_msgs/msg/_UInt16Array.py"
  "../src/hector_std_msgs/msg/_UInt16.py"
  "../src/hector_std_msgs/msg/_Float32.py"
  "../src/hector_std_msgs/msg/_Float64.py"
  "../src/hector_std_msgs/msg/_Float64Array.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
