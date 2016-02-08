
/// @file MCMC3_LxGen.cc
/// @brief MCMC3_LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC3_LxGen.h"
#include "RegVect.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス MCMC3_LxGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MCMC3_LxGen::MCMC3_LxGen()
{
}

// @brief デストラクタ
MCMC3_LxGen::~MCMC3_LxGen()
{
}

// @brief 変数の価値を計算する．
double
MCMC3_LxGen::value(const Variable& var)
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
  ymuint64 nv = rv_list().size();

  if ( n0 > n1 ) {
    return 1.0 - static_cast<double>(n0 - n1) / nv;
  }
  else {
    return 1.0 - static_cast<double>(n1 - n0) / nv;
  }
}

END_NAMESPACE_IGF
