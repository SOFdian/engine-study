# CMake generated Testfile for 
# Source directory: E:/engine-study/tests
# Build directory: E:/engine-study/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(EngineUnitTests "E:/engine-study/build/tests/Debug/unit_tests.exe")
  set_tests_properties(EngineUnitTests PROPERTIES  _BACKTRACE_TRIPLES "E:/engine-study/tests/CMakeLists.txt;21;add_test;E:/engine-study/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(EngineUnitTests "E:/engine-study/build/tests/Release/unit_tests.exe")
  set_tests_properties(EngineUnitTests PROPERTIES  _BACKTRACE_TRIPLES "E:/engine-study/tests/CMakeLists.txt;21;add_test;E:/engine-study/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(EngineUnitTests "E:/engine-study/build/tests/MinSizeRel/unit_tests.exe")
  set_tests_properties(EngineUnitTests PROPERTIES  _BACKTRACE_TRIPLES "E:/engine-study/tests/CMakeLists.txt;21;add_test;E:/engine-study/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(EngineUnitTests "E:/engine-study/build/tests/RelWithDebInfo/unit_tests.exe")
  set_tests_properties(EngineUnitTests PROPERTIES  _BACKTRACE_TRIPLES "E:/engine-study/tests/CMakeLists.txt;21;add_test;E:/engine-study/tests/CMakeLists.txt;0;")
else()
  add_test(EngineUnitTests NOT_AVAILABLE)
endif()
