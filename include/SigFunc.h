#ifndef SIGFUNC_H
#define SIGFUNC_H

/// @file SigFunc.h
/// @brief SigFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class SigFunc SigFunc.h "SigFunc.h"
/// @brief シグネチャ関数を表すクラス
///
/// 登録ベクタ(RegVect) を入力としてシグネチャを出力する関数
//////////////////////////////////////////////////////////////////////
class SigFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数のリスト
  SigFunc(const vector<Variable*>& var_list);

  /// @brief デストラクタ
  ~SigFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力のビット幅を返す．
  ymuint
  output_width() const;

  /// @brief 関数値を求める．
  /// @param[in] rv 登録ベクタ
  ymuint
  eval(const RegVect* rv) const;

  /// @brief 内容を表示する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<Variable*> mVarList;

};

END_NAMESPACE_YM_IGF

#endif // SIGFUNC_H
