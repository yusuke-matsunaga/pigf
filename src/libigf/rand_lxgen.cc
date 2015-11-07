
/// @file lxgen.cc
/// @brief lxgen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

Variable
choose_var(vector<Variable>& var_list,
	   RandGen& rg)
{
  ymuint n = var_list.size();
  ymuint pos = rg.int32() % n;
  Variable var = var_list[pos];
  const Variable& last_var = var_list[n - 1];
  var_list[pos] = last_var;
  var_list.erase(var_list.end() - 1, var_list.end());
  return var;
}

END_NONAMESPACE

void
rand_lxgen(RvMgr& rv_mgr,
	   RandGen& rg,
	   ymuint n,
	   vector<Variable>& ans_list)
{
  // 初期変数集合を作る．
  // 基本的にはすべての変数が対象だが，
  // 登録ベクタを区別しない変数は取り除く．
  ymuint ni = rv_mgr.vect_size();
  vector<Variable> var_list;
  var_list.reserve(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    Variable var1(ni, i);
    // この変数の価値を求める．
    double val = rv_mgr.value(var1);
    if ( val > 0 ) {
      // この変数をリストに積む．
      var_list.push_back(var1);
    }
  }

  // n 個の変数を生成する．
  ans_list.clear();
  ans_list.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    vector<Variable> tmp_list(var_list);

    // シードとなる変数をランダムに選ぶ．
    Variable var1 = choose_var(tmp_list, rg);

    // ランダムに変数を足していって価値の最も高いものを返す．
    double max_val = rv_mgr.value(var1);
    Variable max_var = var1;

    while ( !tmp_list.empty() ) {
      cout << " current max = " << max_val << endl;
      Variable var2 = choose_var(tmp_list, rg);
      var1 *= var2;
      double val = rv_mgr.value(var1);
      cout << "  new_val = " << val << endl;
      if ( max_val < val ) {
	max_val = val;
	max_var = var1;
      }
    }
    ans_list[i] = max_var;
  }

  cout << endl;
  cout << "After linear transformation" << endl;

  for (ymuint i = 0; i < n; ++ i) {
    const Variable& var1 = ans_list[i];
    double val = rv_mgr.value(var1);
    cout << "Var#" << i << ": ";
    var1.dump(cout);
    cout << "  " << val << endl;
  }
}

END_NAMESPACE_YM_IGF
