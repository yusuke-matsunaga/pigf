#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os
import re


# オクテットをビットパタンに変換する
def o2s(b) :
    str = ""
    bint = int(b)
    for i in range(7, -1, -1) :
        if bint & (1 << i) :
            str += '1'
        else :
            str += '0'
    return str

# ビットパタンを表す文字列を作る．
def bits_to_str(b1, b2, b3, b4, mask) :
    tmp = o2s(b1) + o2s(b2) + o2s(b3) + o2s(b4)
    rmask = 32 - mask
    tmp2 = ""
    for i in range(31, -1, -1) :
        if i < rmask :
            tmp2 += '*'
        else :
            tmp2 += tmp[31 - i]
    return tmp2

pat_list_array = [ [] for i in range(0, 33) ]
count_array = [0 for i in range(0, 33)]

while True :
    line = sys.stdin.readline()
    if not line :
        break

    (head, idx_str) = line.split(' ')
    (pat, mask_str) = head.split('/')
    mask = int(mask_str)
    idx = int(idx_str)
    (b1, b2, b3, b4) = pat.split('.')
    bstr = bits_to_str(b1, b2, b3, b4, mask)

    pat_list_array[mask].append( (bstr, idx) )
    count_array[mask] += 1

for i in range(32, -1, -1) :
    n = count_array[i]
    if n > 0 :
        print("")
        print( "%d %d" % (i, n))
        for (bstr, idx) in pat_list_array[i] :
            print( bstr[0:i] )
