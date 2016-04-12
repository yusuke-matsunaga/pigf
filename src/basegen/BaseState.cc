
/// @file BaseState.cc
/// @brief BaseState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BaseState.h"
#include "VarPool.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス BaseState
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数番号のリスト
// @param[in] rv_list 登録ベクタのリスト
// @param[in] var_pool 記録用のオブジェクト
BaseState::BaseState(const vector<Variable>& var_list,
		 const vector<const RegVect*>& rv_list,
		 VarPool& var_pool) :
  mVarList(var_list),
  mRvList(rv_list),
  mVarPool(var_pool)
{
}

// @brief デストラクタ
BaseState::~BaseState()
{
}

// @brief 初期化する．
void
BaseState::init()
{
  mCurState = random_select();
  mCurVal = _value();
  // 念のため
  mPrevState = mCurState;
  mPrevVal = mCurVal;
}

// @brief ランダムに近傍に遷移する．
void
BaseState::random_move()
{
  // mVarList の中からランダムに選ぶ．
  Variable var1 = random_select();

  // 現在の状態をバックアップしておく．
  mPrevState = mCurState;
  mPrevVal = mCurVal;

  // 今の変数と合成する．
  mCurState *= var1;
  mCurVal = _value();
}

// @brief 直前の遷移を取り消す．
void
BaseState::reset_move()
{
  mCurState = mPrevState;
  mCurVal = mPrevVal;
}

// @brief 現在の状態の評価値を返す．
double
BaseState::value()
{
  return mCurVal;
}

// @brief 現在の状態を記録する．
void
BaseState::record()
{
  mVarPool.put(mCurState, mCurVal);
}

// @brief ランダムに変数を取ってくる．
Variable
BaseState::random_select()
{
  // mVarList の中からランダムに選ぶ．
  ymuint pos = mRandGen.int32() % mVarList.size();
  return mVarList[pos];
}

// @brief 評価値を計算する．
double
BaseState::_value()
{
  return mCurState.value(mRvList);
}

END_NAMESPACE_IGF
