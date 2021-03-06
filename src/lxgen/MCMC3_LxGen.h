#ifndef MCMC3_LXGEN_H
#define MCMC3_LXGEN_H

/// @file MCMC3_LxGen.h
/// @brief MCMC3_LxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC_LxGen.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class MCMC3_LxGen MCMC3_LxGen.h "MCMC3_LxGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class MCMC3_LxGen :
  public MCMC_LxGen
{
public:

  /// @brief コンストラクタ
  MCMC3_LxGen();

  /// @brief デストラクタ
  virtual
  ~MCMC3_LxGen();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の価値を計算する．
  virtual
  double
  value(const Variable& var);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_IGF


#endif // MCMC_LXGEN_H
