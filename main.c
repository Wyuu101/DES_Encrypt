#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <matrix.h>
#include <utils.h>
#include <keyOperations.h>
#include <main.h>
#include <conio.h>
#include <windows.h>

//置换函数
uint64_t permute(uint64_t input, int* table,int table_len,int input_len){
    uint64_t output = 0;
    for(int i = 0;i<table_len;i++){
        output <<= 1;
        output |= ((input >> (input_len-table[i])) & 1);
    }
    
    return output;
}

//轮运算
uint64_t feistel(uint64_t data,uint64_t*son_keys){
    uint64_t ip = permute(data,IP,64,64);
    printf("IP置换结果:\n");
    print_bits(&ip,sizeof(ip));
    uint64_t L = (uint64_t) ((ip >>32) & 0xFFFFFFFF);
    uint64_t R0 =(uint64_t) (ip & 0xFFFFFFFF);
    uint64_t R = permute(R0,E,48,32);
    printf("开始轮运算...\n");
    for(int i = 0;i<16;i++){
        uint64_t temp_L = L;
        L= permute(S_box_change(R ^ son_keys[i]),P,32,32);
        printf("L%d:",i);
        print_bits(&L,sizeof(L));
        R = temp_L;
    }
    uint64_t res = (L<<32 | R);
    printf("轮运算结果:\n");
    print_bits(&L,sizeof(L));
    return res;
}

//S盒变换
uint32_t S_box_change(uint64_t R_K){
    uint32_t S_output = 0;
    for(int i=0;i<8;i++){
        uint8_t S_output_i =(uint8_t) (R_K >> ((7-i)*6))  & 0x3F;
        uint8_t S_output_i_x = ((S_output_i & 0x20) >> 4 | (S_output_i & 0x01));
        uint8_t S_output_i_y = (S_output_i & 0x1E) >> 1;
        S_output <<= 4;
        S_output |=  (S[i][S_output_i_x][S_output_i_y] & 0x0F);
    }
    return S_output;
}
    

//加密
void encrypt(uint64_t data,uint64_t key){
    printf("明文(16进制): %016llx\n明文(2进制):",data);
    print_bits(&data,sizeof(data));
    printf("生成初始密钥: %llx\n",key);
    uint64_t*son_keys =feistel_key(key);
    uint64_t c_t_p = feistel(data,son_keys);
    printf("开始IP逆置换...\n");
    uint64_t c_t=permute(c_t_p,IP_INV,64,64);
    printf("加密完毕\n密文(16进制): %llx\n密文(2进制):",c_t);
    print_bits(&c_t,sizeof(c_t));
}

int main(){
    SetConsoleOutputCP(65001);
    srand(time(NULL));
    printf("正在随机生成64位明文...\n");
    uint64_t data =  (uint64_t)(rand()*9999)<<32 | rand()*9999;
    int key_mode = 1;
    printf("请选择密钥产生方式:\n[0]手动输入\n[任意键]自动生成\n");
    scanf("%d",&key_mode);
    uint64_t key ;
    if(key_mode==0){
        printf("请输入密钥:");
        scanf("%016llx",&key);
    }
    else{
        key =  generate_initial_key();
    }
    encrypt(data,key);
    printf("输入任意值退出...\n");
    getch();
    return 0;
}