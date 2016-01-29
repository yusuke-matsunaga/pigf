
/// @file RandSigFuncGen.cc
/// @brief RandSigFuncGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "RandSigFuncGen.h"
#include "RegVect.h"
#include "SigFunc.h"
#include "Variable.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス RandSigFuncGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RandSigFuncGen::RandSigFuncGen()
{
  mRcg = nullptr;
}

// @brief デストラクタ
RandSigFuncGen::~RandSigFuncGen()
{
  delete mRcg;
}

// @brief 初期化を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] var_list 変数のリスト
// @param[in] width 出力のビット幅
// @param[in] m 多重度
void
RandSigFuncGen::init(const vector<const RegVect*>& rv_list,
		     const vector<Variable>& var_list,
		     ymuint width,
		     ymuint m)
{
  mVarList = var_list;
  mWidth = width;
  mM = m;

  delete mRcg;
  mRcg = new RandCombiGen(var_list.size(), width);
}

// @brief signature function を一つ生成する．
vector<const SigFunc*>
RandSigFuncGen::generate()
{
  vector<const SigFunc*> ans(mM);
  for (ymuint i = 0; i < mM; ++ i) {
    mRcg->generate(mRgChoose);
    vector<Variable> tmp_list(mWidth);
    for (ymuint j = 0; j < mWidth; ++ j) {
      tmp_list[j] = mVarList[mRcg->elem(j)];
    }
    ans[i] = new SigFunc(tmp_list);
  }
  return ans;
}

END_NAMESPACE_YM_IGF
