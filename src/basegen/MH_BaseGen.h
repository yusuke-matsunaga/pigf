#ifndef MH_BASEGEN_H
#define MH_BASEGEN_H

/// @file MH_BaseGen.h
/// @brief MH_BaseGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BaseGen.h"
#include "MhSampler.h"
#include "BaseState.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class MH_BaseGen MH_BaseGen.h "MH_BaseGen.h"
/// @brief MhSampler を用いた BaseGen
//////////////////////////////////////////////////////////////////////
class MH_BaseGen :
  public BaseGen
{
public:

  /// @brief コンストラクタ
  MH_BaseGen();

  /// @brief デストラクタ
  virtual
  ~MH_BaseGen();


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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_IGF

#endif // MH_BASEGEN_H