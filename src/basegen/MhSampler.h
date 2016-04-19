#ifndef MHSAMPLER_H
#define MHSAMPLER_H

/// @file MhSampler.h
/// @brief MhSampler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

class MhState;

//////////////////////////////////////////////////////////////////////
/// @class MhSampler MhSampler.h "MhSampler.h"
/// @brief Metropolis-Hastings 法を行うクラス
//////////////////////////////////////////////////////////////////////
class MhSampler
{
public:

  /// @brief コンストラクタ
  MhSampler();

  /// @brief デストラクタ
  ~MhSampler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サンプリングを行う．
  /// @param[in] state 状態を表すクラス
  /// @param[in] warmup ウォーミングアップの回数
  /// @param[in] interval サンプリングインターバル (0 で毎回)
  /// @param[in] sample_count 総サンプル数
  ///
  /// 結果は state.record() で記録される．
  void
  sampling(MhState& state,
	   ymuint warmup,
	   ymuint interval,
	   ymuint sample_count);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1ステップの処理を行う．
  /// @param[in] state 状態
  ///
  /// 次の状態候補を生成して規則にしたがって accept/reject する．
  void
  one_step(MhState& state);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // accept/reject 判定用の乱数発生器
  RandGen mAcceptRandGen;

  // 現在の評価値
  double mCurVal;

};

END_NAMESPACE_IGF

#endif // MHSAMPLER_H