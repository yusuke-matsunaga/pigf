
/// @file BasisChecker.cc
/// @brief BasisChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BasisChecker.h"
#include "Variable.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス BasisChecker
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BasisChecker::BasisChecker()
{
}

// @brief デストラクタ
BasisChecker::~BasisChecker()
{
}

// @brief 与えられた変数集合が基底となっているか調べる．
// @param[in] var_list 変数のリスト
// @return 基底ならば true を返す．
bool
BasisChecker::check(const vector<Variable>& var_list)
{
  // sum_i (k_i * x_i) = 0 を満たす all 0 以外の k_i が存在したら
  // x_i は線形従属なので基底ではない．

  // ここではサイズが数10のオーダーだと考えてベタな配列で実装する．
  ymuint nc = var_list.size();
  ASSERT_COND( nc > 0 );
  ymuint nr = var_list[0].var_size();

  if ( nc > nr ) {
    // 元の変数空間より var_list の空間のほうが大きい
    return false;
  }

  // 変数を列に対応させた行列を作る．
  vector<ymuint> matrix(nr * nc, 0);
  for (ymuint i_c = 0; i_c < nc; ++ i_c) {
    const Variable& var = var_list[i_c];
    for (ymuint i_r = 0; i_r < nr; ++ i_r) {
      if ( var.check_var(i_r) ) {
	matrix[i_r * nc + i_c] = 1;
      }
    }
  }

  // ガウスの消去法を行う．
  for (ymuint pivot = 0; pivot < nc; ++ pivot) {
    ymuint r1_base = pivot * nc;
    // pivot 行は 0 - (pivot - 1) 列まではすべて 0 になっている．
    if ( matrix[r1_base + pivot] == 0 ) {
      // pivot 行の pivot 列が 0 だった．
      // この列が 1 の他の行と入れ替える．
      bool found = false;
      for (ymuint i_r2 = pivot + 1; i_r2 < nr; ++ i_r2) {
	if ( matrix[i_r2 * nc + pivot] == 1 ) {
	  // pivot 行と i_r2 行を入れ替える．
	  ymuint r2_base = i_r2 * nc;
	  for (ymuint i_c = 0; i_c < nc; ++ i_c) {
	    ymuint tmp = matrix[r1_base + i_c];
	    matrix[r1_base + i_c] = matrix[r2_base + i_c];
	    matrix[r2_base + i_c] = tmp;
	  }
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	continue;
      }
    }

    // pivot + 1 以降の行で pivot に 1 が立っている行から
    // pivot 行を引く．
    for (ymuint i_r2 = pivot + 1; i_r2 < nr; ++ i_r2) {
      ymuint r2_base = i_r2 * nc;
      if ( matrix[r2_base + pivot] == 0 ) {
	continue;
      }
      for (ymuint i_c = pivot; i_c < nc; ++ i_c) {
	matrix[r2_base + i_c] ^= matrix[r1_base + i_c];
      }
    }
  }

  // 自明でない解が存在するか調べる．
  for (ymuint i_r = nr; i_r > 0; ) {
    -- i_r;
    ymuint c = 0;
    ymuint r_base = i_r * nc;
    ymuint last_c = nc;
    for (ymuint i_c = 0; i_c < nc; ++ i_c) {
      if ( matrix[r_base + i_c] == 1 ) {
	++ c;
	last_c = i_c;
      }
    }
    if ( c > 1 ) {
      // 自明でない解があった．
      return false;
    }
    if ( c == 1 ) {
      // last_c に 0 を代入する．
      for (ymuint i_r1 = 0; i_r1 < nr; ++ i_r1) {
	matrix[i_r1 * nc + last_c] = 0;
      }
    }
  }
  // 自明な解以外存在しなかった．
  return true;
}

END_NAMESPACE_IGF
