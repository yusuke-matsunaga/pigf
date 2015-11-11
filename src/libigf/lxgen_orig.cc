
/// @file lexgen.cc
/// @brief lexgen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "VarHeap.h"


BEGIN_NAMESPACE_YM_IGF

ymuint
calc_minval(const Variable& var1,
	    const VarHeap& var_set,
	    const vector<const RegVect*>& v_list)
{
  ymuint n00 = 0;
  ymuint n01 = 0;
  ymuint n10 = 0;
  ymuint n11 = 0;
  for (ymuint j = 0; j < var_set.size(); ++ j) {
    const Variable& var2 = var_set.var(j);
    for (vector<const RegVect*>::const_iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      const RegVect* rv = *p;
      if ( rv->classify(var1) ) {
	if ( rv->classify(var2) ) {
	  ++ n11;
	}
	else {
	  ++ n10;
	}
      }
      else {
	if ( rv->classify(var2) ) {
	  ++ n01;
	}
	else {
	  ++ n00;
	}
      }
    }
  }
  ymuint min_n = n00;
  if ( min_n > n01 ) {
    min_n = n01;
  }
  if ( min_n > n10 ) {
    min_n = n10;
  }
  if ( min_n > n11 ) {
    min_n = n11;
  }
  return min_n;
}

void
lxgen_orig(RvMgr& rv_mgr,
	   vector<Variable>& var_list)
{
  ymuint ni = rv_mgr.vect_size();
  VarHeap var_set(ni);
  const vector<const RegVect*>& v_list = rv_mgr.vect_list();
  for (ymuint i = 0; i < ni; ++ i) {
    Variable var1(ni, i);
    ymuint n0 = 0;
    ymuint n1 = 0;
    for (vector<const RegVect*>::const_iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      const RegVect* rv = *p;
      if ( rv->classify(var1) ) {
	++ n1;
      }
      else {
	++ n0;
      }
    }
    if ( n0 > 0 && n1 > 0 ) {
      ymuint n2 = n0 * n1;
      var_set.put(var1, n2);
    }
  }

  for ( ; ; ) {
    ymuint n_old = var_set.value(0);
    const Variable& var_old = var_set.var(0);
    ymuint max_n = n_old + 1;
    vector<Variable> max_vars;
    for (ymuint i = 0; i < ni; ++ i) {
      Variable var1(ni, i);
      if ( var1 && var_old ) {
	continue;
      }

      var1 *= var_old;

      bool found = false;
      for (ymuint j = 0; j < var_set.size(); ++ j) {
	const Variable& var2 = var_set.var(j);
	if ( var1 == var2 ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	continue;
      }

      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<const RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	const RegVect* rv = *p;
	if ( rv->classify(var1) ) {
	  ++ n1;
	}
	else {
	  ++ n0;
	}
      }
      ymuint n2 = n0 * n1;
      if ( max_n < n2 ) {
	max_n = n2;
	max_vars.clear();
	max_vars.push_back(var1);
      }
      else if ( max_n == n2 ) {
	max_vars.push_back(var1);
      }
    }
    if ( max_vars.empty() ) {
      break;
    }

    Variable max_var = max_vars[0];
    ymuint n = max_vars.size();
    if ( n > 1 ) {
      ymuint max_min_n = calc_minval(max_var, var_set, v_list);
      for (ymuint i = 1; i < n; ++ i) {
	const Variable& var1 = max_vars[i];
	ymuint min_n = calc_minval(var1, var_set, v_list);
	if ( max_min_n < min_n ) {
	  max_min_n = min_n;
	  max_var = var1;
	}
      }
      var_set.pop_min();
      var_set.put(max_var, max_n);
    }
  }

  var_list.clear();
  for (ymuint i = 0; i < var_set.size(); ++ i) {
    var_list.push_back(var_set.var(i));
  }
}

END_NAMESPACE_YM_IGF
