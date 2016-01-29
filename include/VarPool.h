#ifndef VARPOOL_H
#define VARPOOL_H

/// @file VarPool.h
/// @brief VarPool のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "Variable.h"
#include "YmUtils/HashSet.h"

BEGIN_NAMESPACE_YM

template<>
struct
HashFunc<nsIgf::Variable>
{
  ymuint
  operator()(const nsIgf::Variable& key) const
  {
    return key.hash();
  }
};

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class VarPool VarPool.h "VarPool.h"
/// @brief 一定数の変数を貯めておくデータ構造
///
/// 溢れたら価値の最も低いものを捨てる．
//////////////////////////////////////////////////////////////////////
class VarPool
{
public:

  /// @brief コンストラクタ
  /// @param[in] num 確保するノード数
  VarPool(ymuint num);

  /// @brief デストラクタ
  ~VarPool();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 保持している変数の数を返す．
  ymuint
  size() const;

  /// @brief 変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  const Variable&
  var(ymuint pos) const;

  /// @brief 価値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  double
  value(ymuint pos) const;

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const;

  /// @brief 変数を追加する．
  /// @param[in] var 追加する変数
  /// @param[in] value 価値
  ///
  /// 容量オーバーのときは最も価値の低い変数を捨てる．
  void
  put(const Variable& var,
      double value);

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  struct Node
  {
    Variable mVar;
    double mValue;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  void
  pop_min();

  /// @brief 変数を適当な位置まで沈める．
  /// @param[in] pos 対象の変数の位置
  void
  move_down(ymuint pos);

  /// @brief 変数を適当な位置まで浮かび上がらせる．
  /// @param[in] var 対象の変数の位置
  void
  move_up(ymuint pos);

  /// @brief 変数の比較を行う．
  /// @param[in] node1, node2 対象のノード
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  static
  int
  compare(const Node& node1,
	  const Node& node2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mHeap の実際の大きさ
  ymuint mHeapSize;

  // 変数のヒープ木
  Node* mHeap;

  // ヒープ木中にある変数の数
  ymuint mVarNum;

  // ハッシュテーブル
  HashSet<Variable> mHashTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 確保されている変数の数を返す．
inline
ymuint
VarPool::size() const
{
  return mVarNum;
}

// @brief 変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < size() )
inline
const Variable&
VarPool::var(ymuint pos) const
{
  ASSERT_COND( pos < size() );
  return mHeap[pos].mVar;
}

// @brief 価値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < size() )
inline
double
VarPool::value(ymuint pos) const
{
  return mHeap[pos].mValue;
}

// @brief ヒープが空の時 true を返す．
inline
bool
VarPool::empty() const
{
  return mVarNum == 0;
}

// @brief 要素の比較を行う．
// @param[in] node1, node2 対象の要素
// @retval 負の値 node1 が node2 より前にある．
// @retval 0 node1 と node2 は同じ
// @retval 正の値 node1 が node2 より後ろにある．
inline
int
VarPool::compare(const Node& node1,
		 const Node& node2)
{
  if ( node1.mValue > node2.mValue ) {
    return -1;
  }
  else if ( node1.mValue < node2.mValue ) {
    return 1;
  }
  else {
    return 0;
  }
}

END_NAMESPACE_YM_IGF

#endif // VARPOOL_H
