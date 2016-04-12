
/// @file Simple_BaseGen.cc
/// @brief Simple_BaseGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Simple_BaseGen.h"
#include "RegVect.h"
#include "Variable.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス Simple_BaseGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Simple_BaseGen::Simple_BaseGen()
{
}

// @brief デストラクタ
Simple_BaseGen::~Simple_BaseGen()
{
}

// @brief 合成変数の生成を行う．
// @param[in] rv_list 登録ベクタのリスト
// @param[in] req_num 生成する変数の数
// @param[out] var_list 結果の変数を入れるリスト
//
// var_list の内容の Variable は呼んだ側で解放すること．
void
Simple_BaseGen::generate(const vector<const RegVect*>& rv_list,
		       ymuint req_num,
		       vector<Variable>& var_list)
{
  // 初期変数集合を作る．
  vector<Variable> pvar_list;
  get_primary_vars(rv_list, pvar_list);

  // 単純なランダムサンプリングで合成変数を作る．
  // ただしもとの変数の数に応じてアルゴリズムを変える．

  ymuint nv = pvar_list.size();
  var_list.clear();
  var_list.reserve(req_num);

  if ( nv < 20 ) {
    // 2^nv 通りのべき集合のなかから正確に req_num 個をランダムサンプリングする．
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
    // 32 ビットの乱数を req_num 個生成し，対応する変数を作る．
    // 場合によっては重複する場合がある．
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
    // nv ビットの乱数を req_num 個生成し，対応する変数を作る．
    // 場合によっては重複する場合がある．
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

END_NAMESPACE_IGF
