
/// @file MH.cc
/// @brief MH の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.

#include "MH.h"
#include "McState.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス MH
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MH::MH()
{
}

// @brief デストラクタ
MH::~MH()
{
}

// @brief サンプリングを行う．
// @param[in] state 状態を表すクラス
// @param[in] warmup_count ウォーミングアップのために捨てるサンプル数
// @param[in] interval サンプリングの間隔
// @param[in] sample_count 総サンプル数
void
MH::sampling(McState& state,
	     ymuint warmup_count,
	     ymuint interval,
	     ymuint sample_count)
{
  // 初期化
  state.init();

  // 現在の評価値を記録．
  mCurVal = state.value();

  // ウォーミングアップ
  for (ymuint i = 0; i < warmup_count; ++ i) {
    one_step(state);
  }

  // サンプリングのメインループ
  for (ymuint i = 0; i < sample_count; ++ i) {
    // 現在の状態を記録する．
    state.record();

    // 次の状態に遷移
    one_step(state);

    // interval 回だけ記録せずに遷移する．
    for (ymuint j = 0; j < interval; ++ j) {
      one_step(state);
    }
  }
}

// @brief 1ステップの処理を行う．
// @param[in] state 状態
//
// 次の状態候補を生成して規則にしたがって accept/reject する．
void
MH::one_step(McState& state)
{
  // ランダムに近傍に遷移する．
  state.random_move();

  double next_val = state.value();

  // next_val が mCurVal より大きいか，
  // next_val / mCurVal が生成した乱数(0〜1)より大きければ
  // accept する．
  // そうでなければ元の状態に戻す．
  if ( next_val < mCurVal ) {
    double ratio = next_val / mCurVal;
    double r = mAcceptRandGen.real1();
    if ( r > ratio ) {
      // 棄却する．
      state.reset_move();
      return;
    }
  }

  // 受容する．
  mCurVal = next_val;
}

END_NAMESPACE_IGF
