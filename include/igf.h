#ifndef IGF_H
#define IGF_H

/// @file igf.h
/// @brief IGF 用の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// ym-common のコンフィギュレーションファイル
//////////////////////////////////////////////////////////////////////


#include "ym_config.h"


//////////////////////////////////////////////////////////////////////
// バージョン変数の設定
//////////////////////////////////////////////////////////////////////

#include "version_config.h"


// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

//////////////////////////////////////////////////////////////////////
// 名前空間の定義用マクロ
// namespace でネストするのがいやなので define マクロでごまかす．
//////////////////////////////////////////////////////////////////////

#define BEGIN_NAMESPACE_IGF			\
  BEGIN_NAMESPACE_YM				\
  BEGIN_NAMESPACE(IGF_NAMESPACE)

#define END_NAMESPACE_IGF			\
  END_NAMESPACE(IGF_NAMESPACE)			\
  END_NAMESPACE_YM


BEGIN_NAMESPACE_IGF

class RegVect;
class RvMgr;

class Variable;
class SigFunc;
class FuncVect;

END_NAMESPACE_IGF

#endif // IGF_H
