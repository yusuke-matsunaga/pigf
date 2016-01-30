
/// @file Simple_LxGen.cc
/// @brief Simple_LxGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Simple_LxGen.h"
#include "RegVect.h"
#include "Variable.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス Simple_LxGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Simple_LxGen::Simple_LxGen()
{
}

// @brief デストラクタ
Simple_LxGen::~Simple_LxGen()
{
}

// @brief 合成変数の生成を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] req_num 生成する変数の数
// @param[out] var_list 結果の変数を入れるリスト
//
// var_list の内容の Variable は呼んだ側で解放すること．
void
Simple_LxGen::generate(const vector<const RegVect*>& rv_list,
		       ymuint req_num,
		       vector<Variable>& var_list)
{
  // 初期変数集合を作る．
  // 基本的にはすべての変数が対象だが，
  // 登録ベクタを区別しない変数は取り除く．
  ymuint var_num = 0;
  {
    ASSERT_COND( !rv_list.empty() );
    const RegVect* rv0 = rv_list[0];
    var_num = rv0->size();
  }
  vector<Variable> pvar_list;
  var_list.reserve(var_num);
  for (ymuint i = 0; i < var_num; ++ i) {
    Variable var1(var_num, i);
    double v = var1.value(rv_list);
    if ( v > 0.0 ) {
      pvar_list.push_back(var1);
    }
  }

  // 単純なランダムサンプリングで合成変数を作る．

  ymuint nv = pvar_list.size();
  var_list.clear();
  var_list.reserve(req_num);

  if ( nv < 20 ) {
    ymuint np = 1U << nv;
    vector<ymuint> pat_list(np);
    for (ymuint i = 0; i < np; ++ i) {
      pat_list[i] = i;
    }
    for (ymuint i = 0; i < req_num; ++ i) {
      // pat_list の中からランダムに選ぶ．
      // 選んだパタンは pat_list から取り除く
      ymuint idx = mRandGen.int32() % np;
      ymuint pat = pat_list[idx];
      -- np;
      pat_list[idx] = pat_list[np];

      // pat の 1 の立っているビットの変数を含む変数を合成する．
      Variable var;
      bool first = true;
      for (ymuint b = 0; b < nv; ++ b) {
	if ( pat & (1U << b) ) {
	  if ( first ) {
	    var = pvar_list[b];
	    first = false;
	  }
	  else {
	    var *= pvar_list[b];
	  }
	}
      }
      var_list.push_back(var);
    }
  }
  else if ( nv < 32 ) {
    ymuint32 np = 1U << nv;
    for (ymuint i = 0; i < req_num; ++ i) {
      ymuint32 pat = mRandGen.int32() % np;

      // pat の 1 の立っているビットの変数を含む変数を合成する．
      Variable var;
      bool first = true;
      for (ymuint b = 0; b < nv; ++ b) {
	if ( pat & (1U << b) ) {
	  if ( first ) {
	    var = pvar_list[b];
	    first = false;
	  }
	  else {
	    var *= pvar_list[b];
	  }
	}
      }
      var_list.push_back(var);
    }
  }
  else {
    for (ymuint i = 0; i < req_num; ++ i) {
      Variable var;
      bool first = true;
      ymuint nblk = (nv + 31) / 32;
      ymuint nv1 = nv - (nblk - 1) * 32;
      for (ymuint j = 0; j < nblk - 1; ++ j) {
	ymuint32 pat = mRandGen.int32();
	for (ymuint b = 0; b < 32; ++ b) {
	  if ( pat & (1U << b) ) {
	    const Variable& var1 = pvar_list[b + j * 32];
	    if ( first ) {
	      var = var1;
	      first = false;
	    }
	    else {
	      var *= var1;
	    }
	  }
	}
      }
      ymuint32 pat = mRandGen.int32();
      for (ymuint b = 0; b < nv1; ++ b) {
	if ( pat & (1U << b) ) {
	  const Variable& var1 = pvar_list[b + (nblk - 1) * 32];
	  if ( first ) {
	    var = var1;
	    first = false;
	  }
	  else {
	    var *= var1;
	  }
	}
      }
      var_list.push_back(var);
    }
  }
}

END_NAMESPACE_YM_IGF
