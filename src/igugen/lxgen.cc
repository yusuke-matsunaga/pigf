
/// @file lxgen.cc
/// @brief LxGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "LxGen.h"
#include "RandLxGen.h"
#include "VarPool.h"
#include "YmUtils/PoptMainApp.h"


BEGIN_NAMESPACE_YM_IGF

struct Lt
{
  bool
  operator()(const pair<ymuint, Variable*>& left,
	     const pair<ymuint, Variable*>& right)
  {
    return left.first > right.first;
  }
};

int
lxgen(int argc,
      const char** argv)
{
  PoptMainApp main_app;

  // l オプション
  PoptNone popt_l("lx", 'l', "linear transformation");
  main_app.add_option(&popt_l);

  // r オプション
  PoptNone popt_r("rnd_lx", 'r', "random linear transformation");
  main_app.add_option(&popt_r);

  // s オプション
  PoptInt popt_s("n_sample", 's', "specify the number of samples", "<INT>");
  main_app.add_option(&popt_s);

  main_app.set_other_option_help("<filename>");

  tPoptStat stat = main_app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return 1;
  }

  if ( !popt_l.is_specified() && !popt_r.is_specified() ) {
    cerr << "Either 'l' or 'r' must be specified." << endl;
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

  ymuint n_sample = 1000;
  if ( popt_s.is_specified() ) {
    n_sample = popt_s.val();
  }
  if ( popt_l.is_specified() ) {
    LxGen lxgen;
    lxgen.init(rv_mgr.vect_list());
    VarPool var_pool(n_sample);
    for (ymuint i = 0; i < n_sample * 10; ++ i) {
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
  else if ( popt_r.is_specified() ) {
    RandLxGen lxgen;
    lxgen.generate(rv_mgr.vect_list(), n_sample, var_list);
  }

  // 度数分布を求める．
  ymuint h_array[20];
  for (ymuint i = 0; i < 20; ++ i) {
    h_array[i] = 0;
  }
  for (ymuint i = 0; i < var_list.size(); ++ i) {
    const Variable& var = var_list[i];
    double v = var.value(rv_mgr.vect_list());
    ymuint pos = static_cast<ymuint>(v * 20);
    ++ h_array[pos];
  }
  for (ymuint i = 0; i < 20; ++ i) {
    cout << " " << h_array[i];
  }
  cout << endl;

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  return nsYm::nsIgf::lxgen(argc, argv);
}
