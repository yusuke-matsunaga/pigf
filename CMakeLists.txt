# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.0)


# C++11 の指定
set (CMAKE_CXX_STANDARD 11)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (pigf)
set (PIGF_VERSION_MAJOR 1)
set (PIGF_VERSION_MINOR 0)
set (PIGF_VERSION "${PIGF_VERSION_MAJOR}.${PIGF_VERSION_MINOR}")


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
list ( APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/ym-common/cmake"
  )

include (FindDoxygen)

include (FindPythonInterp)
include (FindPythonLibs)

find_package(Gperftools)
find_package(YmTools REQUIRED)

include (ym_add_library)


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
include ( ym_use_gtest )
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)

enable_testing()


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory (
  gtest
  )

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${YmTools_INCLUDE_DIRS}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (igugen_SOURCES
  src/igugen/igugen.cc
  )

set (libigf_SOURCES
  src/libigf/IGU.cc
  src/libigf/IguGen.cc
  src/libigf/PhfGraph.cc
  src/libigf/RandHashGen.cc
  src/libigf/RvMgr.cc
  src/libigf/SigFunc.cc
  src/libigf/VarFunc.cc
  src/libigf/VarHeap.cc
  src/libigf/Variable.cc
  src/libigf/XorFunc.cc
  src/libigf/lxgen.cc
  src/libigf/lxgen_old.cc
  )


# ===================================================================
#  library ターゲットの設定
# ===================================================================

ym_add_library (libigf
  STATIC
  ${libigf_SOURCES})


# ===================================================================
#  relase ターゲットの設定
# ===================================================================

add_executable(igugen
  ${igugen_SOURCES}
  )

target_compile_options (igugen
  PRIVATE "-O3"
  )

target_compile_definitions (igugen
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(igugen
  libigf
  ym_algo
  ym_utils
  )


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================

add_executable(igugen_p
  ${igugen_SOURCES}
  )

target_compile_options (igugen_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (igugen_p
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(igugen_p
  libigf_p
  ym_algo_p
  ym_utils_p
  )


# ===================================================================
#  debug ターゲットの設定
# ===================================================================

add_executable(igugen_d
  ${igugen_SOURCES}
  )

target_compile_options (igugen_d
  PRIVATE "-g"
  )

target_link_libraries(igugen_d
  libigf_d
  ym_algo_d
  ym_utils_d
  )


# ===================================================================
#  テストの設定
# ===================================================================



# ===================================================================
# doxygen 起動用のコマンド
# ===================================================================

if (DOXYGEN_FOUND)
  configure_file (
    "${PROJECT_SOURCE_DIR}/etc/Doxyfile.in"
    "${PROJECT_BINARY_DIR}/Doxyfile"
    )

  add_custom_target(dox
    "${DOXYGEN_EXECUTABLE}" ">" "${PROJECT_BINARY_DIR}/doxygen.log"
    DEPENDS "${PROJECT_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMENT "generating doxygen documents"
    )
endif (DOXYGEN_FOUND)


# ===================================================================
# インストールターゲット
# ===================================================================
