/* 
 *  Copyright (c) 2012 Daisuke Okanohara
  * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "PrefixSumLeaf.hpp"

using namespace std;
using namespace prefixsum;

TEST(PrefixSumLeaf, trivial){
  PrefixSumLeaf ps;
  ASSERT_EQ(0, ps.Num());
  ps.Insert(0, 0);
  ASSERT_EQ(1, ps.Num());
  ps.Increment(0, 1);
  ASSERT_EQ(1, ps.GetPrefixSum(1));
  ASSERT_EQ(0, ps.Find(0));
  ASSERT_EQ(1, ps.Find(1));
  ps.Clear();
  ASSERT_EQ(0, ps.Num());
}

TEST(PrefixSumLeaf, small){
  PrefixSumLeaf ps;
  ASSERT_EQ(0, ps.Num());
  vector<uint64_t> vs;
  int n = 128;
  for (uint64_t i = 0; i < n; ++i){
    uint64_t pos = rand() % (i+1);
    uint64_t val = rand() % 100;
    ps.Insert(pos, val);
    vs.push_back(0);
    for (uint64_t j = i; j > pos; --j){
      vs[j] = vs[j-1];
    }
    vs[pos] = val;
  }
  ASSERT_EQ(n, ps.Num());
  for (uint64_t i = 0; i < n; ++i){
    ASSERT_EQ(vs[i], ps.Get(i)) << " i=" << i << endl;
  }
}

TEST(PrefixSumLeaf, Increment){
  PrefixSumLeaf ps;
  ps.Init(64);
  vector<uint64_t> vals(64);
  for (uint64_t i = 0; i < 1000; ++i){
    uint64_t ind = rand() % 64;
    uint64_t val = rand() % 1000;
    ps.Increment(ind, val);
    vals[ind] += val;
  }

  for (uint64_t i = 0; i < 64; ++i){
    ASSERT_EQ(vals[i], ps.Get(i));
  }
}

TEST(PrefixSumLeaf, DecrementRandom){
  PrefixSumLeaf ps;
  ps.Init(64);
  vector<uint64_t> vals(64);
  for (uint64_t i = 0; i < 64; ++i){
    ps.Set(i, 100000);
    vals[i] = 100000;
  }

  for (uint64_t i = 0; i < 1000; ++i){
    uint64_t ind = rand() % 64;
    uint64_t val = rand() % 1000;
    val = min(val, vals[ind]);
    ps.Decrement(ind, val);
    vals[ind] -= val;
  }

  for (uint64_t i = 0; i < 64; ++i){
    ASSERT_EQ(vals[i], ps.Get(i)) << " ind=" << i;
  }
}


TEST(PrefixSumLeaf, Decrement){
  PrefixSumLeaf ps;
  ps.Insert(0, 888);
  ps.Insert(0, 100);
  ASSERT_EQ(100, ps.Get(0));
  ASSERT_EQ(888, ps.Get(1));
  ps.Decrement(0, 77);
  ASSERT_EQ(23, ps.Get(0));
  ps.Decrement(1, 777);
  ASSERT_EQ(111, ps.Get(1));
}




TEST(PrefixSumLeaf, Find){
  PrefixSumLeaf ps;
  ps.Insert(0, 5);
  ASSERT_EQ(0, ps.GetPrefixSum(0));
  ASSERT_EQ(5, ps.GetPrefixSum(1));
  
  ASSERT_EQ(0, ps.Find(0));
  ASSERT_EQ(0, ps.Find(1));
  ASSERT_EQ(0, ps.Find(2));
  ASSERT_EQ(0, ps.Find(3));
  ASSERT_EQ(0, ps.Find(4));
  ASSERT_EQ(1, ps.Find(5));
}

TEST(PrefixSumLeaf, find){
  PrefixSumLeaf ps;
  ps.Insert(0, 2);
  ps.Insert(1, 4);
  ps.Insert(2, 1);
  ASSERT_EQ(0, ps.GetPrefixSum(0));
  ASSERT_EQ(2, ps.GetPrefixSum(1));
  ASSERT_EQ(6, ps.GetPrefixSum(2));
  ASSERT_EQ(7, ps.GetPrefixSum(3));
  
  ASSERT_EQ(0, ps.Find(0));
  ASSERT_EQ(0, ps.Find(1));
  ASSERT_EQ(1, ps.Find(2));
  ASSERT_EQ(1, ps.Find(3));
  ASSERT_EQ(1, ps.Find(4));
  ASSERT_EQ(1, ps.Find(5));
  ASSERT_EQ(2, ps.Find(6));
  ASSERT_EQ(3, ps.Find(7));
}


TEST(PrefixSumLeaf, large_value){
  PrefixSumLeaf ps;
  ps.Insert(0, 100);
  ps.Insert(1, 1000);
  ps.Insert(2, 10000);

  ASSERT_EQ(100, ps.Get(0));
  ASSERT_EQ(1000, ps.Get(1));
  ASSERT_EQ(10000, ps.Get(2));
  
  ASSERT_EQ(0, ps.GetPrefixSum(0));
  ASSERT_EQ(100, ps.GetPrefixSum(1));
  ASSERT_EQ(1100, ps.GetPrefixSum(2));
  ASSERT_EQ(11100, ps.GetPrefixSum(3));

  ps.Insert(1, 77777);

  ASSERT_EQ(100,   ps.Get(0));
  ASSERT_EQ(77777, ps.Get(1));
  ASSERT_EQ(1000,  ps.Get(2));
  ASSERT_EQ(10000, ps.Get(3));

  ASSERT_EQ(0, ps.GetPrefixSum(0));
  ASSERT_EQ(100, ps.GetPrefixSum(1));
  ASSERT_EQ(77877, ps.GetPrefixSum(2));
  ASSERT_EQ(78877, ps.GetPrefixSum(3));
  ASSERT_EQ(88877, ps.GetPrefixSum(4));
 
  ps.Increment(0, 2);
  ps.Increment(0, 3);
  ps.Increment(2, 10);
  ps.Increment(2, 100);
  ps.Increment(2, 1);

  ASSERT_EQ(105,   ps.Get(0));
  ASSERT_EQ(77777, ps.Get(1));
  ASSERT_EQ(1111,  ps.Get(2));
  ASSERT_EQ(10000, ps.Get(3));
}

TEST(PrefixSumLeaf, large){
  uint64_t N = 64;
  vector<uint64_t> vals(N);
  PrefixSumLeaf ps;
  ps.Init(N);

  for (uint64_t i = 0; i < N; ++i){
    uint64_t val = rand();
    ps.Set(i, val);
    vals[i] = val;
  }
  vector<uint64_t> cums(N+1);
  uint64_t cum = 0;
  for (uint64_t i = 0; i < vals.size(); ++i){
    cums[i] = cum;
    cum += vals[i];
  }
  cums[N] = cum;
  
  for (uint64_t i = 0; i < N; ++i){
    ASSERT_EQ(vals[i], ps.Get(i)) << " i=" << i;
    ASSERT_EQ(cums[i], ps.GetPrefixSum(i)) << " i=" << i;
  }
}

TEST(PrefixSumLeaf, random){
  vector<uint64_t> vals(64);
  PrefixSumLeaf ps;
  ps.Init(64);

  for (uint64_t i = 0; i < 10000; ++i){
    uint64_t ind = rand() % 64;
    uint64_t val = rand();
    ps.Set(ind, val);
    vals[ind] = val;
  }

  vector<uint64_t> cums(64+1);
  uint64_t cum = 0;
  for (uint64_t i = 0; i < vals.size(); ++i){
    cums[i] = cum;
    cum += vals[i];
  }
  cums[64] = cum;

  for (uint64_t i = 0; i < 64; ++i){
    ASSERT_EQ(vals[i], ps.Get(i));
    ASSERT_EQ(cums[i], ps.GetPrefixSum(i));
  }
  
  for (uint64_t i = 0; i < 1000; ++i){
    uint64_t v = rand() % (cum + 1);
    uint64_t ind = ps.Find(v);
    ASSERT_LE(cums[ind], v)   << " ind=" << ind;
    ASSERT_LT(v, cums[ind+1]) << " ind=" << ind;
  }
}
