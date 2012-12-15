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

#ifndef PREFIX_SUM_PREFIX_SUM_LEAF_HPP_
#define PREFIX_SUM_PREFIX_SUM_LEAF_HPP_

#include <vector>

namespace prefixsum{

class PrefixSumLeaf{
public:
  PrefixSumLeaf();
  ~PrefixSumLeaf();
  void Clear();
  void Init(uint64_t num);
  void Insert(uint64_t ind, uint64_t val);
  void Increment(uint64_t ind, uint64_t val);
  void Decrement(uint64_t ind, uint64_t val);
  void Set(uint64_t ind, uint64_t val);
  uint64_t Get(uint64_t ind) const;
  uint64_t GetPrefixSum(uint64_t ind) const;

  // return ind s.t. GetPrefixSum(ind) <= val < GetPrefixSum(ind+1)
  uint64_t Find(uint64_t val) const;

  uint16_t Num() const{
    return num_;
  }

  uint8_t Width() const{
    return width_;
  }

  uint64_t Sum() const {
    return GetPrefixSum(num_);
  }

  bool IsFull() const;
  void Print() const;
  void Split(PrefixSumLeaf& ps);
  uint64_t GetAllocatedBytes() const;

private:
  void Rewidth(uint64_t width);
  uint64_t GetBlockSum(uint64_t block, uint64_t offset) const;
  uint64_t GetWidth() const;
  void IncrementInternal(uint64_t ind, uint64_t val, bool plus);
  static uint64_t GetBinaryLen(uint64_t x);
  std::vector<uint64_t> bit_arrays_;
  uint16_t num_;
  uint8_t width_;
};


} // namespace prefixsum

#endif // PREFIX_SUM_PREFIX_SUM_LEAF_HPP_
