﻿
/// @file Variable.cc
/// @brief Variable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Variable.h"
#include "RegVect.h"


BEGIN_NAMESPACE_IGF

//////////////////////////////////////////////////////////////////////
// クラス Variable
//////////////////////////////////////////////////////////////////////

// @brief ビットベクタのブロック数を返す．
inline
ymuint
Variable::nblk() const
{
  return (mVarNum + 63) / 64;
}

// @brief 変数のビット位置のブロック番号を返す．
// @param[in] vid 変数番号
inline
ymuint
Variable::blk(ymuint vid)
{
  return vid / 64;
}

// @brief 変数のビット位置のシフト数を返す．
// @param[in] vid 変数番号
inline
ymuint
Variable::sft(ymuint vid)
{
  return vid % 64;
}

// @brief 空のコンストラクタ
Variable::Variable() :
  mVarNum(0),
  mBitVect(nullptr)
{
}

// @brief 通常の変数用のコンストラクタ
// @param[in] var_num 変数の総数
// @param[in] vid 変数番号
Variable::Variable(ymuint var_num,
		   ymuint vid) :
  mVarNum(var_num)
{
  mBitVect = new ymuint64[nblk()];
  for (ymuint i = 0; i < nblk(); ++ i) {
    mBitVect[i] = 0ULL;
  }
  mBitVect[blk(vid)] |= (1ULL << sft(vid));
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
Variable::Variable(const Variable& src) :
  mVarNum(src.mVarNum)
{
  mBitVect = new ymuint64[nblk()];
  for (ymuint i = 0; i < nblk(); ++ i) {
    mBitVect[i] = src.mBitVect[i];
  }
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
const Variable&
Variable::operator=(const Variable& src)
{
  if ( &src != this ) {
    if ( mVarNum != src.mVarNum ) {
      mVarNum = src.mVarNum;
      delete [] mBitVect;
      mBitVect = new ymuint64[nblk()];
    }
    for (ymuint i = 0; i < nblk(); ++ i) {
      mBitVect[i] = src.mBitVect[i];
    }
  }
  return *this;
}

// @brief デストラクタ
Variable::~Variable()
{
  delete [] mBitVect;
}

// @brief 変数空間のサイズを返す．
ymuint
Variable::var_size() const
{
  return mVarNum;
}

// @brief 指定された変数を含んでいたら true を返す．
// @param[in] vid 変数番号 ( 0 <= vid < var_size() )
bool
Variable::check_var(ymuint vid) const
{
  ASSERT_COND( vid < var_size() );
  return static_cast<bool>((mBitVect[blk(vid)] >> sft(vid)) & 1UL);
}

// @brief 内容を表す変数番号のリストを返す．
//
// vector<> のコピーを作るので比較的コスト
// の高い演算
vector<ymuint>
Variable::vid_list() const
{
  vector<ymuint> vlist;
  for (ymuint i = 0; i < mVarNum; ++ i) {
    if ( mBitVect[blk(i)] & (1UL << sft(i)) ) {
      vlist.push_back(i);
    }
  }
  return vlist;
}

// @brief ビットベクタの生データを返す．
// @param[in] pos ブロック番号
ymuint64
Variable::raw_data(ymuint pos) const
{
  ASSERT_COND( pos < nblk() );
  return mBitVect[pos];
}

// @brief 合成演算
// @param[in] right オペランド
// @return 合成結果(自身への参照)を返す．
//
// 合成結果を自分に代入する intern 演算
const Variable&
Variable::operator*=(const Variable& right)
{
  ASSERT_COND( mVarNum == right.mVarNum );
  for (ymuint i = 0; i < nblk(); ++ i) {
    mBitVect[i] ^= right.mBitVect[i];
  }
  return *this;
}

// @relates Variable
// @brief 合成演算
// @param[in] left, right オペランド
Variable
operator*(const Variable& left,
	  const Variable& right)
{
  return Variable(left).operator*=(right);
}

// @brief 共通要素を持つとき true を返す．
// @param[in] right オペランド
bool
Variable::operator&&(const Variable& right) const
{
  ASSERT_COND( mVarNum == right.mVarNum );

  for (ymuint i = 0; i < nblk(); ++ i) {
    if ( (mBitVect[i] & right.mBitVect[i]) != 0ULL ) {
      return true;
    }
  }
  return false;
}

// @brief ベクタ集合に対する価値を計算する．
// @param[in] rv_list ベクタのリスト
// @return 価値を返す．
//
// 価値はこの変数で区別されるベクタ対の個数の
// 理想値に対する割合のこと．
double
Variable::value(const vector<const RegVect*>& rv_list) const
{
  ymuint64 n0 = 0;
  ymuint64 n1 = 0;
  for (vector<const RegVect*>::const_iterator p = rv_list.begin();
       p != rv_list.end(); ++ p) {
    const RegVect* rv = *p;
    if ( rv->classify(*this) ) {
      ++ n1;
    }
    else {
      ++ n0;
    }
  }
  ymuint64 nv = rv_list.size();
  ymuint64 n_ideal = (nv * nv) / 4;
  ymuint64 n = n0 * n1;
  return static_cast<double>(n) / static_cast<double>(n_ideal);
}

// @brief ハッシュ値を返す．
ymuint
Variable::hash() const
{
  ymuint ans = 0U;
  for (ymuint i = 0; i < nblk(); ++ i) {
    ans ^= mBitVect[i];
  }
  return ans;
}

// @brief 等価比較
// @param[in] right オペランド
// @return 等しい時 true を返す．
bool
Variable::operator==(const Variable& right) const
{
  ASSERT_COND( mVarNum == right.mVarNum );

  for (ymuint i = 0; i < nblk(); ++ i) {
    if ( mBitVect[i] != right.mBitVect[i] ) {
      return false;
    }
  }
  return true;
}

// @relates Variable
// @brief 非等価演算
// @param[in] left, right オペランド
// @return 異なるとき true を返す．
bool
operator!=(const Variable& left,
	   const Variable& right)
{
  return !left.operator==(right);
}

// @brief 内容を出力する．
void
Variable::dump(ostream& s) const
{
  vector<ymuint> vid_list = this->vid_list();

  if ( vid_list.size() == 1 ) {
    // 単一の変数の時
    s << vid_list[0];
  }
  else {
    // 合成変数の時
    s << "(";
    for (vector<ymuint>::const_iterator q = vid_list.begin();
	 q != vid_list.end(); ++ q) {
      s << " " << *q;
    }
    s << ")";
  }
}

// @relates Variable
// @brief ストリーム出力演算子
ostream&
operator<<(ostream& s,
	   const Variable& var)
{
  var.dump(s);
  return s;
}

END_NAMESPACE_IGF
