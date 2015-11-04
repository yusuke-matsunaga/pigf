
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
    Variable* var1 = new Variable(i);
    ymuint n0 = 0;
    ymuint n1 = 0;
    for (vector<const RegVect*>::const_iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      const RegVect* rv = *p;
      if ( var1->classify(rv) ) {
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
    else {
      delete var1;
    }
  }

  for ( ; ; ) {
    ymuint n_old = var_set.value(0);
    Variable* var_old = var_set.var(0);
    const vector<ymuint>& vid_list0 = var_old->vid_list();
    vector<bool> used(ni, false);
    for (vector<ymuint>::const_iterator p = vid_list0.begin();
	 p != vid_list0.end(); ++ p) {
      used[*p] = true;
    }
    ymuint max_n = n_old + 1;
    vector<Variable*> max_vars;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( used[i] ) {
	continue;
      }
      vector<ymuint> vid_list1(vid_list0);
      vid_list1.push_back(i);
      sort(vid_list1.begin(), vid_list1.end());
      bool found = false;
      for (ymuint j = 0; j < var_set.size(); ++ j) {
	Variable* var1 = var_set.var(j);
	if ( var1->vid_list() == vid_list1 ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	continue;
      }
      Variable* var1 = new Variable(vid_list1);
#if 0
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<const RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	const RegVect* rv = *p;
	if ( var1->classify(rv) ) {
	  ++ n1;
	}
	else {
	  ++ n0;
	}
      }
      ymuint n2 = n0 * n1;
#else
      ymuint n2 = rv_mgr.value(var1);
#endif
      if ( max_n < n2 ) {
	max_n = n2;
	for (vector<Variable*>::iterator p = max_vars.begin();
	     p != max_vars.end(); ++ p) {
	  delete *p;
	}
	max_vars.clear();
	max_vars.push_back(var1);
      }
      else if ( max_n == n2 ) {
	max_vars.push_back(var1);
      }
      else {
	delete var1;
      }
    }
    if ( !max_vars.empty() ) {
      ymuint max_min_n = 0;
      Variable* max_var = NULL;
      for (vector<Variable*>::iterator p = max_vars.begin();
	   p != max_vars.end(); ++ p) {
	Variable* var1 = *p;
	ymuint n00 = 0;
	ymuint n01 = 0;
	ymuint n10 = 0;
	ymuint n11 = 0;
	for (ymuint j = 0; j < var_set.size(); ++ j) {
	  Variable* var2 = var_set.var(j);
	  for (vector<const RegVect*>::const_iterator p = v_list.begin();
	       p != v_list.end(); ++ p) {
	    const RegVect* rv = *p;
	    if ( var1->classify(rv) ) {
	      if ( var2->classify(rv) ) {
		++ n11;
	      }
	      else {
		++ n10;
	      }
	    }
	    else {
	      if ( var2->classify(rv) ) {
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
	if ( max_min_n < min_n ) {
	  max_min_n = min_n;
	  delete max_var;
	  max_var = var1;
	}
      }
      var_set.get_min();
      var_set.put(max_var, max_n);
    }
    else {
      break;
    }
  }
  for (ymuint i = 0; i < var_set.size(); ++ i) {
    var_list.push_back(var_set.var(i));
  }
}

END_NAMESPACE_YM_IGF
