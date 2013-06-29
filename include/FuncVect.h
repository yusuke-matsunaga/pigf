#ifndef FUNCVECT_H
#define FUNCVECT_H

/// @file FuncVect.h
/// @brief FuncVect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class FuncVect FuncVect.h "FuncVect.h"
/// @brief 関数の値のベクタ
//////////////////////////////////////////////////////////////////////
class FuncVect
{
public:

  /// @brief コンストラクタ
  FuncVect(ymuint max_val,
	   ymuint input_size);

  /// @brief デストラクタ
  ~FuncVect();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の最大値を得る．
  ymuint
  max_val() const;

  /// @brief 入力の要素数を得る．
  ymuint
  input_size() const;

  /// @brief 値を得る．
  /// @param[in] id 入力番号 ( 0 <= id < input_size() )
  ymuint
  val(ymuint id) const;

  /// @brief 値を設定する．
  /// @param[in] id 入力番号 ( 0 <= id < input_size() )
  /// @param[in] val 値 ( 0 <= val < ma_val() )
  void
  set_val(ymuint id,
	  ymuint val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の最大値 + 1
  ymuint32 mMaxVal;

  // 値のベクタ
  vector<ymuint32> mVector;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FuncVect::FuncVect(ymuint max_val,
		   ymuint input_size) :
  mMaxVal(max_val),
  mVector(input_size)
{
}

// @brief デストラクタ
inline
FuncVect::~FuncVect()
{
}

// @brief 値の最大値を得る．
inline
ymuint
FuncVect::max_val() const
{
  return mMaxVal;
}

// @brief 入力の要素数を得る．
inline
ymuint
FuncVect::input_size() const
{
  return mVector.size();
}

// @brief 値を得る．
// @param[in] id 入力番号 ( 0 <= id < input_size() )
inline
ymuint
FuncVect::val(ymuint id) const
{
  assert_cond( id < mVector.size(), __FILE__, __LINE__);
  return mVector[id];
}

// @brief 値を設定する．
// @param[in] id 入力番号 ( 0 <= id < input_size() )
// @param[in] val 値 ( 0 <= val < ma_val() )
inline
void
FuncVect::set_val(ymuint id,
		  ymuint val)
{
  assert_cond( id < mVector.size(), __FILE__, __LINE__);
  mVector[id] = val;
}

END_NAMESPACE_YM_IGF

#endif // FUNCVECT_H