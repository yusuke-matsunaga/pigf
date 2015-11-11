
/// @file lxgen_old.cc
/// @brief lxgen_old の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.



#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "VarHeap.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

double
calc_minval(const Variable& var1,
	    const VarHeap& var_set,
	    const RvMgr& rv_mgr)
{
  double minval = DBL_MAX;
  for (ymuint i = 1; i < var_set.size(); ++ i) {
    const Variable& var2 = var_set.var(i);
    double val = rv_mgr.value(var1, var2);
    if ( minval > val ) {
      minval = val;
    }
  }
  return minval;
}

END_NONAMESPACE

void
lxgen_old(RvMgr& rv_mgr,
	  vector<Variable>& var_list)
{
  ymuint ni = rv_mgr.vect_size();
  VarHeap var_set(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    Variable var1(ni, i);
    double val = rv_mgr.value(var1);
    if ( val > 0 ) {
      var_set.put(var1, val);

      cout << "Var#" << i << ": " << val << ": " << var1 << endl;
    }
  }

  for ( ; ; ) {
    double n_old = var_set.value(0);
    const Variable& var_old = var_set.var(0);
    vector<ymuint> vid_list0 = var_old.vid_list();

    double max_val = n_old;
    vector<Variable> max_vars;
    for (ymuint i = 0; i < ni; ++ i) {
      Variable new_var(ni, i);

      if ( new_var && var_old ) {
	continue;
      }

      new_var *= var_old;

      bool found = false;
      for (ymuint j = 1; j < var_set.size(); ++ j) {
	const Variable& var1 = var_set.var(j);
	if ( var1 == new_var ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	continue;
      }

      double val = rv_mgr.value(new_var);

      if ( max_val < val ) {
	max_val = val;
	max_vars.clear();
	max_vars.push_back(new_var);
      }
      else if ( max_val == val ) {
	max_vars.push_back(new_var);
      }
    }
    if ( max_vars.empty() ) {
      break;
    }

    ymuint n = max_vars.size();
    Variable max_var = max_vars[0];
    if ( n > 1 ) {
      ymuint max_minval = calc_minval(max_var, var_set, rv_mgr);
      ymuint nv = rv_mgr.vect_list().size();
      for (vector<Variable>::iterator p = max_vars.begin();
	   p != max_vars.end(); ++ p) {
	const Variable& var1 = *p;
	ymuint minval = calc_minval(var1, var_set, rv_mgr);
	if ( max_minval < minval ) {
	  max_minval = minval;
	  max_var = var1;
	}
      }
    }
    var_set.pop_min();
    var_set.put(max_var, max_val);
  }

  cout << endl;
  cout << "After linear transformation" << endl;

  var_list.clear();
  var_list.reserve(var_set.size());
  for (ymuint i = 0; i < var_set.size(); ++ i) {
    const Variable& var1 = var_set.var(i);
    var_list.push_back(var1);

    double val =var_set.value(i);
    cout << "Var#" << i << ": " << val << ": " << var1 << endl;
  }
}

END_NAMESPACE_YM_IGF
