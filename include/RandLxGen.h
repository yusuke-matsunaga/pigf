#ifndef RANDLXGEN_H
#define RANDLXGEN_H

/// @file RandLxGen.h
/// @brief RandLxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class RandLxGen RandLxGen.h "RandLxGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class RandLxGen
{
public:

  /// @brief コンストラクタ
  RandLxGen();

  /// @brief デストラクタ
  ~RandLxGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 合成変数の生成を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  /// @param[in] output_size 出力する合成変数の数
  /// @param[out] var_list 結果の変数を入れるリスト
  void
  generate(const vector<const RegVect*>& rv_list,
	   ymuint output_size,
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

END_NAMESPACE_YM_IGF


#endif // RANDLXGEN_H
