#include <stdint.h>
uint32_t S_box_change(uint64_t R_K);
uint64_t permute(uint64_t input, int* table,int table_len,int input_len);
uint64_t feistel(uint64_t data,uint64_t*son_keys);
uint32_t S_box_change(uint64_t R_K);
uint64_t encrypt(uint64_t data,uint64_t key);
uint64_t decrypt(uint64_t data,uint64_t key);
