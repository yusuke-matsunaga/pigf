
/// @file RegVectTest.cc
/// @brief RegVectTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "RvMgr.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

const char* src_data[] = {
  "0001001101111000",
  "0001010000000101",
  "0001010010010001",
  "0001011100100110",
  "0001011100110111",
  "0001011101100100",
  "0001011101110101",
  "0001011101110110",
  "0001100000101000",
  "0001100001000000",
  "0001100001000100",
  "0001100001000110",
  "0001100100001001",
  "0001100100110101",
  "0001100100111000",
  "0001100101001000",
  "0001100101100000",
  "0001100101100101",
  "0001100101110001",
  "0001100110000111",
  "0010000000000110",
  "0010000100010010",
  "0010000100010100",
  "0010000100010111",
  "0010000101010011",
  "0010000101101001",
  "0010000110011000",
  "0010001000001000",
  "0010001000001001",
  "0010001000010110",
  "0010001010010001",
  "0010001100100101",
  "0010001100100110",
  "0010001100110101",
  "0010001101010100",
  "0010010000010000",
  "0010010000010010",
  "0010010000010111",
  "0010010000011001",
  "0010010000100111",
  "0010010001000101",
  "0010010001110101",
  "0010010010011001",
  "0010010101110011",
  "0010011000001000",
  "0010011000010010",
  "0010011001010101",
  "0010011001100101",
  "0010011001110101",
  "0010011010000011",
  "0010011010001001",
  "0010011100011001",
  "0010011101100010",
  "0010011110000000",
  "0010100000000100",
  "0010100000000101",
  "0010100000000110",
  "0010100000010110",
  "0010100000011000",
  "0010100000110000",
  "0010100000110001",
  "0010100001110010",
  "0010100010010010",
  "0010100010010100",
  "0010100010011000",
  "0010100100000111",
  "0010100100100011",
  "0010100100100100",
  "0011000000010000",
  "0011000000010001",
  "0011000000110100",
  "0011000001011001",
  "0011000100010001",
  "0011000100010011",
  "0011000100100011",
  "0011000101100001",
  "0011000101101000",
  "0011001000111000",
  "0011001001010100",
  "0011001001110001",
  "0011001100000110",
  "0011001100100000",
  "0011001101010101",
  "0011001101100110",
  "0011001101110111",
  "0011001110010010",
  "0011010100101000",
  "0011010101110001",
  "0011010110000011",
  "0011010110010111",
  "0011011000000100",
  "0011011001000000",
  "0011011001000100",
  "0011011001001000",
  "0011011001010100",
  "0011011001100011",
  "0011011101010000",
  "0011011101100010",
  "0011011110011001",
  "0011100000101000",
  "0011100101000101",
  "0011100101000111",
  "0011100101010001",
  "0011100101010101",
  "0100000000110011",
  "0100000010000010",
  "0100000010010011",
  "0100000010010100",
  "0100000100000111",
  "0100000100010101",
  "0100000100010111",
  "0100000100011001",
  "0100001000100010",
  "0100001000100100",
  "0100001000111000",
  "0100001010010001",
  "0100001010011001",
  "0100001100100011",
  "0100001100110011",
  "0100001101000010",
  "0100001101100001",
  "0100001101100100",
  "0100010000001001",
  "0100010001100010",
  "0100010001100100",
  "0100010001100101",
  "0100010100100100",
  "0100010100100110",
  "0100010101010010",
  "0100011000110101",
  "0100011000110110",
  "0100011001000010",
  "0100011010010101",
  "0100011100010011",
  "0100011100011000",
  "0100011110000001",
  "0100011110011000",
  "0100100000100000",
  "0100100000101001",
  "0100100000110010",
  "0100100100100110",
  "0100100101010010",
  "0100100101010101",
  "0100100101010111",
  "0100100101110111",
  "0100100110000111",
  "0100100110010000",
  "0100100110011000",
  "0100100110011001",
  "0101000000001000",
  "0101000000010000",
  "0101000100000011",
  "0101000100000100",
  "0101000101100001",
  "0101000110001001",
  "0101000110010100",
  "0101001001101000",
  "0101001001110001",
  "0101001010000010",
  "0101001010001000",
  "0101001010001001",
  "0101001101010101",
  "0101001110000110",
  "0101001110010000",
  "0101001110010101",
  "0101010001000110",
  "0101010001011000",
  "0101010101100110",
  "0101011000000101",
  "0101011000001001",
  "0101011000010000",
  "0101011000010100",
  "0101011001000001",
  "0101011001010111",
  "0101011001100000",
  "0101011100101001",
  "0101011101010011",
  "0101100000010110",
  "0101100001010010",
  "0101100001010110",
  "0101100100000101",
  "0101100100011000",
  "0101100100100001",
  "0101100100100010",
  "0101100100110011",
  "0101100101000000",
  "0101100101010110",
  "0101100101100100",
  "0101100101100101",
  "0101100110000010",
  "0101100110000100",
  "0101100110010100",
  "0110000000100010",
  "0110000001000010",
  "0110000001010011",
  "0110000001010101",
  "0110000100010010",
  "0110000100100001",
  "0110000100100101",
  "0110000100110001",
  "0110000100110111",
  "0110000101000011",
  "0110000101000100",
  "0110000101010101",
  "0110001001110001",
  "0110001010000100",
  "0110001100000000",
  "0110001100100101",
  "0110001100110100",
  "0110001100110110",
  "0110001101000011",
  "0110001101010110",
  "0110001101100000",
  "0110001101100100",
  "0110001101100101",
  "0110001110000010",
  "0110001110010010",
  "0110001110010110",
  "0110010001000011",
  "0110010001100110",
  "0110010010010010",
  "0110010010010011",
  "0110010010010101",
  "0110010100010110",
  "0110010110010001",
  "0110011000111001",
  "0110011001000011",
  "0110011001000111",
  "0110011001010100",
  "0110011100001001",
  "0110011100011001",
  "0110011101000011",
  "0110011101000111",
  "0110011101110010",
  "0110011101110101",
  "0110011101110110",
  "0110011110000101",
  "0110011110010000",
  "0110100000100110",
  "0110100000110010",
  "0110100000110101",
  "0110100001000010",
  "0110100001001000",
  "0110100001111000",
  "0110100010000001",
  "0110100010010100",
  "0110100010010111",
  "0110100010011001",
  "0110100100100000",
  "0110100101000101",
  "0110100101000110",
  "0110100101010000",
  "0110100101010101",
  "0110100101100100",
  "0110100101110010",
  "0110100101110100",
  "0110100101110111",
  "0110100110000010",
  "0110100110010010",
  "0110100110010011",
  "0110100110010100",
  "0110100110011000",
  "0111000000011000",
  "0111000000100001",
  "0111000101001000",
  "0111001000010011",
  "0111001000110011",
  "0111001000110101",
  "0111001001000111",
  "0111001001010100",
  "0111001001110011",
  "0111001010000100",
  "0111001010000111",
  "0111001010010001",
  "0111001010010010",
  "0111001010011001",
  "0111010000000100",
  "0111010000001000",
  "0111010000010010",
  "0111010001110100",
  "0111010010000111",
  "0111010010010000",
  "0111010100000101",
  "0111010100100101",
  "0111010101000100",
  "0111010101100010",
  "0111010110010110",
  "0111011000000010",
  "0111011000000100",
  "0111011000100101",
  "0111011001000110",
  "0111011100000011",
  "0111011100000101",
  "0111011100011001",
  "0111011100100110",
  "0111011100100111",
  "0111011101000111",
  "0111011101011000",
  "0111011101101000",
  "0111100000100000",
  "0111100000100001",
  "0111100000110001",
  "0111100000111001",
  "0111100001000100",
  "0111100001010110",
  "0111100001011001",
  "0111100001111001",
  "0111100010010110",
  "0111100100000110",
  "0111100100011001",
  "0111100100100000",
  "0111100110000101",
  "0111100110001001",
  "0111100110010001",
  "0111100110010010",
  "1000000000000110",
  "1000000000100011",
  "1000000000110000",
  "1000000000111001",
  "1000000001000000",
  "1000000001000100",
  "1000000001000110",
  "1000000001100110",
  "1000000001111001",
  "1000000010000101",
  "1000000100000101",
  "1000000100111001",
  "1000000101000011",
  "1000000101010010",
  "1000000101010111",
  "1000000101100111",
  "1000000110011000",
  "1000001000000010",
  "1000001000100110",
  "1000001000110000",
  "1000001001000111",
  "1000001001001001",
  "1000001001010100",
  "1000010000010110",
  "1000011000100101",
  "1000011100110010",
  "1000011101110001",
  "1000100010010001",
  "1000100100110101",
  "1000100101000000",
  "1001000000010111",
  "1001000000101000",
  "1001000000101001",
  "1001000000110011",
  "1001000001010101",
  "1001000001010110",
  "1001000001011000",
  "1001000001011001",
  "1001000001100011",
  "1001000010000010",
  "1001000101110001",
  "1001000101110011",
  "1001000101111001",
  "1001000110010011",
  "1001001000000110",
  "1001001000110011",
  "1001001100001000",
  "1001001100010001",
  "1001001100010011",
  "1001001100011000",
  "1001001100100000",
  "1001001101010101",
  "1001001101100000",
  "1001001101100001",
  "1001001101110011",
  "1001001110000100",
  "1001010100110101",
  "1001010100110111",
  "1001010100111001",
  "1001010101000001",
  "1001010101000010",
  "1001010101000100",
  "1001011000010010",
  "1001011000100100",
  "1001011000101001",
  "1001011000110001",
  "1001011000110101",
  "1001011001100001",
  "1001011001110100",
  "1001011010000000",
  "1001011100000001",
  "1001011100000010",
  "1001011100001000",
  "1001011101001000",
  "1001011101100001",
  "1001011101100100",
  "1001011101100101",
  "1001011101100111",
  "1001011110010001",
  "1001011110010101",
  "1001011110010111",
  "1001011110011001",
  "1001100000010100",
  "1001100000011000",
  "1001100000100100",
  "1001100000110101",
  "1001100001000101",
  "1001100001000110",
  "1001100001010101",
  "1001100001010110",
  "1001100001110011",
  "1001100001111000",
  "1001100010011001",
  "1001100100000100",
  "1001100100011001",
  "1001100100100010",
  "1001100100101000",
  "1001100101010111",
  "1001100101011001",
  "1001100101100001",
  "1001100101100110",
  "1001100101101001",
  "1001100101110101",
  "1001100110000110"
};


