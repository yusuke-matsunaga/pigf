#ifndef SIGFUNCGEN_H
#define SIGFUNCGEN_H

/// @file SigFuncGen.h
/// @brief SigFuncGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class SigFuncGen SigFuncGen.h "SigFuncGen.h"
/// @brief signature function を生成するクラス
///
/// 実は signature function は p 個の変数を束にしたもの
//////////////////////////////////////////////////////////////////////
class SigFuncGen
{
public:

  /// @brief コンストラクタ
  SigFuncGen();

  /// @brief デストラクタ
  ~SigFuncGen();


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
       ymuint m,
       ymuint burnin_int,
       ymuint sample_int);

  /// @brief signature function を m 個生成する．
  vector<const SigFunc*>
  generate();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 基本処理
  void
  body();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 一つの関数の状態を表すクラス
  class FuncState
  {
  public:

    /// @brief 初期化を行う．
    /// @param[in] rv_list 登録ベクタのリスト
    /// @param[in] var_list 変数のリスト
    /// @param[in] width 出力のビット幅
    ///
    /// ランダムに初期状態を決める．
    void
    init(const vector<const RegVect*>& rv_list,
	 const vector<Variable>& var_list,
	 ymuint width,
	 RandGen& rg1);

    /// @brief 次の状態に遷移する．
    /// @param[in] rv_list 登録ベクタのリスト
    void
    next_move(const vector<const RegVect*>& rv_list,
	      RandGen& rg1,
	      RandGen& rg2);

    /// @brief 現在の状態から SigFunc を生成する．
    SigFunc*
    new_func() const;


  private:
    //////////////////////////////////////////////////////////////////////
    // 内部で用いられる関数
    //////////////////////////////////////////////////////////////////////

    /// @brief 変数をランダムに選ぶ．
    /// @param[in] rg 乱数発生器
    ///
    /// 重複を避けるために選ばれた変数は mCandList から取り除かれる．
    Variable
    choose_var(RandGen& rg);


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 変数の候補リスト
    // mCurState に含まれる変数は取り除かれている．
    vector<Variable> mCandList;

    // 現在の SigFunc 用の変数
    vector<Variable> mCurState;

    // 現在の価値
    double mCurVal;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数選択用の乱数発生器
  RandGen mRgChoose;

  // 受容/棄却を決めるための乱数発生器
  RandGen mRgAccept;

  // 登録ベクタのリスト
  vector<const RegVect*> mRvList;

  // 変数のリスト
  vector<Variable> mVarList;

  // 出力のビット幅
  ymuint mWidth;

  // 多重度
  ymuint mM;

  // サンプリング間隔
  ymuint mSampleInt;

  // 現在の関数の状態
  // mM 個ある．
  vector<FuncState> mCurStateArray;

};

END_NAMESPACE_YM_IGF

#endif // SIGFUNCGEN_H
