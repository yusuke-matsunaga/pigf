
/// @file igugen.cc
/// @brief igugen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "RandHashGen.h"
#include "SigFunc.h"
#include "IguGen.h"
#include "LxGen.h"
#include "RandLxGen.h"
#include "Partitioner.h"
#include "SigFuncGen.h"
#include "RandSigFuncGen.h"
#include "VarPool.h"
#include "YmUtils/PoptMainApp.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// 変数集合の価値を計算する．
double
calc_val(const vector<const RegVect*>& rv_list,
	 const SigFunc* sf)
{
  ymuint64 n = rv_list.size();
  ymuint nb = sf->output_width();
  ymuint np = 1U << nb;
  vector<ymuint> c_array(np, 0);

  for (ymuint i = 0; i < n; ++ i) {
    const RegVect* rv = rv_list[i];
    ymuint val = sf->eval(rv);
    ++ c_array[val];
  }

  double ave = static_cast<double>(n) / static_cast<double>(np);
  double sqsum = 0.0;
  for (ymuint i = 0; i < np; ++ i) {
    ymuint c1 = c_array[i];
    if ( c1 > ave ) {
      double diff = static_cast<double>(c1) - ave;
      sqsum += diff;
    }
  }
  double stdev = sqsum / n;
  return exp(- stdev);
}

END_NONAMESPACE

struct Lt
{
  bool
  operator()(const pair<ymuint, Variable*>& left,
	     const pair<ymuint, Variable*>& right)
  {
    return left.first > right.first;
  }
};

void
lxgen(RvMgr& rv_mgr,
      vector<Variable>& var_list);

void
lxgen_old(RvMgr& rv_mgr,
	  vector<Variable>& var_list);

void
lxgen_orig(RvMgr& rv_mgr,
	   vector<Variable>& var_list);

void
rand_lxgen(RvMgr& rv_mgr,
	   RandGen& rg,
	   ymuint n,
	   vector<Variable>& var_list);

