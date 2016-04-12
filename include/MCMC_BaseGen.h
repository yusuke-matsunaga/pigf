#ifndef MCMC_BASEGEN_H
#define MCMC_BASEGEN_H

/// @file MCMC_BaseGen.h
/// @brief MCMC_BaseGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BaseGen.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class MCMC_BaseGen MCMC_BaseGen.h "MCMC_BaseGen.h"
/// @brief 線形変換用の合成変数を生成するクラス
//////////////////////////////////////////////////////////////////////
class MCMC_BaseGen :
  public BaseGen
{
public:

  /// @brief コンストラクタ
  MCMC_BaseGen();

  /// @brief デストラクタ
  virtual
  ~MCMC_BaseGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 合成変数の生成を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  /// @param[in] req_num 生成する変数の数
  /// @param[out] var_list 生成された変数を格納するリスト
  void
  generate(const vector<const RegVect*>& rv_list,
	   ymuint req_num,
	   vector<Variable>& var_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録ベクタのリストを返す．
  const vector<const RegVect*>&
  rv_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] rv_list 登録ベクタのリスト
  void
  init(const vector<const RegVect*>& rv_list);

  /// @brief 次の状態に遷移する．
  void
  next_move();

  /// @brief 変数の価値を計算する．
  virtual
  double
  value(const Variable& var);


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

END_NAMESPACE_IGF

#endif // MCMC_BASEGEN_H
