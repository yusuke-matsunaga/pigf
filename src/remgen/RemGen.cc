
/// @file RemGen.cc
/// @brief RemGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "RemGen.h"
#include "BasisChecker.h"
#include "SigFunc.h"
#include "Variable.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス RemGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_num 変数の数
RemGen::RemGen(ymuint var_num) :
  mVarNum(var_num)
{
}

// @brief デストラクタ
RemGen::~RemGen()
{
}

// @brief シグネチャに対応するリマインダ変数を求める．
vector<Variable>
RemGen::generate(const SigFunc& sig)
{
  BasisChecker checker;

  // 現在の変数集合
  vector<Variable> cur_vars;

  // シグネチャの変数を cur_vars に加える．
  // 同時にシグネチャに使われているプライマリ変数に印をつける．
  vector<bool> mark(mVarNum, false);
  ymuint n = sig.output_width();
  for (ymuint i = 0; i < n; ++ i) {
    Variable v = sig.output(i);
    cur_vars.push_back(v);
    for (ymuint j = 0; j < mVarNum; ++ j) {
      if ( v.check_var(j) ) {
	mark[j] = true;
      }
    }
  }

  // シグネチャと無関係なプライマリ変数は無条件でリマインダとなる．
  // けどこれは後で加える．
  vector<Variable> var_list0;
  vector<Variable> var_list1;
  for (ymuint vid = 0; vid < mVarNum; ++ vid) {
    Variable v(mVarNum, vid);
    if ( mark[vid] ) {
      var_list1.push_back(v);
    }
    else {
      var_list0.push_back(v);
    }
  }

  // var_list1 の変数を1つずつ cur_vars に加えて線形独立かどうかのチェックを行う．
  for (ymuint i = 0; i < var_list1.size(); ++ i) {
    // この位置でチェックするのはちょっと無駄だけど
    // ループの外側で同じチェックを一回やるのはかっこ悪いので
    if ( cur_vars.size() + var_list0.size() == mVarNum ) {
      break;
    }
    Variable v = var_list1[i];
    cur_vars.push_back(v);
    if ( !checker.check(cur_vars) ) {
      // 線形従属だったので取り消す．
      cur_vars.pop_back();
    }
  }

  // var_list0 の内容を cur_vars に加える．
  for (ymuint i = 0; i < var_list0.size(); ++ i) {
    Variable v = var_list0[i];
    cur_vars.push_back(v);
  }

  return cur_vars;
}

END_NAMESPACE_IGF
