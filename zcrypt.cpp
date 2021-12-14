//实现RSA,AES,TEA,RC4
#define _CRT_SECURE_NO_DEPRECATE
#pragma comment(lib,"Winmm.lib") //for time usage: timeGetTime()
//这里是主函数入口，负责实际的输出
#include <Windows.h>
#include"zcrypt.h"

void Test_RSA() {
    //RSAINT测试
    //赋值测试
    rsa_uint b = 0x1400870389900455;
    //输出测试
    b.show_hex();
    std::cout << std::endl << "由缓存生成:" << std::endl;
    const char  *m = "//AESdefine AES_1280x10000#define AES_1920x20000#d\nefine AES_2560x30000#define AES_ECB0x00001#\ndefine AES_CBC0x00002#define AES_CFB	0x00003#define AES_OFB			0x00004";
    rsa_uint((uint8_t*)m, strlen(m)).show_hex();
    //比较测试
    auto c1 = (rsa_uint(0x123) == rsa_uint(0x123) ? "==: Yes" : "==: No");
    auto c2 = (rsa_uint(0x123) > rsa_uint(0x123) ? ">: Yes" : ">: No");
    auto c3 = (rsa_uint(0x123) >= rsa_uint(0x123) ? ">=: Yes" : ">=: No");
    std::cout << std::endl << c1;
    std::cout << std::endl << c2;
    std::cout << std::endl << c3;
    //类型转换
    // 
    std::cout << std::endl << "类型转换:";
    //std::cout << std::endl << uint64_t(rsa_uint(1223455555555555));

    //加法
    std::cout << std::endl << "加法测试:" << std::endl;
    rsa_uint a1 = 0xfffffff;
    rsa_uint a2 = 0xff01;
    rsa_uint sum = a1 + a2;
    //sum.show();
    (a1 + a2).show_hex();
    std::cout << std::endl;
    (rsa_uint::max_biguint() + 1).show_hex();
    std::cout << std::endl;

    //减法
    std::cout << std::endl << "减法测试:" << std::endl;
    (a1 - a2).show_hex();
    std::cout << std::endl;
    (BIGUINT(0) - BIGUINT(1)).show_hex();
    std::cout << std::endl;
    //自增
    std::cout << std::endl << "自增测试:" << std::endl;
    a2 += 3;
    a2.show_hex();
    std::cout << std::endl;
    ++a2;
    a2.show_hex();
    std::cout << std::endl;

    //移位，逻辑运算

    std::cout << std::endl << "移位测试:" << std::endl;
    a1 = rsa_uint::binary_ones(10);
    a1.show_binary();
    std::cout << std::endl;
    (a1 >> 2).show_binary();
    std::cout << std::endl;
    (a1 >> 8).show_binary();
    std::cout << std::endl;
    (a1 >> 9).show_binary();
    std::cout << std::endl;
    (a1 >> 10).show_binary();
    std::cout << std::endl;
    (rsa_uint(5) ^ rsa_uint(3)).show_hex();
    std::cout << std::endl;

    a1 = ~rsa_uint::binary_ones(10);
    a1.show_binary();
    std::cout << std::endl;
    (a1 << 2).show_binary();
    std::cout << std::endl;
    (a1 << 8).show_binary();
    std::cout << std::endl;
    (a1 << 9).show_binary();
    std::cout << std::endl;
    (a1 << 13).show_binary();
    std::cout << std::endl;
    (a1 << 16).show_binary();
    std::cout << std::endl;

    std::cout << std::endl << "二进制位数输出测试:" << std::endl;
    for (int i = 120; i < 140; i++) {
        printf("Bits = %d \t",i);
        if (rsa_uint::binary_ones(i).get_binary_bits() == i)
            printf("succeeded\n");
        else
            printf("failed\n");
    }
    std::cout << std::endl;


    //乘法
    std::cout << std::endl << "乘法测试:" << std::endl;
    auto b1 = rsa_uint(12345679);
    auto b2 = rsa_uint(7);
    (b1 * b2).show_hex();
    std::cout << std::endl << "*= 测试:" << std::endl;
    b1.show_hex();
    b1 *= 2;
    std::cout << std::endl;
    b1.show_hex();
    std::cout << std::endl;


    //取模测试
    std::cout << std::endl << "取模测试：" << std::endl;
    a1 = 776;
    a2 = 91;
    sum = a1 % a2;
    sum.show_hex();

    //随机数生成测试
    std::cout << std::endl << "奇数生成测试:" << std::endl;
    bool test_switch = 1;
    for (int i = 0; (i < 30) & test_switch; i++) {
        std::cout << "第" << i + 1 << "个:\t\t";
        auto temp = rsa_uint::gen_random_odd(i);
        temp.show_hex();
        if (temp.byte_part[sizeof(temp) - 1] & 0x1) {
            std::cout <<  "\t是" << std::endl;
        }
        else {
            std::cout << "\t否" << std::endl;
        }
        temp = 0;
    }
        std::cout << std::endl << "模加测试:" << std::endl;
    int attempts = 30;
    uint64_t mad_a = 0, mad_b = 0, modulus = 0;
    time_t t11 = 0, t12 = 0, t21 = 0, t22 = 0;
    for (int i = 0; i < attempts; i++) {
        mad_a = gen_rand()&0xfffffff;
        mad_b = gen_rand()&0xfffffff;
        modulus = gen_rand()&0xfffffff;
        uint64_t result = (mad_a + mad_b) % modulus;
        printf("( 0x%llx \t+\t0x%llx\t ) %% 0x%llx\t=\t0x%llx\t", mad_a, mad_b, modulus, result);
        if (rsa_uint(mad_a).modulus_add(mad_b, modulus) == result) {
            printf("succeeded\n");
        }
        else
            printf("failed\n");
    }
    std::cout << std::endl;



    std::cout << std::endl << "模乘测试:" << std::endl;
    attempts = 20;
    for (int i = 0; i < attempts; i++) {
        mad_a = gen_rand();
        mad_b = gen_rand();
        modulus = gen_rand();
        uint64_t result = (mad_a * mad_b) % modulus;
        printf("( 0x%llx  *  0x%llx  ) %% 0x%llx \t=0x%llx", mad_a, mad_b, modulus, result);
        t11 = timeGetTime();
        rsa_uint s = rsa_uint(mad_a).modulus_mul1(mad_b, modulus);
        t12 = timeGetTime();
        std::cout << '\t';
        s.show_hex();
        t21 = timeGetTime();
        s = rsa_uint(mad_a).modulus_mul2(mad_b, modulus);
        t22 = timeGetTime();
        std::cout << '\t';
        s.show_hex();

        std::cout << "\tmethod1:" << t12 - t11 << " ms\tmethod2:" << t22 - t21 << " ms\t";
        if ( s == result) {
            printf("\tsucceeded:\t");
        }
        else
            printf("\tfailed:\t");
        std::cout << std::endl;
    }
    rsa_uint(0x383a379b).modulus_mul(0xd64f4e24, 0xbf72cda5).show_hex();
    std::cout << std::endl;

    //模乘方
    std::cout << std::endl << "模乘方测试:" << std::endl;
    rsa_uint s;
    attempts = 50;
    time_t sum1 = 0, sum2 = 0;
    for (int i = 0; i < attempts; i++) {
        mad_a = gen_rand();
        mad_b = gen_rand();
        modulus = gen_rand();
        t11 = timeGetTime();
        s = rsa_uint(mad_a).modulus_pow(mad_b, modulus);
        t12 = timeGetTime();
        printf("( 0x%llx  **  0x%llx  ) %% 0x%llx \t=", mad_a, mad_b, modulus);
        s.show_hex();
        std::cout << '\t';
        t21 = timeGetTime();
        s = rsa_uint(mad_a).modulus_pow2(mad_b, modulus);
        t22 = timeGetTime();
        s.show_hex();
        std::cout <<"\tmethod1:" << t12 - t11 << " ms\tmethod2:" << t22 - t21 << " ms\t";
        sum1 += (t12 - t11);
        sum2 += (t22 - t21);
        std::cout << std::endl;
    }
    std::cout << "average \tmethod1:" << sum1/attempts << " ms\tmethod2:" << sum2/attempts << " ms\t";

    std::cout << std::endl << "举例计算:" << std::endl;
    rsa_uint(0x31b00c).modulus_pow2(0x101, 0xcfa14830).show_hex();
    std::cout << '\t';
    rsa_uint(0x31b00c).modulus_pow(0x101, 0xcfa14830).show_hex();
    std::cout << std::endl;
    rsa_uint(0xfb77c541).modulus_pow2(0xcf15, 0xb8fd8439).show_hex();
    std::cout << '\t';
    rsa_uint(0xfb77c541).modulus_pow(0xcf15, 0xb8fd8439).show_hex();
    std::cout << std::endl;
    rsa_uint(0xff80f442).modulus_pow2(0x4508, 0x3c687314).show_hex();
    std::cout << '\t';;
    rsa_uint(0xff80f442).modulus_pow(0x4508, 0x3c687314).show_hex();
    std::cout << std::endl;
    rsa_uint(0x34cccf04).modulus_pow2(0x42c4a, 0xa8e109b5).show_hex();
    std::cout << '\t';
    rsa_uint(0x34cccf04).modulus_pow(0x42c4a, 0xa8e109b5).show_hex();
    std::cout << std::endl;

    std::cout << std::endl << "整数运算部分，测试完成" << std::endl;
}
void Test_Crypt() {
    //素数测试
    std::cout << std::endl << "素数测试" << std::endl;
   // uint64_t prime_list[] = { 0x4d95,0x4d9b,0x4db1,0x4db3,0x4dc9,0x4dcf,0x4dd7,0x4de1,0x4ded,0x4df9,0x4dfb,0x4e05,0x4e0b,0x4e17,0x4e19,0x4e1d,0x4e2b,0x4e35,0x4e37,0x4e3d,0x4e4f,0x4e53,0x4e5f,0x4e67,0x4e79,0x4e85,0x4e8b,0x4e91,0x4e95,0x4e9b,0x4ea1,0x4eaf,0x4eb3,0x4eb5,0x4ec1,0x4ecd,0x4ed1,0x4ed7,0x4ee9,0x4efb,0x4f07,0x4f09,0x4f19,0x4f25,0x4f2d,0x4f3f,0x4f49,0x4f63,0x4f67,0x4f6d,0x4f75,0x4f7b,0x4f81,0x4f85,0x4f87,0x4f91,0x4fa5,0x4fa9,0x4faf,0x4fb7,0x4fbb,0x4fcf,0x4fd9,0x4fdb,0x4ffd,0x4fff,0x5003,0x501b,0x501d,0x5029,0x5035,0x503f,0x5045,0x5047,0x5053,0x5071,0x5077,0x5083,0x5093,0x509f,0x50a1,0x50b7,0x50c9,0x50d5,0x50e3,0x50ed,0x50ef,0x50fb,0x5107,0x510b,0x510d,0x5111,0x5117,0x5123,0x5125,0x5135,0x5147,0x5149,0x5171,0x5179,0x5189,0x518f,0x5197,0x51a1,0x51a3,0x51a7,0x51b9,0x51c1,0x51cb,0x51d3,0x51df,0x51e3,0x51f5,0x51f7,0x5209,0x5213,0x5215,0x5219,0x521b,0x521f,0x5227,0x5243,0x5245,0x524b,0x5261,0x526d,0x5273,0x5281,0x5293,0x5297,0x529d,0x52a5,0x52ab,0x52b1,0x52bb,0x52c3,0x52c7,0x52c9,0x52db,0x52e5,0x52eb,0x52ff,0x5315,0x531d,0x5323,0x5341,0x5345,0x5347,0x534b,0x535d,0x5363,0x5381,0x5383,0x5387,0x538f,0x5395,0x5399,0x539f,0x53ab,0x53b9,0x53db,0x53e9,0x53ef,0x53f3,0x53f5,0x53fb,0x53ff,0x540d,0x5411,0x5413,0x5419,0x5435,0x5437,0x543b,0x5441,0x5449,0x5453,0x5455,0x545f,0x5461,0x546b,0x546d,0x5471,0x548f,0x5491,0x549d,0x54a9,0x54b3,0x54c5,0x54d1,0x54df,0x54e9,0x54eb,0x54f7,0x54fd,0x5507,0x550d,0x551b,0x5527,0x552b,0x5539,0x553d,0x554f,0x5551,0x555b,0x5563,0x5567,0x556f,0x5579,0x5585,0x5597,0x55a9,0x55b1,0x55b7,0x55c9,0x55d9,0x55e7,0x55ed,0x55f3,0x55fd,0x560b,0x560f,0x5615,0x5617,0x5623,0x562f,0x5633,0x5639,0x563f,0x564b,0x564d,0x565d,0x565f,0x566b,0x5671,0x5675,0x5683,0x5689,0x568d,0x568f,0x569b,0x56ad,0x56b1,0x56d5,0x56e7,0x56f3,0x56ff,0x5701,0x5705,0x5707,0x570b,0x5713,0x571f,0x5723,0x5747,0x574d,0x575f,0x5761,0x576d,0x5777,0x577d,0x5789,0x57a1,0x57a9,0x57af,0x57b5,0x57c5,0x57d1,0x57d3,0x57e5,0x57ef,0x5803,0x580d,0x580f,0x5815,0x5827,0x582b,0x582d,0x5855,0x585b,0x585d,0x586d,0x586f,0x5873,0x587b,0x588d,0x5897,0x58a3,0x58a9,0x58ab,0x58b5,0x58bd,0x58c1,0x58c7,0x58d3,0x58d5,0x58df,0x58f1,0x58f9,0x58ff,0x5903,0x5917,0x591b,0x5921,0x5945,0x594b,0x594d,0x5957,0x595d,0x5975,0x597b,0x5989,0x5999,0x599f,0x59b1,0x59b3,0x59bd,0x59d1,0x59db,0x59e3,0x59e9,0x59ed,0x59f3,0x59f5,0x59ff,0x5a01,0x5a0d,0x5a11,0x5a13,0x5a17,0x5a1f,0x5a29,0x5a2f,0x5a3b,0x5a4d,0x5a5b,0x5a67,0x5a77,0x5a7f,0x5a85,0x5a95,0x5a9d,0x5aa1,0x5aa3,0x5aa9,0x5abb,0x5ad3,0x5ae5,0x5aef,0x5afb,0x5afd,0x5b01,0x5b0f,0x5b19,0x5b1f,0x5b25,0x5b2b,0x5b3d,0x5b49,0x5b4b,0x5b67,0x5b79,0x5b87,0x5b97,0x5ba3,0x5bb1,0x5bc9,0x5bd5,0x5beb,0x5bf1,0x5bf3,0x5bfd,0x5c05,0x5c09,0x5c0b,0x5c0f,0x5c1d,0x5c29,0x5c2f,0x5c33,0x5c39,0x5c47,0x5c4b,0x5c4d,0x5c51,0x5c6f,0x5c75,0x5c77,0x5c7d,0x5c87,0x5c89,0x5ca7,0x5cbd,0x5cbf,0x5cc3,0x5cc9,0x5cd1,0x5cd7,0x5cdd,0x5ced,0x5cf9,0x5d05,0x5d0b,0x5d13,0x5d17,0x5d19,0x5d31,0x5d3d,0x5d41,0x5d47,0x5d4f,0x5d55,0x5d5b,0x5d65,0x5d67,0x5d6d,0x5d79,0x5d95,0x5da3,0x5da9,0x5dad,0x5db9,0x5dc1,0x5dc7,0x5dd3,0x5dd7,0x5ddd,0x5deb,0x5df1,0x5dfd,0x5e07,0x5e0d,0x5e13,0x5e1b,0x5e21,0x5e27,0x5e2b,0x5e2d,0x5e31,0x5e39,0x5e45,0x5e49,0x5e57,0x5e69,0x5e73,0x5e75,0x5e85,0x5e8b,0x5e9f,0x5ea5,0x5eaf,0x5eb7,0x5ebb,0x5ed9,0x5efd,0x5f09,0x5f11,0x5f27,0x5f33,0x5f35,0x5f3b };
    
    uint64_t prime_list[] = { 101,103,105,107,121,131 };
    for (int i = 0; i < sizeof(prime_list) / sizeof(uint64_t); i++) {
        if (rsa_uint(prime_list[i])
            .is_prime()) {
            //printf("True\t");
        }
        else printf("False\t");
    }
    std::cout << std::endl;
    //crypto test
    //gcd测试
    std::cout << std::endl << "gcd测试:" << std::endl;
    rsa_uint s;
    int attempts = 20;
    uint64_t a=0, b=0,modulus=0;
    for (int i = 0; i < attempts; i++) {
        a = gen_rand();
        b = gen_rand();
        modulus = gen_rand();
        printf("hex(math.gcd( 0x%llx  ,  0x%llx  )) \t=", a, b);
        s = rsa_uint::gcd(a, b);
        s.show_hex();
        std::cout << std::endl;
    }
    std::cout << std::endl << "整数除法测试" << std::endl;
    attempts = 10;
    for (int i = 0; i < attempts; i++) {
        a = gen_rand();
        b = gen_rand();
        s = gen_rand();
        printf("print (int(0x%llx/0x%llx)==", a,b);
        rsa_uint(a).int_dev(b).show_hex();
        printf("L)");
        std::cout << std::endl;
    }


    //求逆测试
    std::cout << std::endl << "求逆测试" << std::endl;
    attempts = 0;
    for (int i = 0; i < attempts; i++) {
        a = gen_rand();
        b = gen_rand();
        modulus = gen_rand();
        printf("hex(inv(0x%llx,0x%llx)) \t=", a, modulus);
        rsa_uint(a).modulus_inv(modulus).show_hex();
        std::cout << std::endl;
        rsa_uint(13).modulus_inv(70*90).show_hex();
    }
    //加密测试
    printf("求解13x = 1 mod (71-1)*(91-1)\nx =");
    rsa_uint(13).modulus_inv(70 * 90).show_hex();
    std::cout << std::endl;
    std::cout << std::endl << "用以上信息，生成一个公钥，对指定明文进行加密测试" << std::endl;
    rsa_uint plaint_text = 29;

    rsa_uint r_modulus(71 * 91), exponent(13);
    rsa_key eky(13, 71 * 91), dky;

    rsa_uint plain_text = 29;
    rsa_uint cipher_text = crypt(plaint_text, eky);

    if (cipher_text == 4852)
        std::cout << std::endl << "加密成功!" << std::endl;
    std::cout << std::endl << "对加密信息进行解密" << std::endl;
    rsa_key::get_dky(70 * 90, eky, dky);
    plain_text = 0;
    plain_text = crypt(cipher_text, dky);
    if (plain_text == 29)
        std::cout << std::endl << "解密成功!" << std::endl;


    std::cout << std::endl;
    printf("求解17x = 1 mod (151-1)*(131-1)\nx =");
    rsa_uint(17).modulus_inv(150 * 130).show_hex();
    std::cout << std::endl;
    std::cout << std::endl << "用以上信息，生成一个公钥，对指定明文进行加密测试" << std::endl;
    rsa_uint plaint_text2 = 56;

    rsa_uint r_modulus2(151 * 131), exponent2(17);
    rsa_key eky2(17, 151 * 131), dky2;

    rsa_uint plain_text2 = 56;
    rsa_uint cipher_text2 = crypt(plaint_text2, eky2);

    if (cipher_text2 == 4934)
        std::cout << std::endl << "加密成功!" << std::endl;
    std::cout << std::endl << "对加密信息进行解密" << std::endl;
    rsa_key::get_dky(120 * 130, eky2, dky2);
    plain_text2 = 0;
    plain_text2 = crypt(cipher_text2, dky2);
    if (plain_text2 == 56)
        std::cout << std::endl << "解密成功!" << std::endl;
    std::cout << std::endl << "加密运算部分，测试完成" << std::endl;
} 
void RSAKeyGenTest() {
    rsa_uint plain_text = rsa_uint::gen_random_integer(RSA_UINT_SIZE);
    rsa_key eky(0x10001,0) , dky;
    printf("\n*************************************\nGen KeyPair...\n");
    rsa_key::gen_keypair(0, 0, eky, dky, 32);
    printf("\n------Eky:\nmodulus: ");
    eky.modulus.show_hex();
    printf("\nexponent: ");
    eky.exponent.show_hex();
    printf("\n--------\n");

    printf("\n------Dky:\nmodulus: ");
    dky.modulus.show_hex();
    printf("\nexponent: ");
    dky.exponent.show_hex();
    printf("\n--------\n");
    printf("\nPlain:\n");
    plain_text.show_hex();
    printf("\nCipher:\n");
    rsa_uint cipher_text = crypt(plain_text, eky);
    cipher_text.show_hex();
    printf("\nDecrypt:\n");
    rsa_uint decrypt_text = crypt(cipher_text, dky);
    decrypt_text.show_hex();
    if (plain_text == decrypt_text)
        printf("\nEncrypt Test Succeed.\n");

}
void KeyGenTest2() {
    rsa_uint p, q, modulus, phai;
    p.byte_part[sizeof(p.byte_part) - 3] = rsa_uint::u64edian_trans(0xed9386e492010000ui64);
    p.byte_part[sizeof(p.byte_part) - 2] = rsa_uint::u64edian_trans(0x049a81d985047e84ui64);
    p.byte_part[sizeof(p.byte_part) - 1] = rsa_uint::u64edian_trans(0x4ddafe9e61c49782);

    q.byte_part[sizeof(p.byte_part) - 2] = rsa_uint::u64edian_trans(0x3370200000000000);
    q.byte_part[sizeof(p.byte_part) - 1] = rsa_uint::u64edian_trans(0x3354b10aab0bc5ce);

    modulus.byte_part[sizeof(p.byte_part) - 1] = rsa_uint::u64edian_trans(0x833f2ac93da3c157);
    modulus.byte_part[sizeof(p.byte_part) - 2] = rsa_uint::u64edian_trans(0x11baaaf6695b5c83);
    modulus.byte_part[sizeof(p.byte_part) - 3] = rsa_uint::u64edian_trans(0x5076cd0fa0e85a86);
    modulus.byte_part[sizeof(p.byte_part) - 4] = rsa_uint::u64edian_trans(0x330d26564f3875a3);

    auto exponent = rsa_uint(0x10001);

    phai.byte_part[sizeof(p.byte_part) - 4] = rsa_uint::u64edian_trans(0x330d26564f3875a3);
    phai.byte_part[sizeof(p.byte_part) - 3] = rsa_uint::u64edian_trans(0x5076cb7cbc61c698);
    phai.byte_part[sizeof(p.byte_part) - 2] = rsa_uint::u64edian_trans(0x8d3ca6708fb9524b);
    phai.byte_part[sizeof(p.byte_part) - 1] = rsa_uint::u64edian_trans(0x31e25abc93f392d8);

    rsa_key eky(exponent, modulus), dky;
    rsa_key::get_dky(phai, eky, dky);
}
void B64_TEST() {
    char str[60] = "C:\\Users\\lenovo\\source\\repos\\zcrypt\\Debug\\test.log";
    uint32_t ss = 60;
    char ddd[200] = {};
    uint32_t ds = 200;
    base64::b64encode((uint8_t*)str, strlen(str), ddd, &ds);
    memset(str, 0, 60);
    base64::b64decode(ddd, strlen(ddd), (unsigned char*)str, &ss);
}
void AES_TEST() {
    byte plaintext[16] = { 
                0x32, 0x88, 0x31, 0xe0,
                0x43, 0x5a, 0x31, 0x37,
                0xf6, 0x30, 0x98, 0x07,
                0xa8, 0x8d, 0xa2, 0x34 };
    //*/
    //char plaintext[16];
    //strcpy(plaintext, "7h1s 1s a test.");
    uint8_t crymsg[200] = {};
    char keybuf[16];
    //keybuf[15] = '.';
    ((uint32_t*)keybuf)[0] = 0x16157e2b;
    ((uint32_t*)keybuf)[1] = 0xa6d2ae28;
    ((uint32_t*)keybuf)[2] = 0x8815f7ab;
    ((uint32_t*)keybuf)[3] = 0x3c4fcf09;
    //strcpy(keybuf, "this is passwd.");
    AES_KEY key((uint8_t*)keybuf, 16);
    const char* plaintext2 = "I have a pen, I have an apple...errrr...apple pen!!!";
    int l = strlen(plaintext2);
    uint8_t dect[200] = {};
    crypt_aes::show_zeropadding_warning(false);
    crypt_aes::set_padding_mode(AES_PADDING_PKCS);
    crypt_aes::set_work_mode(AES_MODE_CBC);
    char iv[] = { 0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf };
    crypt_aes::set_iv((uint8_t*)iv);
    int cry_size = crypt_aes128::crypt((uint8_t*)plaintext2,strlen(plaintext2), key, crymsg,200);
    int l2 = crypt_aes128::decrypt(crymsg, cry_size,key, dect,200);
    if (!memcmp(plaintext2, dect, strlen(plaintext2)))
        std::cout << "\nYes!";

}

int main() {
    //char szDirName[512];
    //GetModuleFileNameA(0, szDirName,512);
    //printf("%s\n", szDirName);
    //FILE* fp = freopen("C:\\Users\\lenovo\\source\\repos\\zcrypt\\Debug\\test.log", "w", stdout);
    //Test_RSA();
    //Test_Crypt();
    //RSAKeyGenTest();
    //KeyGenTest2()
    // B64_TEST()
    //char plaintext[16];
    //memset(plaintext, 0, sizeof(plaintext));
    
    AES_TEST();
    

    return 0;
}//实现RSA,AES,TEA,RC4