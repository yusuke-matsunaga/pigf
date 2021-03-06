﻿
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "RegVect.h"
#include "VectSetList.h"
#include "Variable.h"
#include <signal.h>

BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// 変数を並び替えるための比較関数
struct Lt
{
  bool
  operator()(const pair<ymuint, const Variable*>& left,
	     const pair<ymuint, const Variable*>& right)
  {
    return left.first < right.first;
  }
};

// 変数を並び替えるための比較関数
struct Gt
{
  bool
  operator()(const pair<ymuint, const Variable*>& left,
	     const pair<ymuint, const Variable*>& right)
  {
    return left.first > right.first;
  }
};

bool timeout = false;

void
timer_handler(int signum)
{
  cerr << "time expired" << endl;
  timeout = true;
}

END_NONAMESPACE

// @brief コンストラクタ
IguGen::IguGen()
{
  mNoChangeLimit = 0;
  mOrderingMode = 0;
  mTimeLimit = 0;
  mDebug = 0;
}

// @brief デストラクタ
IguGen::~IguGen()
{
}

// @brief ベクタのリストをセットする．
void
IguGen::set_vector_list(const vector<const RegVect*>& vector_list)
{
  mVectorList = vector_list;
}

// @brief 変数を求める．
// @param[in] multi 多重度
// @param[in] variable_list 変数のリスト
// @param[in] solution 解として選ばれた変数を格納するリスト
void
IguGen::solve(ymuint multi,
	      const vector<const Variable*>& variable_list,
	      ymuint best_so_far,
	      vector<const Variable*>& solution)
{
  ASSERT_COND( !mVectorList.empty() );
  mVectorLength = mVectorList[0]->size();

  // imbalance measure の昇順に並び替える．
  ymuint nv = variable_list.size();
  vector<pair<ymuint, const Variable*> > tmp_list(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    const Variable* var = variable_list[i];
    ymuint n0 = 0;
    ymuint n1 = 0;
    for (vector<const RegVect*>::const_iterator p = mVectorList.begin();
	 p != mVectorList.end(); ++ p) {
      const RegVect* vect = *p;
      if ( var->classify(vect) == 0 ) {
	++ n0;
      }
      else {
	++ n1;
      }
    }
    ymuint im = n0 * n0 + n1 * n1;
    tmp_list[i] = make_pair(im, var);
  }
  sort(tmp_list.begin(), tmp_list.end(), Lt());
  vector<const Variable*> new_variable_list(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    const Variable* var = tmp_list[i].second;
    new_variable_list[i] = var;
    if ( 0 ) {
      cout << "Variable:";
      var->dump(cout);
      cout << " " << tmp_list[i].first << endl;
    }
  }

  mMulti = multi;
  mBestSoFar = best_so_far;
  mSelectedVariables.clear();
  timeout = false;

  // タイマーの設定
  struct sigaction old_act;
  timer_t tid;
  if ( mTimeLimit > 0 ) {
    struct sigaction act;
    act.sa_handler = timer_handler;
    act.sa_flags = 0;
    if ( sigaction(SIGALRM, &act, &old_act) < 0 ) {
      perror("sigaction()");
      exit(-1);
    }

    struct itimerspec itval;
    itval.it_value.tv_sec = mTimeLimit;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec = 0;

    if ( timer_create(CLOCK_REALTIME, NULL, &tid) < 0 ) {
      perror("timer_create()");
      exit(-1);
    }
    if ( timer_settime(tid, 0, &itval, NULL) < 0 ) {
      perror("timer_settime()");
      exit(-1);
    }
  }

  VectSetList initial_list(mVectorList.size(), 1);
  initial_list.add_set();
  for (vector<const RegVect*>::const_iterator p = mVectorList.begin();
       p != mVectorList.end(); ++ p) {
    const RegVect* vect = *p;
    initial_list.add_vect(vect);
  }

  mBeforeHasSolution = true;
  mNoChangeCount = 0;
  mMaxNoChangeCount = 0;
  solve_recur(initial_list,
	      variable_list.begin(), variable_list.end());
  solution = mSolutionSoFar;

  if ( mTimeLimit > 0 ) {
    timer_delete(tid);
    sigaction(SIGALRM, &old_act, NULL);
  }
}

// @brief 再帰呼び出しの回数制限を設定する．
void
IguGen::set_recur_limit(ymuint limit)
{
  mNoChangeLimit = limit;
}

// @brief 時間制限を設定する．
// @param[in] limit_min 制限の分の部分
// @param[in] limit_sec 制限の秒の部分
void
IguGen::set_time_limit(ymuint limit_min,
		       ymuint limit_sec)
{
  mTimeLimit = limit_min * 60 + limit_sec;
}

// @brief 順序付けのヒューリスティックを指定する．
void
IguGen::set_ordering_mode(ymuint id)
{
  mOrderingMode = id;
}

