#! /usr/bin/env python3
#
# @file gauss_elim.py
# @brief ガウスの消去法を行うクラス
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016 Yusuke Matsunaga
# All rights reserved.


# @brief ガウスの消去法を行う．
#
# ただしこれはちょっと特殊でブール論理を対象にしている．
# 変数・係数は 0, 1 で排他的論理和を使う．
# 右辺の定数は常に 0 とする．
#
# m 個の n 次元(ブール)ベクトルが線形独立かどうかを
# 調べるために用いられる．
class GaussElim :

    # @brief 初期化を行う．
    def __init__(self, vec_list) :
        # データの sanity check を行う．
        ncols = len(vec_list)
        assert ncols > 0
        nrows = len(vec_list[0])
        for i in range(1, nrows) :
            assert len(vec_list[i]) == nrows

        self.mN = nrows
        self.mM = ncols
        self.mArray = [ [ 0 for _ in range(ncols) ] for _ in range(nrows) ]
        for j in range(ncols) :
            vec = vec_list[j]
            for i in range(nrows) :
                if vec[i] == 1 :
                    self.mArray[i][j] = 1

    # @brief 掃き出し法を実行する．
    def elim(self) :

        # 行をソートする．
        self.sort_row()

        # 一行ずつ処理をすすめる．
        for i in range(self.mN) :
            # i 行目の先頭の列を求める．
            for j in range(self.mM) :
                if self.mArray[i][j] == 1 :
                    pivot = j
                    break
            else :
                # すべて 0 の行だった．
                continue

            # i + 1 行目以降で pivot 列に要素を持つ行から
            # i 行を引く(排他的論理和だから足しても同じ)
            for i1 in range(i + 1, self.mN) :
                if self.mArray[i1][pivot] == 0 :
                    continue
                for j in range(self.mM) :
                    self.mArray[i1][j] ^= self.mArray[i][j]

        # 逆順に行をみて自明な解以外が存在するか調べる．
        for i in range(self.mN - 1, -1, -1) :
            c = 0
            for j in range(self.mM) :
                if self.mArray[i][j] == 1 :
                    c += 1
                    last_j = j

            if c > 1 :
                # 自明でない解があった．
                return False
            if c == 1 :
                # last_j を 0 にする．
                for i1 in range(self.mN) :
                    self.mArray[i1][last_j] = 0
        else :
            # 自明な解以外存在しなかった．
            return True

    # @brief 辞書順の降順に行をソートする．
    def sort_row(self) :
        for i in range(1, self.mN) :
            # この時点で 0 - (i - 1) 行まではソートされていると仮定する．
            # i 行の要素を挿入する位置を探す．
            for i1 in range(i) :
                if self.compare_row(i1, i) < 0 :
                    # i1 - (i - 1) までを一つ後ろにずらす．
                    tmp = self.mArray[i][:]
                    for i2 in range(i, i1, -1) :
                        for j in range(self.mM) :
                            val = self.mArray[i2 - 1][j]
                            self.mArray[i2][j] = val
                    # i1 の位置に i行の要素を入れる．
                    for j in range(self.mM) :
                        self.mArray[i1][j] = tmp[j]

    # @brief 行の比較を行う．
    def compare_row(self, i1, i2) :
        assert i1 >= 0 and i1 < self.mN
        assert i2 >= 0 and i2 < self.mN
        for j in range(self.mM) :
            val1 = self.mArray[i1][j]
            val2 = self.mArray[i2][j]
            if val1 < val2 :
                return -1
            elif val1 > val2 :
                return 1
            # 次のビットに進む
        else :
            # 最後まで同じだった．
            return 0

    # @brief 内容を表示する．
    def display(self) :
        for i in range(self.mN) :
            for j in range(self.mM) :
                print(' {}'.format(self.mArray[i][j]), end = '')
            print('')


if __name__ == '__main__' :

    vec_list = []
    vec_list.append([1, 1, 1])
    vec_list.append([0, 1, 1])
    vec_list.append([1, 0, 0])

    ge1 = GaussElim(vec_list)

    ans = ge1.elim()

    print('ge1.elim() = {}'.format(ans))

    vec_list = []
    vec_list.append([1, 1, 1])
    vec_list.append([0, 1, 1])
    vec_list.append([0, 0, 1])

    ge2 = GaussElim(vec_list)

    ans = ge2.elim()

    print('ge2.elim() = {}'.format(ans))
