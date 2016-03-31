#ifndef MH_H
#define MH_H

/// @file MH.h
/// @brief MH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

class McState;

//////////////////////////////////////////////////////////////////////
/// @class MH MH.h "MH.h"
/// @brief Metropolis-Hastings 法を行うクラス
//////////////////////////////////////////////////////////////////////
class MH
{
public:

  /// @brief コンストラクタ
  MH();

  /// @brief デストラクタ
  ~MH();


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
  sampling(McState& state,
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
  one_step(McState& state);


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

#endif // MH_H