int
igugen(int argc,
       const char** argv)
{
  PoptMainApp main_app;

  // m オプション
  PoptUint popt_m("mult", 'm',
		  "specify the number of hash functions", "<INT>");
  main_app.add_option(&popt_m);

  // n オプション
  PoptNone popt_n("naive", 'n',
		  "naive parallel sieve method");
  main_app.add_option(&popt_n);

  // x オプション
  PoptNone popt_x("xor", 'x', "linear transformation");
  main_app.add_option(&popt_x);

  // x オプション
  PoptNone popt_x2("xor_old", 'X', "linear transformation(old)");
  main_app.add_option(&popt_x2);

  // y オプション
  PoptNone popt_y("rand_xor", 'y', "linear transformation(random)");
  main_app.add_option(&popt_y);

  // z オプション
  PoptNone popt_z("rand_xor", 'z', "linear transformation(original)");
  main_app.add_option(&popt_z);

  // c オプション
  PoptUint popt_c("compose", 'c',
		  "compose mode", "max_degree");
  main_app.add_option(&popt_c);

  // l オプション
  PoptUint popt_l("count_limit", 'l',
		  "specify count limit", "<INT>");
  main_app.add_option(&popt_l);

  main_app.set_other_option_help("<filename>");

  tPoptStat stat = main_app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return 1;
  }

  vector<string> args;
  ymuint n_args = main_app.get_args(args);
  if ( n_args != 1 ) {
    main_app.usage(1);
  }

  RvMgr rv_mgr;

  ifstream ifs(args[0]);
  if ( !ifs ) {
    cerr << "Could not open " << args[0] << endl;
    return 1;
  }

  if ( !rv_mgr.read_data(ifs) ) {
    cerr << "Error in reading " << args[0] << endl;
    return 1;
  }

  cout << "# of inputs:     " << rv_mgr.vect_size() << endl
       << "# of vectors:    " << rv_mgr.vect_list().size() << endl
       << "# of index bits: " << rv_mgr.index_size() << endl;

  vector<Variable> var_list;
  if ( popt_x.is_specified() ) {
    LxGen lxgen;
    lxgen.init(rv_mgr.vect_list());
    VarPool var_pool(1000);
    for (ymuint i = 0; i < 10000; ++ i) {
      double val;
      Variable var = lxgen.generate(val);
      var_pool.put(var, val);
    }
    var_list.clear();
    var_list.reserve(var_pool.size());
    for (ymuint i = 0; i < var_pool.size(); ++ i) {
      var_list.push_back(var_pool.var(i));
    }
  }
  else if ( popt_x2.is_specified() ) {
    lxgen_old(rv_mgr, var_list);
  }
  else if ( popt_y.is_specified() ) {
#if 0
    RandGen rg;
    ymuint ni = rv_mgr.vect_size();
    rand_lxgen(rv_mgr, rg, ni, var_list);
#else
    RandLxGen lxgen;
    lxgen.generate(rv_mgr.vect_list(), 1000, var_list);
#endif
  }
  else if ( popt_z.is_specified() ) {
    RandGen rg;
    ymuint ni = rv_mgr.vect_size();
    lxgen_orig(rv_mgr, var_list);
  }
  else {
    ymuint ni = rv_mgr.vect_size();
    for (ymuint i = 0; i < ni; ++ i) {
      Variable var1(ni, i);
      double val = var1.value(rv_mgr.vect_list());
      if ( val > 0.0 ) {
	var_list.push_back(var1);
      }
    }
  }

  cout << "Phase-1 end (" << var_list.size() << ")" << endl;

  ymuint comp = 1;
  if ( popt_c.is_specified() ) {
    comp = popt_c.val();
  }

  ymuint m = 1;
  if ( popt_m.is_specified() ) {
    m = popt_m.val();
  }

  bool naive = popt_n.is_specified();

  ymuint count_limit = 1000;
  if ( popt_l.is_specified() ) {
    count_limit = popt_l.val();
  }

  ymuint n = rv_mgr.vect_size();
  ymuint p = rv_mgr.index_size();

  ymuint p1 = p;
  {
    for (ymuint tmp_m = 1; tmp_m < m; ) {
      -- p1;
      tmp_m <<= 1;
    }
  }

  Partitioner pt;
  const vector<const RegVect*>& vect_list = rv_mgr.vect_list();
  for ( ; ; ++ p1) {
    cout << " trying p = " << p1 << endl;
    bool found = false;
#if 0
    SigFuncGen sfgen;
    sfgen.init(vect_list, var_list, p1, m, 0, 1);
#else
    RandSigFuncGen sfgen;
    sfgen.init(vect_list, var_list, p1, m);
#endif

    for (ymuint c = 0; !found && c < count_limit; ++ c) {
      cout << "\r  " << setw(10) << c << " / " << count_limit;
      cout.flush();
      vector<const SigFunc*> sigfunc_list = sfgen.generate();
      {
	for (ymuint i = 0; i < m; ++ i) {
	  const SigFunc* sf = sigfunc_list[i];
	  double val = calc_val(vect_list, sf);
	  cout << " " << setw(10) << val;
	}
      }

      vector<ymuint> block_map;
      bool stat = pt.cf_partition(vect_list, sigfunc_list, block_map);
      if ( stat ) {
	found = true;
	// 検証する．
	ymuint np = 1U << p1;
	vector<vector<ymuint> > rmap(m);
	for (ymuint i = 0; i < m; ++ i) {
	  rmap[i].resize(np, false);
	}
	for (ymuint i = 0; i < vect_list.size(); ++ i) {
	  const RegVect* rv = vect_list[i];
	  ymuint bid = block_map[i];
	  ymuint idx = sigfunc_list[bid]->eval(rv);
	  if ( rmap[bid][idx] ) {
	    cerr << "Error!: conflicts" << endl;
	  }
	  rmap[bid][idx] = true;
	}
      }

      for (ymuint i = 0; i < m; ++ i) {
	delete sigfunc_list[i];
      }
    }
    cout << endl;

    if ( found ) {
      break;
    }
  }

  ymuint exp_p = 1U << p1;
  ymuint q = rv_mgr.index_size();
  cout << " p = " << p1 << endl
       << "Total memory size = "
       << (exp_p * (q + n - p1) * m) << endl;

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  return nsYm::nsIgf::igugen(argc, argv);
}
