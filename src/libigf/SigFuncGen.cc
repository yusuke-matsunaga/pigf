
/// @file SigFuncGen.cc
/// @brief SigFuncGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SigFuncGen.h"
#include "RegVect.h"
#include "SigFunc.h"
#include "Variable.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// var_list によって rv1 と rv2 が区別できるとき
// true を返す．
bool
compare(const RegVect* rv1,
	const RegVect* rv2,
	const vector<Variable>& var_list)
{
  for (ymuint i = 0; i < var_list.size(); ++ i) {
    const Variable& var = var_list[i];
    if ( rv1->classify(var) != rv2->classify(var) ) {
      return true;
    }
  }
  return false;
}

// 変数集合の価値を計算する．
double
calc_val(const vector<const RegVect*>& rv_list,
	 const vector<Variable>& var_list)
{
  ymuint64 n = rv_list.size();
  ymuint64 c = 0;
  for (ymuint i1 = 1; i1 < n; ++ i1) {
    const RegVect* rv1 = rv_list[i1];
    for (ymuint i2 = 0; i2 < i1; ++ i2) {
      const RegVect* rv2 = rv_list[i2];
      if ( compare(rv1, rv2, var_list) ) {
	++ c;
      }
    }
  }
  // 理想の値は n(n - 1) / 2
  ymuint64 ideal = n * (n - 1)  / 2;
  return static_cast<double>(c) / static_cast<double>(ideal);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス SigFuncGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SigFuncGen::SigFuncGen()
{
}

// @brief デストラクタ
SigFuncGen::~SigFuncGen()
{
}

// @brief 初期化を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] var_list 変数のリスト
// @param[in] width 出力のビット幅
// @param[in] m 多重度
// @param[in] burnin_int 'burn in' のための回数
// @param[in] simple_int サンプリングの間隔
void
SigFuncGen::init(const vector<const RegVect*>& rv_list,
		 const vector<Variable>& var_list,
		 ymuint width,
		 ymuint m,
		 ymuint burnin_int,
		 ymuint sample_int)
{
  mRvList = rv_list;
  mM = m;
  mSampleInt = sample_int;

  mCurStateArray.clear();
  mCurStateArray.resize(mM);
  for (ymuint i = 0; i < mM; ++ i) {
    mCurStateArray[i].init(rv_list, var_list, width, mRgChoose);
  }

  // burn-in を行う．
  for (ymuint c = 0; c < burnin_int; ++ c) {
    body();
  }
}

// @brief signature function を一つ生成する．
vector<const SigFunc*>
SigFuncGen::generate()
{
  for (ymuint c = 0; c < mSampleInt; ++ c) {
    body();
  }
  vector<const SigFunc*> ans(mM);
  for (ymuint i = 0; i < mM; ++ i) {
    ans[i] = mCurStateArray[i].new_func();
  }
  return ans;
}

// @brief 基本処理
void
SigFuncGen::body()
{
  // 独立に mM 個の処理を行う．
  for (ymuint i = 0; i < mM; ++ i) {
    mCurStateArray[i].next_move(mRvList, mRgChoose, mRgAccept);
  }
}

// @brief 初期化を行う．
// @param[in] var_list 変数のリスト
// @param[in] width 出力のビット幅
//
// ランダムに初期状態を決める．
void
SigFuncGen::FuncState::init(const vector<const RegVect*>& rv_list,
			    const vector<Variable>& var_list,
			    ymuint width,
			    RandGen& rg1)
{
  mCurState.clear();
  mCurState.resize(width);

  // 初期状態をランダムに作る．
  mCandList = var_list;
  for (ymuint i = 0; i < width; ++ i) {
    mCurState[i] = choose_var(rg1);
  }
  mCurVal = calc_val(rv_list, mCurState);
}

// @brief 次の状態に遷移する．
// @param[in] rv_list 登録ベクタのリスト
void
SigFuncGen::FuncState::next_move(const vector<const RegVect*>& rv_list,
				 RandGen& rg1,
				 RandGen& rg2)
{
  vector<Variable> new_state = mCurState;
  // 変更する位置を選ぶ．
  ymuint pos = rg1.int32() % mCurState.size();
  Variable old_var = new_state[pos];
  Variable new_var = choose_var(rg1);
  new_state[pos] = new_var;
  double new_val = calc_val(rv_list, new_state);
  if ( new_val < mCurVal ) {
    // 価値が減っていたら価値に基づいたランダム判定を行う．
    double ratio = new_val / mCurVal;
    double r = rg2.real1();
    if ( r < ratio ) {
      // 棄却する．
      mCandList.push_back(new_var);
      return;
    }
  }
  // ここに来たということは受容された．
  mCandList.push_back(old_var);
  mCurState = new_state;
  mCurVal = new_val;
}

// @brief 現在の状態から SigFunc を生成する．
SigFunc*
SigFuncGen::FuncState::new_func() const
{
  return new SigFunc(mCurState);
}

// @brief 変数をランダムに選ぶ．
// @param[in] rg 乱数発生器
//
// 重複を避けるために選ばれた変数は mCandList から取り除かれる．
Variable
SigFuncGen::FuncState::choose_var(RandGen& rg)
{
  ymuint n = mCandList.size();
  ymuint pos = rg.int32() % n;
  Variable ans = mCandList[pos];
  mCandList[pos] = mCandList[n - 1];
  mCandList.erase(mCandList.end() - 1, mCandList.end());
  return ans;
}

END_NAMESPACE_YM_IGF
