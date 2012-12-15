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
#include "PrefixSumNode.hpp"

namespace prefixsum{

PrefixSumNode::PrefixSumNode() : left_size(0), left_sum(0), children(NULL), leaf(NULL){
}

PrefixSumNode::~PrefixSumNode(){
  Clear();
}

void PrefixSumNode::Clear(){
  if (children){
    delete children[0];
    delete children[1];
  }
  delete children;
  delete leaf;
  children = NULL;
  leaf = NULL;
  left_size = 0;
  left_sum = 0;
}



uint64_t PrefixSumNode::GetAllocatedBytes() const{
  uint64_t bytes = 0;
  if (children){
    bytes += children[0]->GetAllocatedBytes();
    bytes += children[1]->GetAllocatedBytes();
  } else {
    bytes += leaf->GetAllocatedBytes();
  }
  std::cout << "node:" << sizeof(left_size) + sizeof(left_sum) + 
    sizeof(children) + sizeof(leaf) << std::endl;
  return sizeof(left_size) + sizeof(left_sum) + 
    sizeof(children) + sizeof(leaf) + bytes;
}

} // namespace prefixsum

