#ifndef SIMPLE_BASEGEN_H
#define SIMPLE_BASEGEN_H

/// @file SimpleBaseGen.h
/// @brief SimpleBaseGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BaseGenBase.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class Simple_BaseGen Simple_BaseGen.h "Simple_BaseGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class Simple_BaseGen :
  public BaseGenBase
{
public:

  /// @brief コンストラクタ
  Simple_BaseGen();

  /// @brief デストラクタ
  virtual
  ~Simple_BaseGen();


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

END_NAMESPACE_IGF

#endif // SIMPLE_BASEGEN_H
