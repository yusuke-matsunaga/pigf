
/// @file MCMC_BaseGen.cc
/// @brief MCMC_BaseGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MCMC_BaseGen.h"
#include "RegVect.h"
#include "VarPool.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス MCMC_BaseGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MCMC_BaseGen::MCMC_BaseGen()
{
}

// @brief デストラクタ
MCMC_BaseGen::~MCMC_BaseGen()
{
}

// @brief 合成変数の生成を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] req_num 生成する変数の数
// @param[out] var_list 生成された変数を格納するリスト
void
MCMC_BaseGen::generate(const vector<const RegVect*>& rv_list,
		     ymuint req_num,
		     vector<Variable>& var_list)
{
  init(rv_list);

  VarPool var_pool(req_num);
  for (ymuint i = 0; i < req_num * 5; ++ i) {
    next_move();
    var_pool.put(mCurState, mCurVal);
  }

  var_list.clear();
  var_list.reserve(var_pool.size());
  for (ymuint i = 0; i < var_pool.size(); ++ i) {
    var_list.push_back(var_pool.var(i));
  }
}

// @brief 初期化を行う．
// @param[in] rv_list 登録ベクタのリスト
void
MCMC_BaseGen::init(const vector<const RegVect*>& rv_list)
{
  // 初期変数集合を作る．
  // 基本的にはすべての変数が対象だが，
  // 登録ベクタを区別しない変数は取り除く．
  ymuint var_num = 0;
  {
    ASSERT_COND( !rv_list.empty() );
    const RegVect* rv0 = rv_list[0];
    var_num = rv0->size();
  }

  mRvList = rv_list;

  mPrimaryList.clear();
  mPrimaryList.reserve(var_num);
  for (ymuint i = 0; i < var_num; ++ i) {
    Variable var1(var_num, i);
    double v = var1.value(mRvList);
    if ( v > 0.0 ) {
      mPrimaryList.push_back(var1);
    }
  }

  // 初期解を作る．
  ymuint vid = mRgMove.int32() % var_num;
  mCurState = Variable(var_num, vid);
  mCurVal = value(mCurState);
}

// @brief 次の状態に遷移する．
void
MCMC_BaseGen::next_move()
{
  // mPrimaryList の中からランダムに選ぶ．
  ymuint pos = mRgMove.int32() % mPrimaryList.size();
  const Variable& var1 = mPrimaryList[pos];
  // 今の変数と合成する．
  Variable new_var = mCurState * var1;
  double new_val = value(new_var);
  if ( new_val < mCurVal ) {
    // 価値が減っていたら価値に基づいたランダム判定を行う．
    double ratio = new_val / mCurVal;
    double r = mRgAccept.real1();
    if ( r > ratio ) {
      // 棄却する．
      return;
    }
  }
  // ここに来たということは受容された．
  mCurState = new_var;
  mCurVal = new_val;
}

// @brief 変数の価値を計算する．
double
MCMC_BaseGen::value(const Variable& var)
{
  return var.value(mRvList);
}

// @brief 登録ベクタのリストを返す．
const vector<const RegVect*>&
MCMC_BaseGen::rv_list() const
{
  return mRvList;
}

END_NAMESPACE_IGF
