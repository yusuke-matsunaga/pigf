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

#include (FindQt4)
#find_package(Qt4 4.8.0 REQUIRED QtCore QtGui QtXml)
#include(${QT_USE_FILE})

#set( CMAKE_INCLUDE_CURRENT_DIR ON )
#find_package(Qt5Widgets)
#find_package(OpenGL)

#find_package(TCL)

find_package (GTest)

#include (FindGMP)

#include (FindPOPT)

#include (FindReadline)

find_package(Gperftools)

find_package(YmTools REQUIRED)

#if (TCL_FOUND)
#  find_package(YmTclpp)
#endif (TCL_FOUND)

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
  src/igugen/VarHeap.cc
  )

set (libigf_SOURCES
  src/libigf/IGU.cc
  src/libigf/IguGen.cc
  src/libigf/PhfGraph.cc
  src/libigf/RandHashGen.cc
  src/libigf/RvMgr.cc
  src/libigf/VarFunc.cc
  src/libigf/Variable.cc
  src/libigf/XorFunc.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(libigf STATIC ${libigf_SOURCES})

add_executable(igugen
  ${igugen_SOURCES}
  )

target_link_libraries(igugen
  libigf
  ym_algo
  ym_utils
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
