//实现RSA,AES,TEA,RC4
#define _CRT_SECURE_NO_DEPRECATE

//这里是主函数入口，负责实际的输出
#include"zcrypt.h"

/*
int main()
{
    rsa_int p = 53, q = 61;
    rsa_int n = p*q;
    rsa_int eky = 367;
    rsa_int dky = 0;
    rsa_get_dky(&p, &q, &eky, &dky);
    rsa_int plain_text = 2007;
    rsa_int cipher_text = 0,decrypt_text=0;
    puts()
    rsa(&eky, &n, &plain_text, &cipher_text);
    rsa(&dky, &n, &cipher_text, &decrypt_text);

    if (plain_text == decrypt_text)
        printf("True!\n");
    else
        printf("Error!\n");
    system("pause");
    return 0;
}
*/
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
    (rsa_uint::max_rsa_uint() + 1).show_hex();
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
        auto t1 = time(0);
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
    std::cout << std::endl << "质数检测测试:" << std::endl;

    std::cout << std::endl << "模加测试:" << std::endl;
    int attempts = 30;
    uint64_t mad_a = 0, mad_b = 0, modulus = 0;
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
    //模乘
    std::cout << std::endl << "模乘测试:" << std::endl;
    attempts = 20;
    for (int i = 0; i < attempts; i++) {
        mad_a = gen_rand();
        mad_b = gen_rand();
        modulus = gen_rand();
        uint64_t result = (mad_a * mad_b) % modulus;
        printf("( 0x%llx  *  0x%llx  ) %% 0x%llx \t=0x%llx", mad_a, mad_b, modulus, result);
        rsa_uint s = rsa_uint(mad_a).modulus_mul(mad_b, modulus);
        if ( s == result) {
            printf("\tsucceeded:\t");
        }
        else
            printf("\tfailed:\t");
        s.show_hex();
        std::cout << std::endl;
    }
    rsa_uint(0x383a379b).modulus_mul(0xd64f4e24, 0xbf72cda5).show_hex();
    std::cout << std::endl;

    //模乘方
    std::cout << std::endl << "模乘方测试:" << std::endl;
    rsa_uint s;
    attempts = 20;
    for (int i = 0; i < attempts; i++) {
        mad_a = gen_rand();
        mad_b = gen_rand();
        modulus = gen_rand();
        s = rsa_uint(mad_a).modulus_pow(mad_b, modulus);
        printf("( 0x%llx  **  0x%llx  ) %% 0x%llx \t=", mad_a, mad_b, modulus);
        s.show_hex();
        std::cout << std::endl;
    }
    std::cout << std::endl << "举例计算:" << std::endl;
    rsa_uint(0x31b00c).modulus_pow(0x2, 0xcfa14830).show_hex();
    std::cout << std::endl;
    rsa_uint(0xfb77c541).modulus_pow(0xcf15, 0xb8fd8439).show_hex();
    std::cout << std::endl;
    rsa_uint(0xff80f442).modulus_pow(0x4508, 0x3c687314).show_hex();
    std::cout << std::endl;
    rsa_uint(0x34cccf04).modulus_pow(0x42c4a, 0xa8e109b5).show_hex();
    std::cout << std::endl;

    std::cout << std::endl << "整数运算部分，测试完成" << std::endl;
}

void Test_Crypt() {
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
    std::cout << std::endl << "加密运算部分，测试完成" << std::endl;
} 

int main() {
    Test_RSA();
    //hex(inv(0x6b77d59, 0xf9c1ca5b))

    //crypt testus).show
    Test_Crypt();
   return 0;
}

