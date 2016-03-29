#ifndef MCMC_H
#define MCMC_H

/// @file MCMC.h
/// @brief MCMC のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class MCMC MCMC.h "MCMC.h"
/// @brief MCMC 法のフレームワーク
//////////////////////////////////////////////////////////////////////
class MCMC
{
public:

  /// @brief デストラクタ
  virtual
  ~MCMC() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サンプリングを行う．
  /// @param[in] warmup_count ウォーミングアップのために捨てるサンプル数
  /// @param[in] interval サンプリングの間隔
  /// @param[in] sample_count 総サンプル数
  void
  sampling(ymuint warmup_count,
	   ymuint interval,
	   ymuint sample_count);


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期状態を作る．
  /// @return 初期状態の評価値を返す．
  virtual
  double
  init() = 0;

  /// @brief 次の状態を作る．
  /// @return 次の状態の評価値を返す．
  virtual
  double
  next_move() = 0;

  /// @brief 直前の next_move() を受け入れる．
  virtual
  void
  accept_move() = 0;

  /// @brief サンプルを記録する．
  virtual
  void
  record_sample() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1ステップの処理を行う．
  ///
  /// 次の状態候補を生成して規則にしたがって accept/reject する．
  void
  one_step();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数発生器
  RandGen mRandGen;

};

END_NAMESPACE_IGF

#endif // MCMC_H
