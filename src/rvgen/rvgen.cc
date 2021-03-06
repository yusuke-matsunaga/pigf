﻿
/// @file rvgen.cc
/// @brief rvgen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

class BinaryRandGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] prob 1 になる確率
  BinaryRandGen(double prob = 0.5);

  /// @brief デストラクタ
  ~BinaryRandGen();


public:

  /// @brief シードで初期化する．
  /// @param[in] seed 初期化で用いるシード
  void
  init(ymuint seed);

  /// @brief 0/1 の乱数を発生させる．
  bool
  value();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数発生器
  RandGen mRandGen;

  // 1になる確率
  double mProb;

  // 一時バッファ
  ymuint mBuffer;

  // バッファ中の次に利用可能な位置
  ymuint mNextPos;

};


// @brief コンストラクタ
// @param[in] prob 1になる確率
BinaryRandGen::BinaryRandGen(double prob)
{
  mProb = prob;
  mNextPos = 32;
}

// @brief デストラクタ
BinaryRandGen::~BinaryRandGen()
{
}

// @brief シードで初期化する．
// @param[in] seed 初期化で用いるシード
void
BinaryRandGen::init(ymuint seed)
{
  mRandGen.init(seed);
}

// @brief 0/1 の乱数を発生させる．
bool
BinaryRandGen::value()
{
#if 0
  if ( mNextPos >= 32 ) {
    mBuffer = mRandGen.int32();
    mNextPos = 0;
  }
  bool ans = static_cast<bool>((mBuffer >> mNextPos) & 1U);
  ++ mNextPos;
  return ans;
#else
  double v = mRandGen.real2();
  if ( v <= mProb ) {
    return true;
  }
  else {
    return false;
  }
#endif
}


// n 次元 k 個の登録ベクタを出力する．
void
gen_vectors(ymuint n,
	    ymuint k,
	    BinaryRandGen& rg,
	    ostream& s)
{
  s << n << " " << k << endl;
  // k 個のベクタを出力
  unordered_set<string> patterns;
  for (ymuint i = 0; i < k; ++ i) {
    ostringstream buf;
    for (ymuint j = 0; j < n; ++ j) {
      buf << rg.value();
    }
    string pat = buf.str();
    if ( patterns.count(pat) > 0 ) {
      -- i;
    }
    else {
      patterns.insert(pat);
      s << pat << endl;
    }
  }
}

int
rvgen(int argc,
      const char** argv)
{
  PoptMainApp main_app;

  // seed オプション
  PoptUint popt_seed("seed", 's',
		     "specify random seed",
		     "<INT>");

  // prob オプション
  PoptDouble popt_prob("prob", 'p',
		       "specify probability",
		       "0.0 - 1.0");

  main_app.add_option(&popt_seed);
  main_app.add_option(&popt_prob);

  main_app.set_other_option_help("<vector size> <vector num>");

  // オプション解析を行なう．
  tPoptStat stat = main_app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  vector<string> args;
  ymuint n_args = main_app.get_args(args);

  if ( n_args != 2 ) {
    main_app.usage(2);
  }

  int tmp_n = atoi(args[0].c_str());
  if ( tmp_n <= 0 ) {
    cerr << "Argument value error: first argument should be >= 0" << endl;
    return 3;
  }

  int tmp_k = atoi(args[1].c_str());
  if ( tmp_k <= 0 ) {
    cerr << "Argument value error: second argument should be >= 0" << endl;
    return 3;
  }

  ymuint n = tmp_n;
  ymuint k = tmp_k;

  ymuint np = 1U << n;
  if ( n < 32 && k > np ) {
    cerr << "Argument value error: second argument should be < 2^n" << endl;
    return 3;
  }

  double prob = 0.5;
  if ( popt_prob.is_specified() ) {
    prob = popt_prob.val();
  }

  BinaryRandGen rg(prob);

  if ( popt_seed.is_specified() ) {
    rg.init(popt_seed.val());
  }

  //  n と k を出力
  gen_vectors(n, k, rg, cout);

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::rvgen;

  return rvgen(argc, argv);
}
