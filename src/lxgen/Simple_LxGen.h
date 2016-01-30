#ifndef SIMPLE_LXGEN_H
#define SIMPLE_LXGEN_H

/// @file SimpleLxGen.h
/// @brief SimpleLxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LxGen.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class Simple_LxGen Simple_LxGen.h "Simple_LxGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class Simple_LxGen :
  public LxGen
{
public:

  /// @brief コンストラクタ
  Simple_LxGen();

  /// @brief デストラクタ
  virtual
  ~Simple_LxGen();


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
	   vector<Variable>& var_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数を選ぶための乱数発生器
  RandGen mRandGen;

};

END_NAMESPACE_YM_IGF

#endif // SIMPLE_LXGEN_H
