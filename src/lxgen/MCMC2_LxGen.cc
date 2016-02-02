
/// @file MCMC2_LxGen.cc
/// @brief MCMC2_LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC2_LxGen.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス MCMC2_LxGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MCMC2_LxGen::MCMC2_LxGen()
{
}

// @brief デストラクタ
MCMC2_LxGen::~MCMC2_LxGen()
{
}

// @brief 変数の価値を計算する．
double
MCMC2_LxGen::value(const Variable& var)
{
  ymuint64 n0 = 0;
  ymuint64 n1 = 0;
  for (vector<const RegVect*>::const_iterator p = rv_list().begin();
       p != rv_list().end(); ++ p) {
    const RegVect* rv = *p;
    if ( rv->classify(var) ) {
      ++ n1;
    }
    else {
      ++ n0;
    }
  }

  if ( n0 > n1 ) {
    return static_cast<double>(n1) / static_cast<double>(n0);
  }
  else {
    return static_cast<double>(n0) / static_cast<double>(n1);
  }
}

END_NAMESPACE_YM_IGF
