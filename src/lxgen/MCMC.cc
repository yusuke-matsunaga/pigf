
/// @file MCMC.cc
/// @brief MCMC の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス MCMC
//////////////////////////////////////////////////////////////////////

// @brief サンプリングを行う．
// @param[in] warmup_count ウォーミングアップのために捨てるサンプル数
// @param[in] interval サンプリングの間隔
// @param[in] sample_count 総サンプル数
void
MCMC::sampling(ymuint warmup_count,
	       ymuint interval,
	       ymuint sample_count)
{
  // 初期化
  double cur_val = init();

  // ウォーミングアップ
  for (ymuint i = 0; i < warmup_count; ++ i) {
    one_step();
  }

  // サンプリングのメインループ
  for (ymuint i = 0; i < sample_count; ++ i) {
    // 現在の状態を記録する．
    record_sample();

    // interval 回だけ状態遷移を行う．
    for (ymuint j = 0; j < interval; ++ j) {
      one_step();
    }
  }
}

// @brief 1ステップの処理を行う．
//
// 次の状態候補を生成して規則にしたがって accept/reject する．
void
MCMC::one_step()
{
  double next_val = next_move();
  // next_val が cur_val より大きいか，
  // next_val / cur_val が生成した乱数(0〜1)より大きければ
  // accept する．
  if ( next_val < cur_val ) {
    double ratio = next_val / cur_val;
    double r = mRandGen.real1();
    if ( r > ratio ) {
      // 棄却する．
      return;
    }
  }

  // 受容する．
  accept_move();
}

END_NAMESPACE_IGF
