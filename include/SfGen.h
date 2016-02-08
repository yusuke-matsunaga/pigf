#ifndef SFGEN_H
#define SFGEN_H

/// @file SfGen.h
/// @brief SfGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class SfGen SfGen.h "SfGen.h"
/// @brief SigFunc を生成する純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class SfGen
{
public:

  /// @brief インスタンスを生成するメソッド
  /// @param[in] method アルゴリズム名
  static
  SfGen*
  new_obj(string method);

  /// @brief デストラクタ
  virtual
  ~SfGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SigFunc の生成を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  /// @param[in] base_list 基底変数のリスト
  /// @param[in] m 多重度
  /// @param[out] sigfunc_list 生成された SigFunc を格納するリスト
  virtual
  void
  generate(const vector<const RegVect*>& rv_list,
	   const vector<Variable>& base_list,
	   ymuint m,
	   vector<SigFunc*>& sigfunc_list) = 0;

};

END_NAMESPACE_IGF

#endif // SFGEN_H
