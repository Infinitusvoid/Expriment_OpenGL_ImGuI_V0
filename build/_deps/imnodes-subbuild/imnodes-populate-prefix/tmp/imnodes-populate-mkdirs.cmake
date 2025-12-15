# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-src"
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-build"
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix"
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix/tmp"
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix/src/imnodes-populate-stamp"
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix/src"
  "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix/src/imnodes-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix/src/imnodes-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Cosmos/Downloads/ImGuiNodes_CMake_Starter_v3/ImGuiNodes_CMake_Starter_v3/build/_deps/imnodes-subbuild/imnodes-populate-prefix/src/imnodes-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
