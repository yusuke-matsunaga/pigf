
/// @file lxgen.cc
/// @brief lxgen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "VarHeap.h"


BEGIN_NAMESPACE_YM_IGF

void
lxgen(RvMgr& rv_mgr,
      vector<Variable*>& var_list)
{
  // 初期変数集合を作る．
  // 基本的にはすべての変数が対象だが，
  // 登録ベクタを区別しない変数は取り除く．
  ymuint ni = rv_mgr.vect_size();
  VarHeap var_set(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    Variable* var1 = new Variable(ni, i);
    // この変数の価値を求める．
    double val = rv_mgr.value(var1);
    if ( val > 0 ) {
      // 価値をキーにしてヒープ木に積む．
      var_set.put(var1, val);

      cout << "Var#" << i << ": ";
      var1->dump(cout);
      cout << "  " << val << endl;
    }
    else {
      // この変数は登録ベクタを区別しない．
      delete var1;
    }
  }

  // 変化がある限り改善ループを繰り返す．
  for ( ; ; ) {
    // 最も価値の低い変数を取り出す．
    double val_old = var_set.value(0);
    Variable* var_old = var_set.var(0);
    vector<ymuint> vid_list0 = var_old->vid_list();

    // 他の変数と組み合わせて新しい合成変数を作る．
    double max_val = val_old;
    vector<Variable*> max_vars;
#if 1
    for (ymuint i = 1; i < var_set.size(); ++ i) {
      Variable* var1 = var_set.var(i);
      Variable* new_var = new Variable(*var_old * *var1);
#else
    vector<bool> used(ni, false);
    for (vector<ymuint>::const_iterator p = vid_list0.begin();
	 p != vid_list0.end(); ++ p) {
      used[*p] = true;
    }

    for (ymuint i = 0; i < ni; ++ i) {
      if ( used[i] ) {
	continue;
      }

      Variable* new_var = new Variable(ni, i);
      (*new_var) *= (*var_old);
#endif
#if 1
      bool found = false;
      for (ymuint j = 1; j < var_set.size(); ++ j) {
	Variable* var1 = var_set.var(j);
	if ( *var1 == *new_var ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	delete new_var;
	continue;
      }
#endif

      double val = rv_mgr.value(new_var);

      if ( max_val < val ) {
	max_val = val;
	for (vector<Variable*>::iterator p = max_vars.begin();
	     p != max_vars.end(); ++ p) {
	  delete *p;
	}
	max_vars.clear();
	max_vars.push_back(new_var);
      }
      else if ( max_val == val ) {
	max_vars.push_back(new_var);
      }
      else {
	delete new_var;
      }
    }

    if ( max_vars.empty() ) {
      // 価値を改善する変数を作れなかった．
      break;
    }

    Variable* max_var = NULL;
    if ( max_vars.size() == 1 ) {
      max_var = max_vars[0];
    }
    else {
      // タイブレークは他の変数と組み合わせた時の区別できるベクタ数の最小値が最大のもの
      double max_minval = 0;
      for (vector<Variable*>::iterator p = max_vars.begin();
	   p != max_vars.end(); ++ p) {
	Variable* var1 = *p;
	double minval = DBL_MAX;
	for (ymuint j = 1; j < var_set.size(); ++ j) {
	  Variable* var2 = var_set.var(j);
	  double val = rv_mgr.value(var1, var2);
	  if ( minval > val ) {
	    minval = val;
	  }
	}
	if ( max_var == nullptr || max_minval < minval ) {
	  max_minval = minval;
	  delete max_var;
	  max_var = var1;
	}
	else {
	  delete var1;
	}
      }
    }
    var_set.get_min();
    var_set.put(max_var, max_val);
  }

  cout << endl;
  cout << "After linear transformation" << endl;

  var_list.clear();
  var_list.reserve(var_set.size());
  for (ymuint i = 0; i < var_set.size(); ++ i) {
    Variable* var1 = var_set.var(i);
    var_list.push_back(var1);

    double val =var_set.value(i);
    cout << "Var#" << i << ": ";
    var1->dump(cout);
    cout << "  " << val << endl;
  }
}

END_NAMESPACE_YM_IGF
