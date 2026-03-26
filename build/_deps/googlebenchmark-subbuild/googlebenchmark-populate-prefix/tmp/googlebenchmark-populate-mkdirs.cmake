# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/engine-study/build/_deps/googlebenchmark-src")
  file(MAKE_DIRECTORY "E:/engine-study/build/_deps/googlebenchmark-src")
endif()
file(MAKE_DIRECTORY
  "E:/engine-study/build/_deps/googlebenchmark-build"
  "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix"
  "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/tmp"
  "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
  "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src"
  "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/engine-study/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
