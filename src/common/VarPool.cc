﻿
/// @file VarPool.cc
/// @brief VarPool の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "VarPool.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス VarPool
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num ノード数
VarPool::VarPool(ymuint num)
{
  mHeapSize = num;
  mHeap = new Node[num];
  mVarNum = 0;
}

// @brief デストラクタ
VarPool::~VarPool()
{
  delete [] mHeap;
}

// @brief 変数を追加する．
void
VarPool::put(const Variable& var,
	     double value)
{
  if ( mHashTable.find(var) ) {
    // 同じものがすでに入っていた．
    return;
  }

  if ( mVarNum == mHeapSize ) {
    // 一杯だった．
    // 先頭の要素を捨てる．
    mHashTable.erase(mHeap[0].mVar);
    // 代わりに新しい変数を入れる．
    mHeap[0].mVar = var;
    mHeap[0].mValue = value;
    // 適切な位置まで降ろす．
    move_down(0);
  }
  else {
    // 末尾に追加して上に上げる．
    ymuint pos = mVarNum;
    ++ mVarNum;
    mHeap[pos].mVar = var;
    mHeap[pos].mValue = value;
    move_up(pos);
  }
  // ハッシュに登録する．
  mHashTable.add(var);
}

// @brief 変数を適当な位置まで沈める．
// @param[in] pos 対象の変数の位置
void
VarPool::move_down(ymuint pos)
{
  ymuint idx = pos;
  for ( ; ; ) {
    // ヒープ木の性質から親の位置から子の位置が分かる．
    ymuint l_idx = idx * 2 + 1;
    ymuint r_idx = l_idx + 1;
    if ( r_idx > mVarNum ) {
      // 左右の子供を持たない時
      break;
    }
    Node p_node = mHeap[idx];
    Node l_node = mHeap[l_idx];
    if ( r_idx == mVarNum ) {
      // 右の子供を持たない時
      if ( compare(p_node, l_node) > 0 ) {
	// 逆転
	mHeap[l_idx] = p_node;
	mHeap[idx] = l_node;
      }
      // これ以上子供はいない．
      break;
    }
    else {
      // 左右の子供がいる場合
      Node r_node = mHeap[r_idx];
      if ( compare(p_node, l_node) > 0 &&
	   compare(l_node, r_node) <= 0 ) {
	// 左の子供と入れ替える．
	// 次は左の子供に対して同じ事をする．
	mHeap[l_idx] = p_node;
	mHeap[idx] = l_node;
	idx = l_idx;
      }
      else if ( compare(p_node, r_node) > 0 &&
		compare(r_node, l_node) < 0 ) {
	// 右の子供と入れ替える．
	// 次は右の子供に対して同じ事をする．
	mHeap[r_idx] = p_node;
	mHeap[idx] = r_node;
	idx = r_idx;
      }
      else {
	break;
      }
    }
  }
}

// @brief 変数を適当な位置まで浮かび上がらせる．
// @param[in] pos 対象の変数の位置
void
VarPool::move_up(ymuint pos)
{
  ymuint idx = pos;
  while ( idx > 0 ) {
    Node node = mHeap[idx];
    ymuint p_idx = (idx - 1) / 2;
    Node p_node = mHeap[p_idx];
    if ( compare(p_node, node) > 0 ) {
      mHeap[p_idx] = node;
      mHeap[idx] = p_node;
      idx = p_idx;
    }
    else {
      break;
    }
  }
}

// @brief 内容を出力する．
void
VarPool::print(ostream& s)
{
  s << "*** VarPool ***" << endl
    << " size() = " << size() << endl;
  for (ymuint i = 0; i < size(); ++ i) {
    const Variable& var1 = var(i);
    s << var1 << ": value = " << mHeap[i].mValue << endl;
  }
  s << endl;
}

END_NAMESPACE_IGF
