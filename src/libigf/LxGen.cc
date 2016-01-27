
/// @file LxGen.cc
/// @brief LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LxGen.h"
#include "RegVect.h"
#include "VarHeap.h"


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

// @brief 合成変数の生成を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] output_size 出力する合成変数の数
// @param[out] var_list 結果の変数を入れるリスト
//
// var_list の内容の Variable は呼んだ側で解放すること．
void
LxGen::generate(const vector<const RegVect*>& rv_list,
		ymuint output_size,
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
  vector<Variable> pvar_list;
  var_list.reserve(var_num);
  for (ymuint i = 0; i < var_num; ++ i) {
    Variable var1(var_num, i);
    double v = var1.value(rv_list);
    if ( v > 0.0 ) {
      pvar_list.push_back(var1);
    }
  }

  // 変数を貯めておくヒープ
  VarHeap var_heap(output_size);

  // MCMC(MH法) で合成変数を生成する．
  // burn-in が必要なので output_size の2倍サンプリングを行う．
  ymuint ns = output_size * 2;
  // 初期解
  Variable cur_var(var_num, 0);
  double cur_val = cur_var.value(rv_list);
  for (ymuint i = 0; i < ns; ++ i) {
    // var_list の中からランダムに選ぶ．
    ymuint pos = mRgMove.int32() % pvar_list.size();
    const Variable& var1 = pvar_list[pos];
    // 今の変数と合成する．
    Variable new_var = cur_var * var1;
    double new_val = new_var.value(rv_list);
    if ( new_val < cur_val ) {
      // 価値が減っていたら価値に基づいたランダム判定を行う．
      double ratio = new_val / cur_val;
      double r = mRgAccept.real1();
      if ( r > ratio ) {
	// 棄却する．
	continue;
      }
    }
    // ここに来たということは受容された．
    var_heap.put(new_var, new_val);
    cur_var = new_var;
  }

  // 結果を var_list に入れる．
  var_list.clear();
  var_list.reserve(output_size);
  for (ymuint i = 0; i < var_heap.size(); ++ i) {
    const Variable& var = var_heap.var(i);
    var_list.push_back(var);
  }
}

END_NAMESPACE_YM_IGF
