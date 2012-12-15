#include <iostream>
#include "../lib/PrefixSum.hpp"
#include "../lib/BitUtil.hpp"

using namespace std;

int main(int argc, char* argv[]){
  prefixsum::PrefixSum ps;
  uint64_t num = 1000000;
  uint64_t maxval = 100;
  uint64_t sum = 0;
  for (uint64_t i = 0; i < num; ++i){
    uint64_t val = rand() % maxval;
    uint64_t pos = rand() % (i+1);
    sum += val;
    ps.Insert(pos, val);
  }
  uint64_t alloc_bytes = ps.GetAllocatedBytes();
  uint64_t optimal_bytes = prefixsum::BitUtil::GetBinaryLen(maxval) * num / 8;
  cout << "             num " << num << endl
       << "             avg " << (float) sum / num << endl
       << "   optimal_bytes " << optimal_bytes << endl
       << " allocated_bytes " << alloc_bytes << endl
       << "           ratio " << (float)alloc_bytes / optimal_bytes << endl;
  return 0;
}
