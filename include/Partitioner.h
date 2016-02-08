#ifndef PARTITIONER_H
#define PARTITIONER_H

/// @file Partitioner.h
/// @brief Partitioner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
/// @class Partitioner Partitioner.h "Partitioner.h"
/// @brief ベクタを分割する処理を行うクラス
//////////////////////////////////////////////////////////////////////
class Partitioner
{
public:

  /// @brief コンストラクタ
  Partitioner();

  /// @brief デストラクタ
  ~Partitioner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ベクタを分割する．
  /// @param[in] vect_list ベクタのリスト
  /// @param[in] sigfunc_list シグネチャ関数のリスト
  /// @param[out] mapping 個々のベクタの割当結果を入れる配列
  /// @return 分割が成功したら true を返し，割当結果を mapping に入れる．
  ///
  /// mapping[i] には i 番目のベクタの割当先の番号が入る．
  bool
  cf_partition(const vector<const RegVect*>& vect_list,
	       const vector<const SigFunc*>& sigfunc_list,
	       vector<ymuint>& mapping);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Slot;

  // ベクタに対応する情報
  struct VectInfo {
    // 元のベクタ
    const RegVect* mVect;

    // 現在の割当先のスロット
    Slot* mCurSlot;

    // 自分を追い出したベクタ
    VectInfo* mSrc;

    // 処理済みの「印」
    bool mMark;

  };

  // ベクタを納めるスロットに関する情報
  struct Slot {

    // シグネチャ関数(ブロック)の番号
    ymuint mSigNum;

    // シグネチャの値
    ymuint mSignature;

    // 現在割り当てられているベクタ
    VectInfo* mCurVect;

  };

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタの配列
  vector<VectInfo> mVectArray;

  // シグネチャ関数ごとのスロットの配列
  vector<vector<Slot> > mSlotArray;

};

END_NAMESPACE_IGF

#endif // PARTITIONER_H
