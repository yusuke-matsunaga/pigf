﻿#ifndef VARIABLE_H
#define VARIABLE_H

/// @file Variable.h
/// @brief Variable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class Variable Variable.h "Variable.h"
/// @brief 変数を表すクラス
///
/// ここでいう変数とは1つまたは複数の primary input を XOR で結合
/// したもの
/// 要するに変数番号の集合と等価
/// 実装としては固定長のビットベクタを用いる．
//////////////////////////////////////////////////////////////////////
class Variable
{
public:

  /// @brief 空のコンストラクタ
  Variable();

  /// @brief 通常の変数用のコンストラクタ
  /// @param[in] var_num 変数の総数
  /// @param[in] vid 変数番号
  Variable(ymuint var_num,
	   ymuint vid);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Variable(const Variable& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const Variable&
  operator=(const Variable& src);

  /// @brief デストラクタ
  ~Variable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表す変数番号のリストを返す．
  ///
  /// vector<> のコピーを作るので比較的コスト
  /// の高い演算
  vector<ymuint>
  vid_list() const;

  /// @brief ビットベクタの生データを返す．
  /// @param[in] pos ブロック番号
  ymuint64
  raw_data(ymuint pos) const;

  /// @brief 合成演算
  /// @param[in] right オペランド
  /// @return 合成結果(自身への参照)を返す．
  ///
  /// 合成結果を自分に代入する intern 演算
  const Variable&
  operator*=(const Variable& right);

  /// @brief 共通要素を持つとき true を返す．
  /// @param[in] right オペランド
  bool
  operator&&(const Variable& right) const;

  /// @brief 等価比較
  /// @param[in] right オペランド
  /// @return 等しい時 true を返す．
  bool
  operator==(const Variable& right) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタのブロック数を返す．
  ymuint
  nblk() const;

  /// @brief 変数のビット位置のブロック番号を返す．
  /// @param[in] vid 変数番号
  static
  ymuint
  blk(ymuint vid);

  /// @brief 変数のビット位置のシフト数を返す．
  /// @param[in] vid 変数番号
  static
  ymuint
  sft(ymuint vid);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint mVarNum;

  // 変数を表すビットベクタ
  ymuint64* mBitVect;

};

/// @relates Variable
/// @brief 合成演算
/// @param[in] left, right オペランド
/// @return 演算結果を返す．
Variable
operator*(const Variable& left,
	  const Variable& right);

END_NAMESPACE_YM_IGF

#endif // VARIABLE_H
