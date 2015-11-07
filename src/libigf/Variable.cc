
/// @file Variable.cc
/// @brief Variable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Variable.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

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

// @brief デストラクタ
Variable::~Variable()
{
  delete [] mBitVect;
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

#if 0
BEGIN_NONAMESPACE

inline
ymuint
parity64(ymuint64 data)
{
  ymuint64 data_a = data & 0x00000000FFFFFFFFULL;
  ymuint64 data_b = data >> 32;
  data = data_a ^ data_b;

  data_a = data & 0x000000000000FFFFULL;
  data_b = data >> 16;
  data = data_a ^ data_b;

  data_a = data & 0x00000000000000FFULL;
  data_b = data >> 8;
  data = data_a ^ data_b;

  data_a = data & 0x000000000000000FULL;
  data_b = data >> 4;
  data = data_a ^ data_b;

  data_a = data & 0x0000000000000007ULL;
  data_b = data >> 3;
  data = data_a ^ data_b;

  data_a = data & 0x0000000000000003ULL;
  data_b = data >> 2;
  data = data_a ^ data_b;

  data_a = data & 0x0000000000000001ULL;
  data_b = data >> 1;
  data = data_a ^ data_b;

  return data;
}

END_NONAMESPACE

// @brief ベクタを分類する．
// @param[in] vect 対象のベクタ
ymuint
Variable::classify(const RegVect* vect) const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < nblk(); ++ i) {
    ymuint64 tmp = mBitVect[i] & vect->raw_data(i);
    ans ^= parity64(tmp);
  }
  return ans;
}
#endif

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

END_NAMESPACE_YM_IGF
