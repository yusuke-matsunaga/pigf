﻿#ifndef XORFUNC_H
#define XORFUNC_H

/// @file XorFunc.h
/// @brief XorFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class XorFunc XorFunc.h "XorFunc.h"
/// @brief XOR でまとめた入力関数を表すクラス
//////////////////////////////////////////////////////////////////////
class XorFunc :
  public InputFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] vars_list 変数のリストのリスト
  XorFunc(const vector<vector<ymuint> >& vars_list);

  /// @brief デストラクタ
  virtual
  ~XorFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint
  output_width() const;

  /// @brief 関数値を求める．
  /// @param[in] rv 登録ベクタ
  virtual
  ymuint
  eval(const RegVect* rv) const;

  /// @brief 内容を表示する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<vector<ymuint> > mVarsList;

};

END_NAMESPACE_YM_IGF

#endif // XORFUNC_H
