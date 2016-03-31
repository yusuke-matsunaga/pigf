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
set (IGF_VERSION_MAJOR 1)
set (IGF_VERSION_MINOR 0)
set (IGF_VERSION "${IGF_VERSION_MAJOR}.${IGF_VERSION_MINOR}")

# 名前空間
set ( IGF_NAMESPACE "nsIgf" )


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

# Python のバージョン指定
set( Python_ADDITIONAL_VERSIONS 3.6 3.5 3.4 3.3 3.2 3.1 )
include (FindPythonInterp)
include (FindPythonLibs)

include( UseCython )

find_package(Gperftools)

include ( YmUtils )

ym_init( "" )


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)
enable_testing()


# ===================================================================
# ヘッダファイルの生成
# ===================================================================

configure_file(
  "${PROJECT_SOURCE_DIR}/etc/version_config.h.in"
  "${PROJECT_BINARY_DIR}/version_config.h"
  )


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_BINARY_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( ym-common )
add_subdirectory ( gtest )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (igugen_SOURCES
  src/igugen/igugen.cc
  )

set (common_SOURCES
  src/common/BasisChecker.cc
  src/common/Partitioner.cc
  src/common/RvMgr.cc
  src/common/SigFunc.cc
  src/common/VarPool.cc
  src/common/Variable.cc
  )

set (lxgen_SOURCES
  src/lxgen/Greedy_LxGen.cc
  src/lxgen/MCMC_LxGen.cc
  src/lxgen/MCMC2_LxGen.cc
  src/lxgen/MCMC3_LxGen.cc
  src/lxgen/LxGen.cc
  src/lxgen/LxGenBase.cc
  src/lxgen/Shift_LxGen.cc
  src/lxgen/Simple_LxGen.cc
  src/lxgen/VarHeap.cc

  src/lxgen/MH.cc
  src/lxgen/LxState.cc
  )

set (libigf_SOURCES
  src/libigf/phf/PhfGraph.cc
  src/libigf/IGU.cc
  src/libigf/IguGen.cc
  src/libigf/RandHashGen.cc
  src/libigf/RandSigFuncGen.cc
  src/libigf/SigFuncGen.cc
  src/libigf/RandSigFuncGen.cc
  )


# ===================================================================
#  library ターゲットの設定
# ===================================================================

ym_add_object_library (libigf
  ${common_SOURCES}
  ${lxgen_SOURCES}
  )


# ===================================================================
#  relase ターゲットの設定
# ===================================================================

add_executable(igugen
  ${igugen_SOURCES}
  $<TARGET_OBJECTS:libigf>
  $<TARGET_OBJECTS:ym_common>
  )

target_compile_options (igugen
  PRIVATE "-O3"
  )

target_compile_definitions (igugen
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(igugen
  ${YM_LIB_DEPENDS}
  )

add_executable(lxgen
  src/igugen/lxgen.cc
  $<TARGET_OBJECTS:libigf>
  $<TARGET_OBJECTS:ym_common>
  )

target_compile_options (lxgen
  PRIVATE "-O3"
  )

target_compile_definitions (lxgen
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(lxgen
  ${YM_LIB_DEPENDS}
  )



# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================

add_executable(igugen_p
  ${igugen_SOURCES}
  $<TARGET_OBJECTS:libigf_p>
  $<TARGET_OBJECTS:ym_common_p>
  )

target_compile_options (igugen_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (igugen_p
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(igugen_p
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  debug ターゲットの設定
# ===================================================================

add_executable(igugen_d
  ${igugen_SOURCES}
  $<TARGET_OBJECTS:libigf_d>
  $<TARGET_OBJECTS:ym_common_d>
  )

target_compile_options (igugen_d
  PRIVATE "-g"
  )

target_link_libraries(igugen_d
  ${YM_LIB_DEPENDS}
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
