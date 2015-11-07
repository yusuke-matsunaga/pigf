
/// @file RvMgr.cc
/// @brief RvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "SigFunc.h"
#include "FuncVect.h"
#include "YmUtils/HashFunc.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// HashFunc<RegVect*> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<nsIgf::RegVect*>
{
  ymuint
  operator()(nsIgf::RegVect* key) const
  {
    return key->hash();
  }
};

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス RvMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RvMgr::RvMgr()
{
  mVectSize = 0;
  mBlockSize = 0;
  mRvSize = 0;
  mAlloc = NULL;
}

// @brief デストラクタ
// @note このオブジェクトが確保したすべての RegVec が削除される．
RvMgr::~RvMgr()
{
  delete mAlloc;
}


BEGIN_NONAMESPACE

ymuint
read_num(const string& str,
	 ymuint& rpos)
{
  ymuint ans = 0;
  for ( ; ; ) {
    char c = str[rpos];
    ++ rpos;
    if ( c >= '0' && c <= '9' ) {
      ans = ans * 10 + static_cast<ymuint>(c - '0');
    }
    if ( c == ' ' || c == '\0' ) {
      return ans;
    }
  }
}

END_NONAMESPACE

// @brief データを読み込む．
// @param[in] s 読み込み元のストリーム演算子
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
RvMgr::read_data(istream& s)
{
  // 最初の行はベクタのサイズと要素数(残りの行数)
  string buf;
  getline(s, buf);

  ymuint rpos = 0;
  ymuint n = read_num(buf, rpos);
  ymuint k = read_num(buf, rpos);

  set_size(n);

  mVectList.clear();
  mVectList.reserve(k);

  HashSet<RegVect*> vect_hash;
  for (ymuint i = 0; i < k; ++ i) {
    string buf;
    if ( !getline(s, buf) ) {
      cerr << "read error" << endl;
      return false;
    }
    if ( buf.size() != n ) {
      cerr << "data length error at line " << (i + 1) << endl;
      return false;
    }
    for (ymuint j = 0; j < n; ++ j) {
      char c = buf[j];
      if ( c != '0' && c != '1' ) {
	cerr << "illegal charactor at line "
	     << (i + 1) << ", column " << (j + 1) << endl;
	return false;
      }
    }
    ymuint id = mVectList.size();
    RegVect* rv = new_vector(id);
    for (ymuint j = 0; j < n; ++ j) {
      ymuint blk = j / 64;
      ymuint sft = j % 64;
      char c = buf[j];
      if ( c == '1' ) {
	rv->mBody[blk] |= (1ULL << sft);
      }
    }
    if ( vect_hash.check(rv) ) {
      // 重複したデータ
      delete_vector(rv);
    }
    else {
      mVectList.push_back(rv);
      vect_hash.add(rv);
    }
  }

  return true;
}

// @brief 変数の価値を計算する．
// @param[in] var 変数
//
// 価値とはその変数で区別できる要素対の数
double
RvMgr::value(const Variable& var) const
{
  ymuint64 n0 = 0;
  ymuint64 n1 = 0;
  for (vector<const RegVect*>::const_iterator p = mVectList.begin();
       p != mVectList.end(); ++ p) {
    const RegVect* rv = *p;
    if ( rv->classify(var) ) {
      ++ n1;
    }
    else {
      ++ n0;
    }
  }
  ymuint64 nv = mVectList.size();
  ymuint64 n_ideal = (nv * nv) / 4;
  ymuint64 n = n0 * n1;
  return static_cast<double>(n) / static_cast<double>(n_ideal);
  //return static_cast<double>(n);
}

