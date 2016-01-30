
/// @file LxGen.cc
/// @brief LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LxGen.h"
#include "Greedy_LxGen.h"
#include "MCMC_LxGen.h"
#include "Shift_LxGen.h"
#include "Simple_LxGen.h"


BEGIN_NAMESPACE_YM_IGF

// @brief インスタンスを生成するクラスメソッド
// @param[in] method アルゴリズム名
LxGen*
LxGen::new_obj(string method)
{
  if ( method == "Greedy" ) {
    return new Greedy_LxGen();
  }
  if ( method == "MCMC" ) {
    return new MCMC_LxGen();
  }
  if ( method == "Shift" ) {
    return new Shift_LxGen();
  }
  if ( method == "Simple" ) {
    return new Simple_LxGen();
  }
  cerr << "Error in LxGen::new_obj(" << method << "): illegal method" << endl;
  return nullptr;
}

END_NAMESPACE_YM_IGF
