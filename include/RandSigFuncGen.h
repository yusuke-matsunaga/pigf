#ifndef RANDSIGFUNCGEN_H
#define RANDSIGFUNCGEN_H

/// @file RandSigFuncGen.h
/// @brief RandSigFuncGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/RandCombiGen.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class RandSigFuncGen RandSigFuncGen.h "RandSigFuncGen.h"
/// @brief signature function を生成するクラス
///
/// 実は signature function は p 個の変数を束にしたもの
//////////////////////////////////////////////////////////////////////
class RandSigFuncGen
{
public:

  /// @brief コンストラクタ
  RandSigFuncGen();

  /// @brief デストラクタ
  ~RandSigFuncGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  /// @param[in] var_list 変数のリスト
  /// @param[in] width 出力のビット幅
  /// @param[in] m 多重度
  /// @param[in] burnin_int 'burn in' のための回数
  /// @param[in] simple_int サンプリングの間隔
  void
  init(const vector<const RegVect*>& rv_list,
       const vector<Variable>& var_list,
       ymuint width,
       ymuint m);

  /// @brief signature function を m 個生成する．
  vector<const SigFunc*>
  generate();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数選択用の乱数発生器
  RandGen mRgChoose;

  // 組み合わせ選択器
  RandCombiGen* mRcg;

  // 変数のリスト
  vector<Variable> mVarList;

  // 出力のビット幅
  ymuint mWidth;

  // 多重度
  ymuint mM;

};

END_NAMESPACE_YM_IGF

#endif // RANDSIGFUNCGEN_H