// @brief 変数対の価値を計算する．
// @param[in] var1, var2 変数
//
// 価値とはその変数で区別できる要素対の数
double
RvMgr::value(const Variable& var1,
	     const Variable& var2) const
{
  ymuint64 n00 = 0;
  ymuint64 n01 = 0;
  ymuint64 n10 = 0;
  ymuint64 n11 = 0;
  for (vector<const RegVect*>::const_iterator p = mVectList.begin();
       p != mVectList.end(); ++ p) {
    const RegVect* rv = *p;
    if ( rv->classify(var1) ) {
      if ( rv->classify(var2) ) {
	++ n11;
      }
      else {
	++ n10;
      }
    }
    else {
      if ( rv->classify(var2) ) {
	++ n01;
      }
      else {
	++ n00;
      }
    }
  }
  ymuint64 nv = mVectList.size();
  ymuint64 n_ideal = (nv * nv * 6) / 16;
  ymuint64 n = n00 * (n01 + n10 + n11) + n01 * (n10 + n11) + n10 * n11;
  return static_cast<double>(n) / static_cast<double>(n_ideal);
  //return static_cast<double>(n);
}

// @brief インデックスのサイズを得る．
ymuint
RvMgr::index_size() const
{
  ymuint k = mVectList.size();
  ++ k;
  ymuint ans = 0;
  ymuint m = 1;
  while ( m < k ) {
    ++ ans;
    m <<= 1;
  }
  return ans;
}

// @brief ベクタのサイズを設定する．
// @note 付随するいくつかの処理を行う．
void
RvMgr::set_size(ymuint size)
{
  ASSERT_COND( mVectSize == 0 );
  mVectSize = size;
  mBlockSize = (size + 63) / 64;
  mRvSize = sizeof(RegVect) + 8 * (mBlockSize - 1);
  mAlloc = new UnitAlloc(mRvSize, 1024);
}

// @brief ベクタを作る．
// @param[in] index インデックス
RegVect*
RvMgr::new_vector(ymuint index)
{
  void* p = mAlloc->get_memory(mRvSize);
  RegVect* rv = new (p) RegVect(mVectSize, index);

  // 最初に0に初期化しておく．
  for (ymuint i = 0; i < mBlockSize; ++ i) {
    rv->mBody[i] = 0ULL;
  }

  return rv;
}

// @brief ベクタを削除する．
// @param[in] vec 削除対象のベクタ
void
RvMgr::delete_vector(RegVect* vec)
{
  mAlloc->put_memory(mRvSize, vec);
}

// @brief ベクタにハッシュ関数を適用した結果を作る．
// @param[in] hash_func ハッシュ関数
// @return ハッシュ値のベクタ
FuncVect*
RvMgr::gen_hash_vect(const SigFunc& hash_func) const
{
  ymuint nv = mVectList.size();
  ymuint p = hash_func.output_width();
  ymuint exp_p = 1U << p;

  FuncVect* fv = new FuncVect(nv, exp_p);
  for (ymuint i = 0; i < nv; ++ i) {
    const RegVect* rv = mVectList[i];
    fv->set_val(i, hash_func.eval(rv));
  }
  return fv;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
RvMgr::dump(ostream& s) const
{
  for (vector<const RegVect*>::const_iterator p = mVectList.begin();
       p != mVectList.end(); ++ p) {
    const RegVect* rv = *p;
    rv->dump(s);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス RegVect
//////////////////////////////////////////////////////////////////////

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

// @brief 分類する．
// @param[in] var 分類用の変数
//
// 0 か 1 を返す．
ymuint
RegVect::classify(const Variable& var) const
{
  ymuint ans = 0;
  ymuint nblk = (size() + 63) / 64;
  for (ymuint i = 0; i < nblk; ++ i) {
    ymuint64 tmp = var.raw_data(i) & mBody[i];
    ans ^= parity64(tmp);
  }
  return ans;
}

// @brief ハッシュ値を返す．
ymuint
RegVect::hash() const
{
  ymuint val = 0;
  ymuint nblk = (size() + 63) / 64;
  for (ymuint i = 0; i < nblk; ++ i) {
    ymuint64 tmp = mBody[i];
    ymuint val1 = tmp & 0xFFFFFFFFUL;
    ymuint val2 = (tmp >> 32) & 0xFFFFFFFFUL;
    val = val * 13 + val1 * 7 + val2;
  }
  return val;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
RegVect::dump(ostream& s) const
{
  s << setw(8) << index() << ": ";
  for (ymuint i = 0; i < size(); ++ i) {
    s << val(i);
  }
  s << endl;
}

END_NAMESPACE_YM_IGF
