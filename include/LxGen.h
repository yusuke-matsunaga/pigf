#ifndef LXGEN_H
#define LXGEN_H

/// @file LxGen.h
/// @brief LxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"
#include "Variable.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class LxGen LxGen.h "LxGen.h"
/// @brief 線形変換用の合成変数を生成する純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class LxGen
{
public:

  /// @brief インスタンスを生成するクラスメソッド
  /// @param[in] method アルゴリズム名
  static
  LxGen*
  new_obj(string method);

  /// @brief デストラクタ
  ~LxGen() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 合成変数の生成を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  /// @param[in] req_num 生成する変数の数
  /// @param[out] var_list 生成された変数を格納するリスト
  virtual
  void
  generate(const vector<const RegVect*>& rv_list,
	   ymuint req_num,
	   vector<Variable>& var_list) = 0;

};

END_NAMESPACE_IGF


#endif // LXGEN_H
