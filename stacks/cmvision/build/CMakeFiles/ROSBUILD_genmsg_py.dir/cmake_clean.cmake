FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/cmvision/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/cmvision/msg/__init__.py"
  "../src/cmvision/msg/_Blob.py"
  "../src/cmvision/msg/_Blobs.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
