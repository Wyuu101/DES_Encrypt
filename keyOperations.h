#include <stdint.h>
uint64_t generate_initial_key();
uint64_t* feistel_key(uint64_t key);
uint64_t* get_sonkeys_decrypt(uint64_t* sonkeys_encrypt);