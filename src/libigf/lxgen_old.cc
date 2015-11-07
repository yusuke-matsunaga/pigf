
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

void
lxgen_old(RvMgr& rv_mgr,
	  vector<Variable*>& var_list)
{
  ymuint ni = rv_mgr.vect_size();
  VarHeap var_set(ni);
  const vector<const RegVect*>& v_list = rv_mgr.vect_list();
  for (ymuint i = 0; i < ni; ++ i) {
    Variable* var1 = new Variable(ni, i);
    double val = rv_mgr.value(var1);
    if ( val > 0 ) {
      var_set.put(var1, val);

      cout << "Var#" << i << ": " << val << endl;
    }
    else {
      delete var1;
    }
  }

  for ( ; ; ) {
    double n_old = var_set.value(0);
    Variable* var_old = var_set.var(0);
    vector<ymuint> vid_list0 = var_old->vid_list();

    cout << "Chooose ";
    var_old->dump(cout);
    cout << "  " << n_old << endl;

    vector<bool> used(ni, false);
    for (vector<ymuint>::const_iterator p = vid_list0.begin();
	 p != vid_list0.end(); ++ p) {
      ymuint pos = *p;
      ASSERT_COND( pos < ni );
      used[pos] = true;
    }

    double max_val = n_old;
    vector<Variable*> max_vars;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( used[i] ) {
	continue;
      }

      Variable* new_var = new Variable(ni, i);
      (*new_var) *= (*var_old);

      bool found = false;
      for (ymuint j = 1; j < var_set.size(); ++ j) {
	Variable* var1 = var_set.var(j);
	if ( *var1 == *new_var ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	cout << " duplicated" << endl;
	delete new_var;
	continue;
      }

      double val = rv_mgr.value(new_var);

      cout << " new var = ";
      new_var->dump(cout);
      cout << "  " << val << endl;

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
	delete new_var;;
      }
    }
    if ( max_vars.empty() ) {
      break;
    }

    Variable* max_var = NULL;
    if ( max_vars.size() == 1 ) {
      max_var = max_vars[0];
    }
    else {
      ymuint max_minval = 0;
      ymuint nv = rv_mgr.vect_list().size();
      for (vector<Variable*>::iterator p = max_vars.begin();
	   p != max_vars.end(); ++ p) {
	Variable* var1 = *p;
	ymuint minval = (nv * nv * 6) / 16;
	for (ymuint j = 1; j < var_set.size(); ++ j) {
	  Variable* var2 = var_set.var(j);
	  double val = rv_mgr.value(var1, var2);
	  if ( minval > val ) {
	    minval = val;
	  }
	}
	if ( max_var == NULL || max_minval < minval ) {
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
    {
      cout << "BEFORE" << endl;
      var_set.print(cout);
      cout << "max_var = ";
      max_var->dump(cout);
      cout << endl;
    }
    var_set.put(max_var, max_val);
    {
      cout << "AFTER" << endl;
      var_set.print(cout);
    }
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
