
/// @file Partitioner.cc
/// @brief Partitioner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Partitioner.h"
#include "SigFunc.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス Partitioner
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Partitioner::Partitioner()
{
}

// @brief デストラクタ
Partitioner::~Partitioner()
{
}

// @brief ベクタを分割する．
// @param[in] vect_list ベクタのリスト
// @param[in] sigfunc_list シグネチャ関数のリスト
// @param[out] mapping 個々のベクタの割当結果を入れる配列
// @return 分割が成功したら true を返し，割当結果を mapping に入れる．
//
// mapping[i] には i 番目のベクタの割当先の番号が入る．
bool
Partitioner::cf_partition(const vector<const RegVect*>& vect_list,
			  const vector<const SigFunc*>& sigfunc_list,
			  vector<ymuint>& mapping)
{
  // スロットの情報を初期化する．
  ymuint nb = sigfunc_list.size();
  mSlotArray.clear();
  mSlotArray.resize(nb);
  for (ymuint i = 0; i < nb; ++ i) {
    const SigFunc* sf = sigfunc_list[i];
    ymuint ow = sf->output_width();
    ymuint ns = 1UL << ow;
    vector<Slot>& slot_array = mSlotArray[i];
    slot_array.clear();
    slot_array.resize(ns);
    for (ymuint j = 0; j < ns; ++ j) {
      Slot& slot = slot_array[j];
      slot.mSigNum = i;
      slot.mSignature = j;
      slot.mCurVect = nullptr;
    }
  }

  // ベクタの情報を初期化する．
  ymuint nv = vect_list.size();
  mVectArray.clear();
  mVectArray.resize(nv);
  // ひとつずつベクタを割り当てていく．
  for (ymuint i = 0; i < nv; ++ i) {
    const RegVect* v = vect_list[i];
    VectInfo* vi = &mVectArray[i];
    vi->mVect = v;
    vi->mCurSlot = nullptr;
    vi->mSrc = nullptr;
    vi->mMark = false;

    // 空いているスロットを探す．
    vector<VectInfo*> queue;
    queue.push_back(vi);
    for (ymuint rpos = 0; rpos < queue.size(); ++ rpos) {
      VectInfo* vi1 = queue[rpos];

      // vi1 の別の割当を求める．
      bool found = false;
      vector<VectInfo*> tmp_vect_list;
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint sig = sigfunc_list[j]->eval(vi1->mVect);
	Slot* slot = &mSlotArray[j][sig];
	if ( slot->mCurVect == nullptr ) {
	  // 空いていた．
	  for ( ; ; ) {
	    Slot* old_slot = vi1->mCurSlot;
	    vi1->mCurSlot = slot;
	    slot->mCurVect = vi1;
	    VectInfo* vi2 = vi1->mSrc;
	    if ( vi2 == nullptr ) {
	      break;
	    }
	    vi1 = vi2;
	    slot = old_slot;
	  }
	  found = true;
	  break;
	}
	else {
	  VectInfo* vi2 = slot->mCurVect;
	  if ( vi2 != vi1 && !vi2->mMark ) {
	    tmp_vect_list.push_back(vi2);
	  }
	}
      }
      if ( found ) {
	break;
      }

      for (ymuint j = 0; j < tmp_vect_list.size(); ++ j) {
	VectInfo* vi2 = tmp_vect_list[j];
	vi2->mSrc = vi1;
	vi2->mMark = true;
	queue.push_back(vi1);
      }
    }

    // 処理したノードのマークを消す．
    for (ymuint j = 0; j < queue.size(); ++ j) {
      queue[j]->mMark = false;
    }

    if ( vi->mCurSlot == nullptr ) {
      // 割当が見つからなかった．
      return false;
    }

    // 割当結果を記録する．
    mapping[i] = vi->mCurSlot->mSigNum;
  }

  return true;
}


END_NAMESPACE_YM_IGF
