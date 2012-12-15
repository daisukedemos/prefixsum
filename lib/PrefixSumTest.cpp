#include <gtest/gtest.h>
#include "PrefixSum.hpp"

using namespace std;
using namespace prefixsum;

TEST(PrefixSum, trivial){
  PrefixSum ps;
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

TEST(PrefixSum, Increment){
  PrefixSum ps;
  ps.Insert(0, 0);
  ps.Insert(1, 77777);
  ps.Increment(0, 555);
  ASSERT_EQ(555, ps.Get(0));
}

TEST(PrefixSum, Decrement){
  PrefixSum ps;
  ps.Insert(0, 888);
  ps.Insert(0, 100);
  ps.Decrement(0, 10);
  ASSERT_EQ(90, ps.Get(0));
  ps.Decrement(1, 111);
  ASSERT_EQ(777, ps.Get(1));
}


TEST(PrefixSum, Find){
  PrefixSum ps;
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

TEST(PrefixSum, find){
  PrefixSum ps;
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


TEST(PrefixSum, small){
  PrefixSum ps;
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
 
  ps.Increment(0, 55);
  ps.Increment(0, 55);
  ps.Increment(2, 1);
  ps.Increment(2, 2);
  ps.Increment(2, 3);

  ASSERT_EQ(210,   ps.Get(0));
  ASSERT_EQ(77777, ps.Get(1));
  ASSERT_EQ(1006,  ps.Get(2));
  ASSERT_EQ(10000, ps.Get(3));
}

TEST(PrefixSum, large){
  uint64_t N = 10000;
  vector<uint64_t> vals(N);
  PrefixSum ps;
  for (uint64_t i = 0; i < N; ++i){
    ps.Insert(0, 0);
  }

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

TEST(PrefixSum, random){
  vector<uint64_t> vals(10000);
  PrefixSum ps;
  uint64_t N = 10000;
  for (uint64_t i = 0; i < N; ++i){
    ps.Insert(0, 0);
  }

  for (uint64_t i = 0; i < 100000; ++i){
    uint64_t ind = rand() % N;
    uint64_t val = rand();
    ps.Set(ind, val);
    vals[ind] = val;
  }

  vector<uint64_t> cums(N+1);
  uint64_t cum = 0;
  for (uint64_t i = 0; i < vals.size(); ++i){
    cums[i] = cum;
    cum += vals[i];
  }
  cums[N] = cum;

  for (uint64_t i = 0; i < N; ++i){
    ASSERT_EQ(vals[i], ps.Get(i));
    ASSERT_EQ(cums[i], ps.GetPrefixSum(i));
  }
  
  for (uint64_t i = 0; i < 10000; ++i){
    uint64_t v = rand() % (cum + 1);
    uint64_t ind = ps.Find(v);
    ASSERT_LE(cums[ind], v)   << " ind=" << ind;
    ASSERT_LT(v, cums[ind+1]) << " ind=" << ind;
  }
}
