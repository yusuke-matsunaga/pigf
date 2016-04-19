#! /usr/bin/env python3
#
# @file xorfunc.py
# @brief
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016 Yusuke Matsunaga
# All rights reserved.


class XorFunc :

    # @brief 初期化する．
    # @param[in] var_list 変数番号のリスト
    def __init__(self, var_list) :
        self._var_list = var_list

    # @brief 含んでいる変数のビットを1にしたビットベクタを作る．
    # @param[in] var_num 変数の総数
    def bitvect(self, var_num) :
        for vid in self._var_list :
            # vid のビットを1にする．
            pass
