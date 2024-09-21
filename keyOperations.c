#include <keyOperations.h>
#include <stdint.h>
#include <matrix.h>
#include <stdlib.h>
#include <stdio.h>
#include <main.h>
#include <utils.h>

//生成随机初始密钥
uint64_t generate_initial_key(){
    //避免使用弱密钥和半弱密钥
    uint64_t  week_keys[8] =
    {   
        0x0101010101010101,
        0xFEFEFEFEFEFEFEFE,
        0xE0E0E0E0F1F1F1F1,
        0x1F1F1F1F0E0E0E0E,
        0x0000000000000000,
        0xFFFFFFFFFFFFFFFF,
        0xE1E1E1E1F0F0F0F0,
        0x1E1E1E1E0F0F0F0F,
    };
    int count = 0;
    int again =0;
    while(count<=10){
    again=0;
    uint64_t key = ((uint64_t)rand() <<32) | rand();
    uint64_t key2 = set_parity(key);
    for(int i=0;i<8;i++){
        if(key2 == week_keys[i]){
            printf("检测到弱密钥，正在重新生成...\n");
            count++;
            again=1;
            break;
        }
    }
    if(again){
        continue;
    }
    return key2;
    }
}

//子密钥轮运算
uint64_t* feistel_key(uint64_t key){
    uint64_t *son_keys=(uint64_t*)malloc(16*sizeof(uint64_t));
    if(son_keys==NULL){
        printf("申请子密钥内存空间失败！");
        return NULL;
    } 
    uint64_t key0 = permute(key,PC_1,56,64);
    uint64_t C1 =(uint64_t) ((key0 >> 28) & 0xFFFFFFF);
    uint64_t D1 =(uint64_t) (key0 & 0xFFFFFFF);
    printf("运算生成16个子密钥:\n");
    for(int i = 0;i<16;i++){
        uint64_t C2 = ((C1<<SHIFT[i]) & 0xFFFFFFF) | ((C1<<SHIFT[i]) & 0x30000000)>>56;
        uint64_t D2 = ((D1<<SHIFT[i]) & 0xFFFFFFF) | ((D1<<SHIFT[i]) & 0x30000000)>>56;
        C1 = C2, D1 = D2;
        uint64_t key_i = permute(((C2<<28) | D2),PC_2,48,56);
        print_bits(&key_i,sizeof(key_i));
        son_keys[i] = key_i;
    }
    return son_keys;
}