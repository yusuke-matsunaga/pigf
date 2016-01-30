
/// @file SigFunc.cc
/// @brief SigFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SigFunc.h"
#include "Variable.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス SigFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数のリスト
SigFunc::SigFunc(const vector<Variable>& var_list) :
  mVarList(var_list)
{
}

// @brief デストラクタ
SigFunc::~SigFunc()
{
}

// @brief 出力のビット幅を返す．
ymuint
SigFunc::output_width() const
{
  return mVarList.size();
}

// @brief 関数値を求める．
// @param[in] rv 登録ベクタ
ymuint
SigFunc::eval(const RegVect* rv) const
{
  ymuint ans = 0U;
  for (ymuint i = 0; i < mVarList.size(); ++ i) {
    const Variable& var = mVarList[i];
    if ( rv->classify(var) ) {
      ans |= (1U << i);
    }
  }
  return ans;
}

// @brief 内容を表示する．
// @param[in] s 出力先のストリーム
void
SigFunc::dump(ostream& s) const
{
  for (ymuint i = 0; i < mVarList.size(); ++ i) {
    const Variable& var = mVarList[i];
    s << "#" << i << ": " << var << endl;
  }
}

END_NAMESPACE_YM_IGF
