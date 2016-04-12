
/// @file VariableTest.cc
/// @brief VariableTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "Variable.h"


BEGIN_NAMESPACE_IGF

// コンストラクタのテスト
TEST(VariableTest, constructor)
{
  ymuint n = 10;
  for (ymuint i = 0; i < n; ++ i) {
    Variable a(n, i);

    vector<ymuint> vlist = a.vid_list();
    EXPECT_EQ( 1, vlist.size() );
    EXPECT_EQ( i, vlist[0] );

  }
}

// コピーコンストラクタのテスト
TEST(VariableTest, copy_constructor)
{
  ymuint n = 10;
  for (ymuint i = 0; i < n; ++ i) {
    Variable a(n, i);

    Variable b(a);

    EXPECT_TRUE( a == b );

    vector<ymuint> vlist = b.vid_list();
    EXPECT_EQ( 1, vlist.size() );
    EXPECT_EQ( i, vlist[0] );
  }
}

// 合成演算のテスト
TEST(VariableTest, compose)
{
  ymuint n = 10;
  for (ymuint i = 0; i < n; ++ i) {
    Variable a(n, i);

    for (ymuint j = i + 1; j < n; ++ j) {
      Variable b(n, j);

      Variable c = a * b;

      vector<ymuint> vlist = c.vid_list();
      EXPECT_EQ( 2, vlist.size() );
      EXPECT_EQ( i, vlist[0] );
      EXPECT_EQ( j, vlist[1] );
    }
  }
}

// コンストラクタのテスト
TEST(VariableTest, large_constructor)
{
  ymuint n = 100;
  for (ymuint i = 0; i < n; ++ i) {
    Variable a(n, i);

    vector<ymuint> vlist = a.vid_list();
    EXPECT_EQ( 1, vlist.size() );
    EXPECT_EQ( i, vlist[0] );

  }
}

// コピーコンストラクタのテスト
TEST(VariableTest, large_copy_constructor)
{
  ymuint n = 100;
  for (ymuint i = 0; i < n; ++ i) {
    Variable a(n, i);

    Variable b(a);

    EXPECT_TRUE( a == b );

    vector<ymuint> vlist = b.vid_list();
    EXPECT_EQ( 1, vlist.size() );
    EXPECT_EQ( i, vlist[0] );
  }
}

// 合成演算のテスト
TEST(VariableTest, large_compose)
{
  ymuint n = 100;
  for (ymuint i = 0; i < n; ++ i) {
    Variable a(n, i);

    for (ymuint j = i + 1; j < n; ++ j) {
      Variable b(n, j);

      Variable c = a * b;

      vector<ymuint> vlist = c.vid_list();
      EXPECT_EQ( 2, vlist.size() );
      EXPECT_EQ( i, vlist[0] );
      EXPECT_EQ( j, vlist[1] );
    }
  }
}

END_NAMESPACE_IGF
