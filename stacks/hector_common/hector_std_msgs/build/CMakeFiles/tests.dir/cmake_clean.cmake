FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/hector_std_msgs/msg"
  "CMakeFiles/tests"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/tests.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)