
/// @file BaseGen.cc
/// @brief BaseGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BaseGen.h"
#include "Greedy_BaseGen.h"
#include "MCMC_BaseGen.h"
#include "MCMC2_BaseGen.h"
#include "MCMC3_BaseGen.h"
#include "Shift_BaseGen.h"
#include "Simple_BaseGen.h"


BEGIN_NAMESPACE_IGF

// @brief インスタンスを生成するクラスメソッド
// @param[in] method アルゴリズム名
BaseGen*
BaseGen::new_obj(string method)
{
  if ( method == "Greedy" ) {
    return new Greedy_BaseGen();
  }
  if ( method == "MCMC" ) {
    return new MCMC_BaseGen();
  }
  if ( method == "MCMC2" ) {
    return new MCMC2_BaseGen();
  }
  if ( method == "MCMC3" ) {
    return new MCMC3_BaseGen();
  }
  if ( method == "Shift" ) {
    return new Shift_BaseGen();
  }
  if ( method == "Simple" ) {
    return new Simple_BaseGen();
  }
  cerr << "Error in BaseGen::new_obj(" << method << "): illegal method" << endl;
  return nullptr;
}

END_NAMESPACE_IGF
