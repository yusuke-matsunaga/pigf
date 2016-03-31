#ifndef LXSTATE_H
#define LXSTATE_H

/// @file LxState.h
/// @brief LxState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "McState.h"
#include "Variable.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

class VarPool;

//////////////////////////////////////////////////////////////////////
/// @class LxState LxState.h "LxState.h"
/// @brief LxGen で用いる McState の継承クラス
///
/// 具体的には変数番号の部分集合が状態となる．
//////////////////////////////////////////////////////////////////////
class LxState :
  public McState
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数番号のリスト
  /// @param[in] rv_list 登録ベクタのリスト
  /// @param[in] var_pool 記録用のオブジェクト
  LxState(const vector<Variable>& var_list,
	  const vector<const RegVect*>& rv_list,
	  VarPool& var_pool);

  /// @brief デストラクタ
  virtual
  ~LxState();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  virtual
  void
  init();

  /// @brief ランダムに近傍に遷移する．
  virtual
  void
  random_move();

  /// @brief 直前の遷移を取り消す．
  virtual
  void
  reset_move();

  /// @brief 現在の状態の評価値を返す．
  virtual
  double
  value();

  /// @brief 現在の状態を記録する．
  virtual
  void
  record();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ランダムに変数を取ってくる．
  Variable
  random_select();

  /// @brief 評価値を計算する．
  double
  _value();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数発生器
  RandGen mRandGen;

  // プライマリ変数のリスト
  const vector<Variable>& mVarList;

  // 登録ベクタのリスト
  const vector<const RegVect*>& mRvList;

  // 現在の状態
  Variable mCurState;

  // 現在の評価値
  double mCurVal;

  // 直前の状態
  Variable mPrevState;

  // 直前の評価値
  double mPrevVal;

  // 記録用のオブジェクト
  VarPool& mVarPool;

};

END_NAMESPACE_IGF

#endif // LXSTATE_H
