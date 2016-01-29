#ifndef LXGEN_H
#define LXGEN_H

/// @file LxGen.h
/// @brief LxGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "Variable.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class LxGen LxGen.h "LxGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class LxGen
{
public:

  /// @brief コンストラクタ
  LxGen();

  /// @brief デストラクタ
  ~LxGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  void
  init(const vector<const RegVect*>& rv_list);

  /// @brief 合成変数の生成を行う．
  Variable
  generate(double& val);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の状態に遷移する．
  void
  next_move();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遷移を行うための乱数発生器
  RandGen mRgMove;

  // 受容/棄却を決めるための乱数発生器
  RandGen mRgAccept;

  // 登録ベクタのリスト
  vector<const RegVect*> mRvList;

  // プライマリ変数のリスト
  vector<Variable> mPrimaryList;

  // 現在の状態
  Variable mCurState;

  // 現在の値
  double mCurVal;

};

END_NAMESPACE_YM_IGF


#endif // LXGEN_H
