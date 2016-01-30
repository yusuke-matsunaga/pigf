
/// @file SigFuncGen.cc
/// @brief SigFuncGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SigFuncGen.h"
#include "BasisChecker.h"
#include "RegVect.h"
#include "SigFunc.h"
#include "Variable.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// 変数集合の価値を計算する．
double
calc_val(const vector<const RegVect*>& rv_list,
	 const vector<Variable>& var_list)
{
  return 0.5;
  ymuint64 n = rv_list.size();
  ymuint nb = var_list.size();
  ymuint np = 1U << nb;
  vector<ymuint> c_array(np, 0);

  for (ymuint i = 0; i < n; ++ i) {
    const RegVect* rv = rv_list[i];
    ymuint val = 0U;
    for (ymuint j = 0; j < nb; ++ j) {
      const Variable& var = var_list[j];
      if ( rv->classify(var) ) {
	val |= (1U << j);
      }
    }
    ++ c_array[val];
  }

  double ave = static_cast<double>(n) / static_cast<double>(np);
  double sqsum = 0.0;
  for (ymuint i = 0; i < np; ++ i) {
    ymuint c1 = c_array[i];
    if ( c1 > ave ) {
      double diff = static_cast<double>(c1) - ave;
      sqsum += diff;
    }
  }
  double stdev = sqsum / n;
  return exp(- stdev);
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
#if 0
  // 独立に mM 個の処理を行う．
  for (ymuint i = 0; i < mM; ++ i) {
    mCurStateArray[i].next_move(mRvList, mRgChoose, mRgAccept);
  }
#else
  ymuint pos = mRgChoose.int32() % mM;
  mCurStateArray[pos].next_move(mRvList, mRgChoose, mRgAccept);
#endif
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
  // 候補リストを作る．
#if 0
  vector<Variable> tmp_list = var_list;
  ymuint nv = rv_list[0]->size();
  mCandList.clear();
  mCandList.reserve(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    for ( ; ; ) {
      ymuint n = tmp_list.size();
      ASSERT_COND( n > 0 );
      ymuint pos = rg1.int32() % n;
      Variable var = tmp_list[pos];
      tmp_list[pos] = tmp_list[n - 1];
      tmp_list.erase(tmp_list.end() - 1, tmp_list.end());
      mCandList.push_back(var);
      BasisChecker bc;
      if ( bc.check(mCandList) ) {
	break;
      }
      mCandList.pop_back();
    }
  }
#else
  mCandList = var_list;
#endif

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
