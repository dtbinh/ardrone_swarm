FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/ardrone_pose_estimation/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/ardrone_pose_estimation/msg/__init__.py"
  "../src/ardrone_pose_estimation/msg/_Pose.py"
  "../src/ardrone_pose_estimation/msg/_KeyCode.py"
  "../src/ardrone_pose_estimation/msg/_Navdata.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
