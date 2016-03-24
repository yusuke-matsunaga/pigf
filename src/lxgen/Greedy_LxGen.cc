
/// @file Greedy_LxGen.cc
/// @brief Greedy_LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Greedy_LxGen.h"
#include "RegVect.h"
#include "Variable.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス Greedy_LxGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Greedy_LxGen::Greedy_LxGen()
{
}

// @brief デストラクタ
Greedy_LxGen::~Greedy_LxGen()
{
}

// @brief 合成変数の生成を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] req_num 出力する合成変数の数
// @param[out] var_list 結果の変数を入れるリスト
//
// var_list の内容の Variable は呼んだ側で解放すること．
void
Greedy_LxGen::generate(const vector<const RegVect*>& rv_list,
		    ymuint req_num,
		    vector<Variable>& var_list)
{
  // 初期変数集合を作る．
  vector<Variable> pvar_list;
  get_primary_vars(rv_list, pvar_list);

  // 最初のシードをランダムに作り，
  // 評価値の最も高くなる変数と合成してゆく．
  var_list.clear();
  var_list.reserve(req_num);
  for (ymuint i = 0; i < req_num; ++ i) {
    // var_list の中からランダムに選ぶ．
    // といっても choose_var() を呼ぶとその変数は
    // リストから取り除かれるのでコピーを使う．
    vector<Variable> tmp_list(pvar_list);

    // シードとなる変数をランダムに選ぶ．
    Variable var1 = choose_var(tmp_list);

    // ランダムに変数を足していって価値の最も高いものを返す．
    // いわゆる山登り法
    double max_val = var1.value(rv_list);
    Variable max_var = var1;
    while ( !tmp_list.empty() ) {
      Variable var2 = choose_var(tmp_list);
      var1 *= var2;
      double val = var1.value(rv_list);
      if ( max_val < val ) {
	max_val = val;
	max_var = var1;
      }
    }
    var_list.push_back(max_var);
  }
}

// @brief var_list の中からランダムに変数を選ぶ．
// @param[inout] var_list 変数のリスト
//
// 選ばれた変数は var_list から取り除かれる．
Variable
Greedy_LxGen::choose_var(vector<Variable>& var_list)
{
  ymuint n = var_list.size();
  ymuint pos = mRandGen.int32() % n;
  Variable var = var_list[pos];
  const Variable& last_var = var_list[n - 1];
  var_list[pos] = last_var;
  var_list.erase(var_list.end() - 1, var_list.end());
  return var;
}

END_NAMESPACE_IGF