TEST(RegVectTest, empty)
{
  RvMgr rv_mgr;

  const vector<const RegVect*>& vlist = rv_mgr.vect_list();
  EXPECT_EQ( 0, vlist.size() );
}

TEST(RegVectTest, read_data)
{
  RvMgr rv_mgr;

  string filename = "data1";

  ymuint n = sizeof(src_data) / sizeof(const char*);

  const char* data0 = src_data[0];
  ymuint bitlen = strlen(data0);
  {
    ofstream os(filename);
    ASSERT_FALSE( os.fail() );
    os << bitlen << " " << n << endl;
    for (ymuint i = 0; i < n; ++ i) {
      const char* data1 = src_data[i];
      os << data1 << endl;
    }
  }

  {
    ifstream s(filename);
    ASSERT_FALSE( s.fail() );

    bool stat = rv_mgr.read_data(s);
    EXPECT_TRUE( stat );
  }

  const vector<const RegVect*>& vlist = rv_mgr.vect_list();
  EXPECT_EQ( n, vlist.size() );
  EXPECT_EQ( bitlen, rv_mgr.vect_size() );

  for (ymuint i = 0; i < n; ++ i) {
    const RegVect* rv = vlist[i];
    const char* data1 = src_data[i];
    EXPECT_EQ( bitlen, rv->size() );
    EXPECT_EQ( i, rv->index() );
    for (ymuint j = 0; j < bitlen; ++ j) {
      ymuint val = rv->val(j);
      if ( data1[j] == '1' ) {
	EXPECT_EQ( 1, val );
      }
      else {
	EXPECT_EQ( 0, val );
      }
    }
  }
}

END_NAMESPACE_YM_IGF