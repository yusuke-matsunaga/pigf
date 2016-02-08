#ifndef GREEDY_LXGEN_H
#define GREEDY_LXGEN_H

/// @file Greedy_LxGen.h
/// @brief Greedy_LxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LxGen.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class Greedy_LxGen Greedy_LxGen.h "Greedy_LxGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class Greedy_LxGen :
  public LxGen
{
public:

  /// @brief コンストラクタ
  Greedy_LxGen();

  /// @brief デストラクタ
  virtual
  ~Greedy_LxGen();


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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief var_list の中からランダムに変数を選ぶ．
  /// @param[inout] var_list 変数のリスト
  ///
  /// 選ばれた変数は var_list から取り除かれる．
  Variable
  choose_var(vector<Variable>& var_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数を選ぶための乱数発生器
  RandGen mRandGen;

};

END_NAMESPACE_IGF

#endif // GREEDY_LXGEN_H
