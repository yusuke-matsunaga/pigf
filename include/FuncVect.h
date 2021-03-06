﻿#ifndef FUNCVECT_H
#define FUNCVECT_H

/// @file FuncVect.h
/// @brief FuncVect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class FuncVect FuncVect.h "FuncVect.h"
/// @brief 関数の値のベクタ
///
/// ここで言っている関数とは入力ハッシュ関数のこと
/// ただし，実際の登録ベクタがどういうものかは一切関係ない．
/// ただ単に pos 番めのベクタの関数値が何，という値をだけを保持する．
//////////////////////////////////////////////////////////////////////
class FuncVect
{
public:

  /// @brief コンストラクタ
  /// @param[in] input_size 入力の要素数
  /// @param[in] max_val 値の最大値
  ///
  /// max_val は正確には最大値＋１
  FuncVect(ymuint input_size,
	   ymuint max_val);

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
  /// @note 出力値の範囲は [0, max_val())
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

  // 入力の要素数
  ymuint mInputSize;

  // 出力値の最大値 + 1
  ymuint mMaxVal;

  // 値のベクタ
  // サイズは mInputSize
  ymuint* mVector;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FuncVect::FuncVect(ymuint input_size,
		   ymuint max_val) :
  mInputSize(input_size),
  mMaxVal(max_val)
{
  mVector = new ymuint[mInputSize];
}

// @brief デストラクタ
inline
FuncVect::~FuncVect()
{
  delete [] mVector;
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
  return mInputSize;
}

// @brief 値を得る．
// @param[in] id 入力番号 ( 0 <= id < input_size() )
inline
ymuint
FuncVect::val(ymuint id) const
{
  ASSERT_COND( id < mInputSize );
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
  ASSERT_COND( id < mInputSize );
  ASSERT_COND( val < mMaxVal );
  mVector[id] = val;
}

END_NAMESPACE_IGF

#endif // FUNCVECT_H
