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

#ifndef PREFIX_SUM_PREFIX_SUM_HPP_
#define PREFIX_SUM_PREFIX_SUM_HPP_

#include <vector>
#include <stdint.h>
#include "PrefixSumNode.hpp"

namespace prefixsum{

/**
 * Dynamic Succinct Prefix Sum Data Structure
 * Store integer arrrays vs[0...num_-1] compactly and support
 *   prefixsum(k)    : return \sum_{i=0}^{k} vs[i]
 *   find(i)         : return i s.t. prefixum(k) <= i < prefixsum(k+1)
 *   insert(i, x)    : vs <- vs[0...i-1] x vs[i ... num_-1]
 *   set(i, x)       : vs[i] <- x
 *   increment(i, x) : vs[i] <- vs[i] + 1
 *   decrement(i, x) : vs[i] <- vs[i] - 1
 */
class PrefixSum{
public:
  /**
   * Constructor
   */ 
  PrefixSum();
  
  /**
   * Destructor
   */ 
  ~PrefixSum();

  /**
   * Clear the internal state
   */
  void Clear();

  /**
   * Insert val between vs[ind-1] and vs[ind]
   */
  void Insert(uint64_t ind, uint64_t val);

  /**
   * Increment current value vs[ind] <- vs[ind] + 1
   */
  void Increment(uint64_t ind, uint64_t val);

  /**
   * Decrement current value vs[ind] <- vs[ind] - 1
   */
  void Decrement(uint64_t ind, uint64_t val);

  /**
   * Set vs[ind] <- val
   */
  void Set(uint64_t ind, uint64_t val);

  /**
   * Return vs[ind]
   */
  uint64_t Get(uint64_t ind) const;

  /**
   * Return vs[0] + vs[1] + ... + vs[ind-1]
   */
  uint64_t GetPrefixSum(uint64_t ind) const;

  /**
   * Return ind s.t. GetPrefixSum(ind) <= ind < GetPrefixSum(ind+1)
   */
  uint64_t Find(uint64_t val) const;

  /**
   * Return the number of interger nums
   */
  uint64_t Num() const{
    return num_;
  }

  /**
   * Return the sum of integers
   */
  uint64_t Sum() const {
    return sum_;
  }

  /**
   * Return the allocated bytes
   */
  uint64_t GetAllocatedBytes() const;

private:
  PrefixSumNode root_;
  uint64_t num_;
  uint64_t sum_;
};


} // namespace prefixsum

#endif // PREFIX_SUM_PREFIX_SUM_HPP_
