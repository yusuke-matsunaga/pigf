
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${YmTools_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( TEST_SOURCES
  VariableTest.cc
  RegVectTest.cc
  )


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

add_executable ( libIgfTest
  ${TEST_SOURCES}
  )

target_compile_options ( libIgfTest
  PRIVATE "-g"
  )

target_link_libraries ( libIgfTest
  libigf_d
  ym_utils_d
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( AllTestsInPIGF
  libIgfTest
  )