// @brief デバッグレベルを指定する．
void
IguGen::set_debug_level(ymuint level)
{
  mDebug = level;
}

// @brief solve() の下請け関数
// @param[in] vector_list 分割されたベクタのリスト
// @param[in] var_begin 使用可能な変数のリストの先頭の反復子
// @param[in] var_end 使用可能な変数のリストの末尾の反復子
void
IguGen::solve_recur(const VectSetList& vector_list,
		    vector<const Variable*>::const_iterator var_begin,
		    vector<const Variable*>::const_iterator var_end)
{
  if ( mNoChangeLimit > 0 && mNoChangeCount > mNoChangeLimit ) {
    return;
  }

  if ( mMaxNoChangeCount < mNoChangeCount ) {
    mMaxNoChangeCount = mNoChangeCount;
  }

  if ( mDebug > 0 ) {
    cerr << "best so far = " << mBestSoFar << endl
	 << "selected variables = ";
    for (vector<const Variable*>::const_iterator p = mSelectedVariables.begin();
	 p != mSelectedVariables.end(); ++ p) {
      cerr << " ";
      const Variable* var = *p;
      var->dump(cerr);
    }
    cerr << endl;
    cerr << "remain variables = ";
    for (vector<const Variable*>::const_iterator p = var_begin;
	 p != var_end; ++ p) {
      cerr << " ";
      const Variable* var = *p;
      var->dump(cerr);
    }
    cerr << endl;
    if ( mDebug > 1 ) {
      ymuint n = vector_list.set_num();
      for (ymuint i = 0; i < n; ++ i) {
	ymuint m = vector_list.set_size(i);
	for (ymuint j = 0; j < m; ++ j) {
	  const RegVect* vect = vector_list.set_elem(i, j);
	  vect->dump(cerr);
	}
	cerr << "-----------------" << endl;
      }
    }
    else {
      ymuint n = vector_list.set_num();
      ymuint max_size = 0;
      for (ymuint i = 0; i < n; ++ i) {
	ymuint ssize = vector_list.set_size(i);
	if ( max_size < ssize ) {
	  max_size = ssize;
	}
      }
      vector<ymuint> hist_array(max_size + 1);
      for (ymuint i = 0; i < n; ++ i) {
	ymuint ssize = vector_list.set_size(i);
	++ hist_array[ssize];
      }

      cerr << "[";
      for (ymuint i = 0; i <= max_size; ++ i) {
	ymuint m = hist_array[i];
	if ( m > 0 ) {
	  cerr << " " << i << ": " << m;
	}
      }
      cerr << " ]" << endl;
    }
  }

  ymuint set_num = vector_list.set_num();

  // 残りの変数を全部使って所望の分割が行なえるか試す．
  ymuint nvars = var_end - var_begin;
  if ( nvars < 16 ) {
    ymuint nexp = 1U << nvars;
    bool overflow = false;
    for (ymuint set_id = 0; set_id < set_num; ++ set_id) {
      ymuint m = vector_list.set_size(set_id);
      vector<ymuint> n_array(nexp, 0);
      for (ymuint k = 0; k < m; ++ k) {
	const RegVect* vect = vector_list.set_elem(set_id, k);
	ymuint id = 0;
	for (ymuint i = 0; i < nvars; ++ i) {
	  const Variable* var = *(var_begin + i);
	  if ( var->classify(vect) == 1 ) {
	    id |= (1U << i);
	  }
	}
	++ n_array[id];
	if ( n_array[id] > mMulti ) {
	  overflow = true;
	  break;
	}
      }
      if ( overflow ) {
	break;
      }
    }
    if ( overflow ) {
      return;
    }
  }

  ymuint slack = mBestSoFar - mSelectedVariables.size();
  ASSERT_COND( slack > 1 );

  // ambiguity measure の昇順に並び替える．
  vector<pair<ymuint, const Variable*> > tmp_list;
  for (vector<const Variable*>::const_iterator p = var_begin;
       p != var_end; ++ p) {
    const Variable* var = *p;
    ymuint max_size = 0;
    ymuint am = 0;
    ymuint am2 = 0;
    ymuint am3 = 0;
    ymuint am4 = 0;
    bool split = false;
    for (ymuint set_id = 0; set_id < set_num; ++ set_id) {
      ymuint m = vector_list.set_size(set_id);
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (ymuint k = 0; k < m; ++ k) {
	const RegVect* vect = vector_list.set_elem(set_id, k);
	if ( var->classify(vect) == 0 ) {
	  ++ n0;
	}
	else {
	  ++ n1;
	}
      }
      if ( n0 == 0 ) {
	++ am4;
      }
      if ( n1 == 1 ) {
	++ am4;
      }
      if ( n0 > 0 && n1 > 0 ) {
	split = true;
      }
      if ( max_size < n0 ) {
	max_size = n0;
      }
      if ( max_size < n1 ) {
	max_size = n1;
      }
      if ( n0 > mMulti ) {
	am += n0 * n0;
	ymuint lb = lower_bound(n0);
	am2 += lb * lb;
	ymuint x = (n0 + mMulti - 1) / mMulti;
	am3 += x * x;
      }
      if ( n1 > mMulti ) {
	am += n1 * n1;
	ymuint lb = lower_bound(n1);
	am2 += lb * lb;
	ymuint x = (n1 + mMulti - 1) / mMulti;
	am3 += x * x;
      }
    }

    if ( !split ) {
      // 一つも区別できない変数はいらない．
      continue;
    }

    ymuint lb = lower_bound(max_size) + mSelectedVariables.size() + 1;
    if ( lb >= mBestSoFar ) {
      // 下界が現時点の最適解を下回らないので枝刈り
      continue;
    }

    ymuint val;
    switch ( mOrderingMode ) {
    case 0:
      val = am;
      break;

    case 1:
      if ( mBeforeHasSolution ) {
	val = am2;
      }
      else {
	val = am;
      }
      break;

    case 2:
      val = am2;
      break;

    case 3:
      val = am3;
      break;

    case 4:
      val = am4 * 10000 + am;
      break;

    default:
      val = am;
      break;
    }

    tmp_list.push_back(make_pair(val, var));
  }
  if ( tmp_list.empty() ) {
    // 使える変数が残っていなかった．
    return;
  }

  sort(tmp_list.begin(), tmp_list.end(), Lt());

  ymuint nv2 = tmp_list.size();

  vector<const Variable*> new_variables(nv2);
  for (ymuint i = 0; i < nv2; ++ i) {
    new_variables[i] = tmp_list[i].second;
  }

  ymuint bid = 0;
  for (vector<const Variable*>::const_iterator p = new_variables.begin();
       p != new_variables.end(); ++ p, ++ bid) {
    const Variable* var = *p;

    if ( mDebug > 0 ) {
      cout << "Choose Variable#" << (mSelectedVariables.size() + 1)
	   << ": ";
      var->dump(cout);
      cout << endl;
    }

    VectSetList new_vector_list(vector_list.all_elem_num(), set_num * 2);
    bool bounded = false;
    for (ymuint set_id = 0; set_id < set_num && !bounded; ++ set_id) {
      ymuint m = vector_list.set_size(set_id);
      if ( m <= mMulti ) {
	// 多重度以下に分割されているベクタセットは無視する．
	continue;
      }

      // 現在のベクタセットが var によって分割されるときの
      //各々の部分集合の数を数える．
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (ymuint k = 0; k < m; ++ k) {
	const RegVect* vect = vector_list.set_elem(set_id, k);
	if ( var->classify(vect) == 0 ) {
	  ++ n0;
	}
	else {
	  ++ n1;
	}
      }
      if ( n0 > mMulti ) {
	bool first = true;
	for (ymuint k = 0; k < m; ++ k) {
	  const RegVect* vect = vector_list.set_elem(set_id, k);
	  if ( var->classify(vect) == 0 ) {
	    if ( first ) {
	      first = false;
	      new_vector_list.add_set();
	    }
	    new_vector_list.add_vect(vect);
	  }
	}
      }
      if ( n1 > mMulti ) {
	bool first = true;
	for (ymuint k = 0; k < m; ++ k) {
	  const RegVect* vect = vector_list.set_elem(set_id, k);
	  if ( var->classify(vect) == 1 ) {
	    if ( first ) {
	      first = false;
	      new_vector_list.add_set();
	    }
	    new_vector_list.add_vect(vect);
	  }
	}
      }
    }

    if ( new_vector_list.set_num() == 0 ) {
      // すべて多重度以下に分割されていた．
      ymuint ans = mSelectedVariables.size() + 1;
      if ( ans < mBestSoFar ) {
	mSolutionSoFar = mSelectedVariables;
	mSolutionSoFar.push_back(var);
	mBestSoFar = ans;
	mBeforeHasSolution = false;
	mNoChangeCount = 0;
	if ( mDebug > 0 ) {
	  cerr << "best_so_far => " << mBestSoFar << endl;
	}
	// この再帰レベルでは mBestSoFar が更新される可能性はない．
	break;
      }
      continue;
    }

    if ( mBestSoFar > mSelectedVariables.size() + 2 ) {
      mSelectedVariables.push_back(var);
      ++ mNoChangeCount;
      solve_recur(new_vector_list, p + 1, new_variables.end());
      mSelectedVariables.pop_back();
    }

    if ( timeout ) {
      break;
    }
  }
}

// @brief 下界を計算する．
// @param[in] num 要素数
ymuint
IguGen::lower_bound(ymuint num) const
{
  ymuint n1 = (num + mMulti - 1) / mMulti;
  ymuint ans = 0;
  ymuint m = 1;
  while ( m < n1 ) {
    ++ ans;
    m <<= 1;
  }
  return ans;
}

END_NAMESPACE_YM_IGF
