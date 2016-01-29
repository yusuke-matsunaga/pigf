
/// @file LxGen.cc
/// @brief LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LxGen.h"
#include "RegVect.h"
#include "VarPool.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス LxGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LxGen::LxGen()
{
}

// @brief デストラクタ
LxGen::~LxGen()
{
}

// @brief 初期化を行う．
// @param[in] rv_list 登録ベクタのリスト
void
LxGen::init(const vector<const RegVect*>& rv_list)
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
  mCurVal = mCurState.value(mRvList);
}

// @brief 合成変数の生成を行う．
Variable
LxGen::generate(double& val)
{
  next_move();
  val = mCurVal;
  return mCurState;
}

// @brief 次の状態に遷移する．
void
LxGen::next_move()
{
  // mPrimaryList の中からランダムに選ぶ．
  ymuint pos = mRgMove.int32() % mPrimaryList.size();
  const Variable& var1 = mPrimaryList[pos];
  // 今の変数と合成する．
  Variable new_var = mCurState * var1;
  double new_val = new_var.value(mRvList);
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

END_NAMESPACE_YM_IGF
