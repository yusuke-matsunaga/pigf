
/// @file MH_BaseGen.cc
/// @brief MH_BaseGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MH_BaseGen.h"
#include "MhSampler.h"
#include "BaseState.h"
#include "RegVect.h"
#include "VarPool.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス MH_BaseGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MH_BaseGen::MH_BaseGen()
{
}

// @brief デストラクタ
MH_BaseGen::~MH_BaseGen()
{
}

// @brief 合成変数の生成を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] req_num 生成する変数の数
// @param[out] var_list 生成された変数を格納するリスト
void
MH_BaseGen::generate(const vector<const RegVect*>& rv_list,
		   ymuint req_num,
		   vector<Variable>& var_list)
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

  vector<Variable> primary_list;
  primary_list.reserve(var_num);
  for (ymuint i = 0; i < var_num; ++ i) {
    Variable var1(var_num, i);
    double v = var1.value(rv_list);
    if ( v > 0.0 ) {
      primary_list.push_back(var1);
    }
  }

  // 結果の変数を貯めておくオブジェクト
  VarPool var_pool(req_num);

  // 合成変数を表すオブジェクト
  BaseState state(primary_list, rv_list, var_pool);

  // Metropolis Hastings 法を用いたランダムサンプラー
  MhSampler sampler;

  // サンプリングを行う．
  // 結果は var_pool に格納される．
  sampler.sampling(state, 0, 0, req_num * 2);

  // var_pool の結果を var_list に移す．
  var_list.clear();
  var_list.reserve(var_pool.size());
  for (ymuint i = 0; i < var_pool.size(); ++ i) {
    var_list.push_back(var_pool.var(i));
  }
}

END_NAMESPACE_IGF
