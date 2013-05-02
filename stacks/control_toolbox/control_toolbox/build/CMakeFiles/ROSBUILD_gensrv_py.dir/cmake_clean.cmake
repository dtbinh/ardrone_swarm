FILE(REMOVE_RECURSE
  "../srv_gen"
  "../src/control_toolbox/srv"
  "CMakeFiles/ROSBUILD_gensrv_py"
  "../src/control_toolbox/srv/__init__.py"
  "../src/control_toolbox/srv/_SetPidGains.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
