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

#include <cassert>
#include <iostream>
#include "PrefixSumLeaf.hpp"
#include "BitUtil.hpp"

using namespace std;

namespace prefixsum{

namespace {
static const uint64_t MAX_NUM = 256; // 128, 256, 512, 1024...
static const uint64_t BLOCK_NUM = MAX_NUM / 64;
}

PrefixSumLeaf::PrefixSumLeaf() : num_(0), width_(0){
}

PrefixSumLeaf::~PrefixSumLeaf() {
}

void PrefixSumLeaf::Init(uint64_t num){
  num_ = num;
}

void PrefixSumLeaf::Clear(){
  bit_arrays_.clear();
  num_ = 0;
  width_ = 0;
}

bool PrefixSumLeaf::IsFull() const{
  return num_ == MAX_NUM;
}

void PrefixSumLeaf::Rewidth(uint64_t width){
  vector<uint64_t> new_bit_arrays(width * BLOCK_NUM);
  for (uint64_t i = 0; i < BLOCK_NUM; ++i){
    for (uint64_t j = 0; j < width_; ++j){
      new_bit_arrays[i * width + j] = bit_arrays_[i * width_ + j];
    }
  }
  width_ = width;
  bit_arrays_.swap(new_bit_arrays);
}

void PrefixSumLeaf::Insert(uint64_t ind, uint64_t val){
  assert(ind < MAX_NUM);
  assert(num_ < MAX_NUM);
  uint64_t blen = BitUtil::GetBinaryLen(val);
  if (width_ < blen){
    Rewidth(blen);
  }

  uint64_t block = ind / 64;
  uint64_t offset = ind % 64;
  uint64_t mask = (1LLU << offset) - 1;
  uint64_t not_mask = ~mask;
  ++num_;
  const uint64_t block_num = (num_ + 64 - 1) / 64;
  for (uint64_t shift = 0; shift < width_; ++shift){
    uint64_t& bits = bit_arrays_[block * width_ + shift];
    uint64_t carry = bits >> (64 - 1);
    bits = (bits & mask) | ((bits & not_mask) << 1) | BitUtil::GetBit(val, shift) << offset;
    for (uint64_t i = block+1; i < block_num; ++i){
      uint64_t& next_bits = bit_arrays_[i * width_ + shift];
      uint64_t next_carry = next_bits >> (64-1);
      next_bits = (next_bits << 1) | carry;
      carry = next_carry;
    }
  }

}

void PrefixSumLeaf::Increment(uint64_t ind, uint64_t val){
  uint64_t block = ind / 64;
  uint64_t offset = ind % 64;
  const uint64_t set_bit = (1LLU << offset);
  uint64_t carry_bit = 0;
  for (uint64_t shift = 0;; ++shift){
    if ((val >> shift) == 0 && carry_bit == 0){
      return;
    }
    if (shift == width_){
      Rewidth(shift+1);
    }
    uint64_t val_bit = (val >> shift) & 1LLU;
    uint64_t& bits = bit_arrays_[block * width_ + shift];
    uint64_t cur_bit = BitUtil::GetBit(bits, offset);
    uint64_t sum = cur_bit + carry_bit + val_bit;
    
    if (cur_bit == 0 && (sum & 1LLU) == 1){
      bits |= set_bit;       // 0 -> 1
    } else if (cur_bit == 1 && (sum & 1LLU) == 0){
      bits &= ~set_bit;      // 1 -> 0
    }
    carry_bit = sum >> 1;
  }
  assert(false); // should not come here
}

void PrefixSumLeaf::Decrement(uint64_t ind, uint64_t val){
  uint64_t block = ind / 64;
  uint64_t offset = ind % 64;
  const uint64_t set_bit = (1LLU << offset);
  uint64_t carry_bit = 0;
  val = ~val + 1;
  for (uint64_t shift = 0; shift < width_; ++shift){
    uint64_t val_bit = (val >> shift) & 1LLU;
    uint64_t& bits = bit_arrays_[block * width_ + shift];
    uint64_t cur_bit = BitUtil::GetBit(bits, offset);
    uint64_t sum = cur_bit + carry_bit + val_bit;
    
    if (cur_bit == 0 && (sum & 1LLU) == 1){
      bits |= set_bit;       // 0 -> 1
    } else if (cur_bit == 1 && (sum & 1LLU) == 0){
      bits &= ~set_bit;      // 1 -> 0
    }
    carry_bit = sum >> 1;
  }
}

void PrefixSumLeaf::Set(uint64_t ind, uint64_t val){
  uint64_t block = ind / 64;
  uint64_t offset = ind % 64;
  const uint64_t unset_bit = ~(1LLU << offset);
  for (uint64_t shift = 0; shift < width_; ++shift){
    bit_arrays_[block * width_ + shift] &= unset_bit;
  }
  uint64_t blen = BitUtil::GetBinaryLen(val);
  if (width_ < blen){
    Rewidth(blen);
  }
  for (uint64_t shift = 0; shift < blen; ++shift){
    bit_arrays_[block * width_ + shift] |= (BitUtil::GetBit(val, shift) << offset);
  }
}

uint64_t PrefixSumLeaf::Get(uint64_t ind) const{
  uint64_t ret = 0;
  uint64_t block = ind / 64;
  uint64_t offset = ind % 64;
  for (uint64_t shift = 0; shift < width_; ++shift){
    ret += BitUtil::GetBit(bit_arrays_[block * width_ + shift], offset) << shift;
  }
  return ret;
}

uint64_t PrefixSumLeaf::GetBlockSum(uint64_t block, uint64_t offset) const {
  uint64_t ret = 0;
  uint64_t mask = (offset == 64) ? 0xFFFFFFFFFFFFFFFFLLU : ((1LLU << offset) - 1);
  for (uint64_t shift = 0; shift < width_; ++shift){
    ret += BitUtil::PopCount(bit_arrays_[block * width_ + shift] & mask) << shift;
  }
  return ret;
}

uint64_t PrefixSumLeaf::GetPrefixSum(uint64_t ind) const{
  uint64_t block = ind / 64;
  uint64_t offset = ind % 64;
  uint64_t ret = 0;
  for (uint64_t i = 0; i < block; ++i){
    ret += GetBlockSum(i, 64);
  }
  ret += GetBlockSum(block, offset);
  return ret;
}

namespace {
  static uint64_t masks[5] = 
    {0x5555555555555555LLU,
     0x3333333333333333LLU,
     0x0f0f0f0f0f0f0f0fLLU,
     0x00ff00ff00ff00ffLLU,
     0x0000ffff0000ffffLLU};
}

uint64_t PrefixSumLeaf::Find(uint64_t val) const{
  if (bit_arrays_.size() == 0) return 0;
  uint64_t block = 0;
  for ( ;num_ / 64 ; ++block){
    uint64_t sum = GetBlockSum(block, 64);
    if (val < sum) break;
    val -= sum;
  }
  assert(block * width_ < bit_arrays_.size());

  uint64_t cums[64][6];
  for (uint64_t shift = 0; shift < width_; ++shift){
    cums[shift][0] = bit_arrays_[block * width_ + shift];
  }

  for (uint64_t shift = 0; shift < width_; ++shift){
    uint64_t* cs = cums[shift];
    for (uint64_t i = 0, offset =1; i < 5; ++i, offset <<= 1){
      cs[i+1] = (cs[i] & masks[i]) + ((cs[i] >> offset) & masks[i]);
    }
  }
  
  uint64_t ind = 0;
  uint64_t sum = 0;
  for (uint64_t sums = 6; sums > 0; ){
    --sums;
    uint64_t psum = 0;
    for (uint64_t shift = 0; shift < width_; ++shift){
      psum += BitUtil::GetBits(cums[shift][sums], ind, 1LLU << sums) << shift;
    }
    if (sum + psum < val){
      sum += psum;
      ind += (1LLU << sums);
    }
  }
  if (sum + Get(ind) <= val){
    ++ind;
  }
  return block * 64 + ind;
}

void PrefixSumLeaf::Print() const{
  uint64_t block_num = (num_ + 64 - 1) / 64;
  for (uint64_t block = 0; block < block_num; ++block){
    for (uint64_t w = 0; w < width_; ++w){
      BitUtil::PrintBit(bit_arrays_[block * width_ + w]);
    }
  }
}

namespace {
uint64_t GetLeafWidth(uint64_t beg, uint64_t end, uint64_t width,
                       const vector<uint64_t>& bit_arrays){
  uint64_t max_w = 0;
  for (uint64_t block = beg; block < end; ++block){
    for (uint64_t w = 0; w < width; ++w){
      if (bit_arrays[block * width + w] > 0) max_w = w+1;
    }
  }
  return max_w;
}
}

void PrefixSumLeaf::Split(PrefixSumLeaf& ps){
  // assume num_ = MAX_NUM
  uint64_t first_leaf_width = GetLeafWidth(0, BLOCK_NUM/2, width_, bit_arrays_);
  uint64_t second_leaf_width = GetLeafWidth(BLOCK_NUM/2, BLOCK_NUM, width_, bit_arrays_);

  vector<uint64_t> first_bit_arrays(first_leaf_width * BLOCK_NUM);
  vector<uint64_t> second_bit_arrays(second_leaf_width * BLOCK_NUM);

  for (uint64_t i = 0; i < BLOCK_NUM/2; ++i){
    for (uint64_t j = 0; j < first_leaf_width; ++j){
      first_bit_arrays[i * first_leaf_width + j] = bit_arrays_[i * width_ + j];
    }
  }
  for (uint64_t i = 0; i < BLOCK_NUM/2; ++i){
    for (uint64_t j = 0; j < second_leaf_width; ++j){
      second_bit_arrays[i * second_leaf_width + j] = bit_arrays_[(i + BLOCK_NUM/2) * width_ + j];
    }
  }

  num_ = num_ / 2;
  width_ = first_leaf_width;
  bit_arrays_.swap(first_bit_arrays);

  ps.num_ = MAX_NUM / 2;
  ps.width_ = second_leaf_width;
  ps.bit_arrays_.swap(second_bit_arrays);
}

uint64_t PrefixSumLeaf::GetAllocatedBytes() const{
  cout << "leaf=" << sizeof(bit_arrays_[0]) * bit_arrays_.size() << endl;
  return sizeof(bit_arrays_[0]) * bit_arrays_.size() + sizeof(num_) + sizeof(width_);
}

} // namespace prefixsum
