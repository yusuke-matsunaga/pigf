
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

#if 0
#include "RandHashGen.h"
#include "InputFunc.h"
#include "XorFunc.h"
#include "FuncVect.h"
#include "IguGen.h"
#include "YmUtils/PoptMainApp.h"
#include "YmUtils/RandCombiGen.h"
#endif

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
  const vector<const RegVect*>& v_list = rv_mgr.vect_list();
  for (ymuint i = 0; i < ni; ++ i) {
    Variable* var1 = new Variable(i);
    // この変数の価値を求める．
    ymuint val = rv_mgr.value(var1);
    if ( val > 0 ) {
      // 価値をキーにしてヒープ木に積む．
      var_set.put(var1, val);

      cout << "Var#" << i << ": " << val << endl;
    }
    else {
      // この変数は登録ベクタを区別しない．
      delete var1;
    }
  }

  // 変化がある限り改善ループを繰り返す．
  for ( ; ; ) {
    // 最も価値の低い変数を取り出す．
    ymuint val_old = var_set.value(0);
    Variable* var_old = var_set.var(0);
    const vector<ymuint>& vid_list0 = var_old->vid_list();

    cout << "Choose ";
    var_old->dump(cout);
    cout << endl;

    // 他の変数と組み合わせて新しい合成変数を作る．
    ymuint max_val = val_old + 1;
    vector<Variable*> max_vars;
    for (ymuint i = 1; i < var_set.size(); ++ i) {
      Variable* var1 = var_set.var(i);
      const vector<ymuint>& vid_list1 = var1->vid_list();

      cout << "  Choose ";
      var1->dump(cout);
      cout << endl;

      // vid_list0 と vid_list1 をマージする．
      // ただし，両方に現れていたら削除する．
      vector<ymuint> vid_list;
      ymuint n0 = vid_list0.size();
      ymuint n1 = vid_list1.size();
      vid_list.reserve(n0 + n1);
      ymuint i0 = 0;
      ymuint i1 = 0;
      while ( i0 < n0 && i1 < n1 ) {
	ymuint v0 = vid_list0[i0];
	ymuint v1 = vid_list1[i1];
	if ( v0 < v1 ) {
	  vid_list.push_back(v0);
	  ++ i0;
	}
	else if ( v0 > v1 ) {
	  vid_list.push_back(v1);
	  ++ i1;
	}
	else {
	  ++ i0;
	  ++ i1;
	}
      }
      for ( ; i0 < n0; ++ i0) {
	vid_list.push_back(vid_list0[i0]);
      }
      for ( ; i1 < n1; ++ i1) {
	vid_list.push_back(vid_list1[i1]);
      }
#if 0
      bool found = false;
      for (ymuint j = 1; j < var_set.size(); ++ j) {
	Variable* var1 = var_set.var(j);
	if ( var1->vid_list() == vid_list ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	continue;
      }
#endif

      Variable* new_var = new Variable(vid_list);
      ymuint val = rv_mgr.value(new_var);
      {
	cout << " new variable = ";
	new_var->dump(cout);
	cout << "  value = " << val << endl;

      }
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
      ymuint nv = rv_mgr.vect_list().size();
      ymuint max_minval = 0;
      for (vector<Variable*>::iterator p = max_vars.begin();
	   p != max_vars.end(); ++ p) {
	Variable* var1 = *p;
	ymuint minval = (nv * nv * 6) / 16;
	for (ymuint j = 1; j < var_set.size(); ++ j) {
	  Variable* var2 = var_set.var(j);
	  ymuint val = rv_mgr.value(var1, var2);
	  if ( minval > val ) {
	    minval = val;
	  }
	}
	if ( max_minval < minval ) {
	  max_minval = minval;
	  delete max_var;
	  max_var = var1;
	}
	else {
	  delete var1;
	}
      }
    }
    {
      cout << " max_var = ";
      max_var->dump(cout);
      cout << "  value = " << max_val << endl;
    }
    var_set.get_min();
    var_set.put(max_var, max_val);
  }

  cout << endl;
  cout << "After linear transformation" << endl;

  var_list.clear();
  var_list.reserve(var_set.size());
  for (ymuint i = 0; i < var_set.size(); ++ i) {
    var_list.push_back(var_set.var(i));

    cout << "Var#" << i << ": " << var_set.value(i) << endl;
  }
}

END_NAMESPACE_YM_IGF
