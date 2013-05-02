FILE(REMOVE_RECURSE
  "msg_gen"
  "srv_gen"
  "src/ar_recog/msg"
  "src/ar_recog/srv"
  "CMakeFiles/clean_test_results"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/clean_test_results.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
