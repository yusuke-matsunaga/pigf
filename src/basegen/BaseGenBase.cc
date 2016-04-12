
/// @file BaseGenBase.cc
/// @brief BaseGenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BaseGenBase.h"
#include "RegVect.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス BaseGenBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BaseGenBase::BaseGenBase()
{
}

// @brief デストラクタ
BaseGenBase::~BaseGenBase()
{
}

// @brief 初期変数集合を求める．
// @param[in] rv_list 登録ベクタのリスト
// @param[out] var_list 変数を格納するリスト
//
// 基本的にはすべての変数が対象だが，
// 登録ベクタを区別しない変数は取り除く．
void
BaseGenBase::get_primary_vars(const vector<const RegVect*>& rv_list,
			    vector<Variable>& var_list)
{
  ymuint var_num = 0;
  {
    ASSERT_COND( !rv_list.empty() );
    const RegVect* rv0 = rv_list[0];
    var_num = rv0->size();
  }
  var_list.clear();
  var_list.reserve(var_num);
  for (ymuint i = 0; i < var_num; ++ i) {
    Variable var1(var_num, i);
    double v = var1.value(rv_list);
    if ( v > 0.0 ) {
      var_list.push_back(var1);
    }
  }
}

END_NAMESPACE_IGF
