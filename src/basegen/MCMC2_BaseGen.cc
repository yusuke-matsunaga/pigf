
/// @file MCMC2_BaseGen.cc
/// @brief MCMC2_BaseGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC2_BaseGen.h"
#include "RegVect.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス MCMC2_BaseGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MCMC2_BaseGen::MCMC2_BaseGen()
{
}

// @brief デストラクタ
MCMC2_BaseGen::~MCMC2_BaseGen()
{
}

// @brief 変数の価値を計算する．
double
MCMC2_BaseGen::value(const Variable& var)
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

END_NAMESPACE_IGF
