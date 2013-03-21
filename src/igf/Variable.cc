
/// @file Variable.cc
/// @brief Variable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Variable.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス Variable
//////////////////////////////////////////////////////////////////////

// @brief 通常の変数用のコンストラクタ
// @param[in] vid 変数番号
Variable::Variable(ymuint vid) :
  mVidList(1, vid)
{
}

// @brief 合成変数用のコンストラクタ
// @param[in] vid_list 変数番号のリスト
Variable::Variable(const vector<ymuint>& vid_list) :
  mVidList(vid_list)
{
}

// @brief デストラクタ
Variable::~Variable()
{
}

// @brief 合成度を返す．
ymuint
Variable::compound_degree() const
{
  return mVidList.size();
}

// @brief 通常の変数の場合に変数番号を返す．
// @note 合成変数の場合の動作は未定
ymuint
Variable::vid() const
{
  return mVidList[0];
}

// @brief 合成変数の場合に変数番号のリストを返す．
// @note 通常の変数の場合の動作は未定
const vector<ymuint>&
Variable::vid_list() const
{
  return mVidList;
}

// @brief ベクタを分類する．
// @param[in] vect 対象のベクタ
ymuint
Variable::classify(const RegVect* vect) const
{
  ymuint ans = 0;
  for (vector<ymuint>::const_iterator p = mVidList.begin();
       p != mVidList.end(); ++ p) {
    ymuint varid = *p;
    ans ^= vect->val(varid);
  }
  return ans;
}


END_NAMESPACE_YM_IGF