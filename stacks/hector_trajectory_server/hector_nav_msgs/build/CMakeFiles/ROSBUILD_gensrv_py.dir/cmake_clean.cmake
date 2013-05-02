FILE(REMOVE_RECURSE
  "../srv_gen"
  "../src/hector_nav_msgs/srv"
  "CMakeFiles/ROSBUILD_gensrv_py"
  "../src/hector_nav_msgs/srv/__init__.py"
  "../src/hector_nav_msgs/srv/_GetRecoveryInfo.py"
  "../src/hector_nav_msgs/srv/_GetDistanceToObstacle.py"
  "../src/hector_nav_msgs/srv/_GetSearchPosition.py"
  "../src/hector_nav_msgs/srv/_GetRobotTrajectory.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
