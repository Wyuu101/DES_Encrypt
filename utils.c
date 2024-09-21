#include <utils.h>
#include <stdio.h>
//求出奇偶校验位
int parity(uint8_t byte){
    int count = 0;
    for(int i=0;i<7;i++){
        if(byte & (1 << i)){
            count++;
        }
    }
    return (count % 2 ==0) ? 1 : 0;
}
//设置奇偶校验位
uint64_t set_parity(uint64_t key){
    uint64_t result = 0;
    for(int i = 7 ; i >=0; i-- ){
        uint8_t byte = (key>> (i*8))&0x7F;
        byte = ((uint8_t)parity(byte)<<7)|byte;
        result <<= 8;
        result |= byte;
    }
    return result;
}

//输出数据的二进制形式
void print_bits(void *data, size_t size){
    uint8_t * bytes = (uint8_t*) data;
    for(int i=size-1;i>=0;i--){
        for(int j = 7;j>=0;j--){
            printf("%d",(bytes[i]>>j)&1);
        }
    };
    printf("\n");
}
