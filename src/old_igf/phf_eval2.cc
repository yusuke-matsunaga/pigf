﻿
/// @file eval.cc
/// @brief 複数の IGU 構成法の比較を行うプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "FuncVect.h"
#include "PhfGen.h"
#include "RandHashGen.h"
#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

int
phf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");
  app.add_option(&popt_xor);

  // count オプション
  PoptUint popt_count("count", 'c',
		      "specify loop limit", "<INT>");
  app.add_option(&popt_count);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 comp = 1;
  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }

  ymuint32 count_limit = 1000;
  if ( popt_count.is_specified() ) {
    count_limit = popt_count.val();
  }

  vector<string> args;
  ymuint n_args = app.get_args(args);

  if ( n_args != 1 ) {
    app.usage(2);
  }

  string base = args[0];
  for (ymuint num = 4500; num <= 10000; num += 500) {
    ymuint p1_sum = 0;
    ymuint mem1_sum = 0;
    ymuint p2_sum = 0;
    ymuint mem2_sum = 0;
    for (ymuint i = 0; i < 20; ++ i) {
      ostringstream buf;
      buf << base << "-" << num << "-" << i;
      const char* f_str = buf.str().c_str();
      cerr << "file = " << f_str << endl;
      ifstream ifs(f_str);
      if ( !ifs ) {
	cerr << f_str << ": No such file" << endl;
	return 2;
      }

      RvMgr rvmgr;

      bool rstat = rvmgr.read_data(ifs);
      if ( !rstat ) {
	cerr << "read error" << endl;
	return 3;
      }

      ymuint n = rvmgr.vect_size();

      RandHashGen rhg;

      ymuint p = rvmgr.index_size();
      for (bool found = false; !found ; ++ p) {
	for (ymuint count = 0; count < count_limit; ++ count) {
	  const vector<const RegVect*>& vlist = rvmgr.vect_list();
	  ymuint nv = vlist.size();
	  ymuint exp_p = 1U << p;

	  vector<const FuncVect*> func_list(2);
	  for (ymuint i = 0; i < 2; ++ i) {
	    InputFunc* f = rhg.gen_func(n, p, comp);
	    FuncVect* fv = new FuncVect(exp_p, nv);
	    for (ymuint v = 0; v < nv; ++ v) {
	      const RegVect* rv = vlist[v];
	      fv->set_val(v, f->eval(rv));
	    }
	    delete f;
	    func_list[i] = fv;
	  }

	  vector<ymuint> displace_map(nv);
	  PhfGen pg;
	  bool stat = pg.displace_decomposition(func_list[0], func_list[1], displace_map, false);
	  if ( stat ) {
	    found = true;
	    ymuint q = rvmgr.index_size();
	    ymuint exp_q = 1U << q;
	    ymuint mem = exp_p * (p + q) + exp_q * n;
	    p1_sum += p;
	    mem1_sum += mem;
	    cerr << "p1 = " << p << ", mem1 = " << mem << endl;
	  }
	  delete func_list[0];
	  delete func_list[1];
	  if ( stat ) {
	    break;
	  }
	}
      }

      p = rvmgr.index_size();
      for (bool found = false; !found ; ++ p) {
	for (ymuint count = 0; count < count_limit; ++ count) {
	  const vector<const RegVect*>& vlist = rvmgr.vect_list();
	  ymuint nv = vlist.size();
	  ymuint exp_p = 1U << p;

	  vector<const FuncVect*> func_list(2);
	  for (ymuint i = 0; i < 2; ++ i) {
	    InputFunc* f = rhg.gen_func(n, p, comp);
	    FuncVect* fv = new FuncVect(exp_p, nv);
	    func_list[i] = fv;
	    for (ymuint v = 0; v < nv; ++ v) {
	      const RegVect* rv = vlist[v];
	      fv->set_val(v, f->eval(rv));
	    }
	    delete f;
	  }

	  vector<ymuint> displace_map(nv);
	  PhfGen pg;
	  bool stat = pg.displace_decomposition(func_list[0], func_list[1], displace_map, true);
	  if ( stat ) {
	    found = true;
	    ymuint q = rvmgr.index_size();
	    ymuint exp_q = 1U << q;
	    ymuint mem = exp_p * (p + q) + exp_q * n;
	    p2_sum += p;
	    mem2_sum += mem;
	    cerr << "p2 = " << p << ", mem2 = " << mem << endl;
	  }
	  for (ymuint i = 0; i < 2; ++ i) {
	    delete func_list[i];
	  }
	  if ( stat ) {
	    break;
	  }
	}
      }
    }
    cout << num;
    double p1_ave = p1_sum / 20.0;
    double mem1_ave = mem1_sum / 20.0;
    double p2_ave = p2_sum / 20.0;
    double mem2_ave = mem2_sum / 20.0;
    cout << " " << p1_ave << " " << mem1_ave << " " << p2_ave << " " << mem2_ave;
    cout << endl;
  }

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::phf;

  return phf(argc, argv);
}
