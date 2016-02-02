#ifndef MCMC2_LXGEN_H
#define MCMC2_LXGEN_H

/// @file MCMC2_LxGen.h
/// @brief MCMC2_LxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC_LxGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class MCMC2_LxGen MCMC2_LxGen.h "MCMC2_LxGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class MCMC2_LxGen :
  public MCMC_LxGen
{
public:

  /// @brief コンストラクタ
  MCMC2_LxGen();

  /// @brief デストラクタ
  virtual
  ~MCMC2_LxGen();


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

END_NAMESPACE_YM_IGF


#endif // MCMC_LXGEN_H
