FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/hector_mapping/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/hector_mapping/msg/__init__.py"
  "../src/hector_mapping/msg/_HectorDebugInfo.py"
  "../src/hector_mapping/msg/_HectorIterData.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
