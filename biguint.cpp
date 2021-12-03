#include "biguint.h"


/*已经弃用的随机数生成方案
time_t global_srand_last_time = 0;
int global_last_rand = 0;
int get_rand_int_previous()
{
	time_t current_time = time(0);
	while (current_time == global_srand_last_time)
		current_time = time(0);
	srand((unsigned)current_time);
	global_srand_last_time = current_time;
	int current_rand = rand();
	while (current_rand == global_last_rand)
		current_rand = rand();
	global_last_rand = current_rand;
	return current_rand;
}


*/

size_t gen_rand()
{
	return rd();
}


BIGUINT::BIGUINT()
{
	memset(byte_part, 0, sizeof(byte_part));
}

BIGUINT::BIGUINT(uint64_t b)
{
	memset(byte_part, 0, sizeof(byte_part));
	uint32_t index = sizeof(byte_part) - 1;
	while (b && index) {
		byte_part[index--] = b & 0xff;
		b >>= 8;
	}
}

BIGUINT::BIGUINT(const BIGUINT& b)
{
	memset(byte_part, 0, sizeof(byte_part));
	if (this != &b)
		memcpy_s(byte_part, sizeof(byte_part), b.byte_part, sizeof(b.byte_part));
}

BIGUINT::BIGUINT(const uint8_t* buffer, uint32_t buf_len)
{
	memset(byte_part, 0, sizeof(BIGUINT));
	int length = buf_len < sizeof(BIGUINT) ? buf_len : sizeof(BIGUINT);
	memcpy_s(&byte_part[sizeof(BIGUINT) - length], sizeof(byte_part), buffer, length);
}


BIGUINT& BIGUINT::operator=(uint64_t b)
{
	memset(byte_part, 0, sizeof(BIGUINT));
	uint32_t index = sizeof(byte_part) - 1;
	while (b && index) {
		byte_part[index--] = b & 0xff;
		b >>= 8;
	}
	return *this;
}

BIGUINT& BIGUINT::operator=(const BIGUINT& b)
{
	memset(byte_part, 0, sizeof(BIGUINT));
	if (this != &b)
		memcpy_s(byte_part, sizeof(byte_part), b.byte_part, sizeof(b.byte_part));
	return *this;
}


bool BIGUINT::operator>(const BIGUINT& b) const
{
	return memcmp(this->byte_part, b.byte_part, sizeof(BIGUINT)) > 0;
}

bool BIGUINT::operator>=(const BIGUINT& b) const
{
	return memcmp(this->byte_part, b.byte_part, sizeof(BIGUINT)) >= 0;
}

bool BIGUINT::operator<(const BIGUINT& b) const
{
	return memcmp(this->byte_part, b.byte_part, sizeof(BIGUINT)) < 0;
}

bool BIGUINT::operator<=(const BIGUINT& b) const
{
	return memcmp(this->byte_part, b.byte_part, sizeof(BIGUINT)) <= 0;
}

bool BIGUINT::operator==(const BIGUINT& b) const
{
	return memcmp(this->byte_part, b.byte_part, sizeof(BIGUINT)) == 0;
}

bool BIGUINT::operator!=(const BIGUINT& b) const
{
	return memcmp(this->byte_part, b.byte_part, sizeof(BIGUINT)) != 0;
}


bool BIGUINT::is_little_edian()
{
	uint16_t test = 0x0001;
	return *((uint8_t*)&test);
}

uint16_t BIGUINT::u16edian_trans(uint16_t t)
{
	if (is_little_edian())
		return t >> 8 | t << 8;
	else
		return t;
}

uint32_t BIGUINT::u32edian_trans(uint32_t t)
{
	if (is_little_edian())
		return
		(t >> 24) | (t << 24) |
		((t & 0x00ff0000) >> 8) |
		((t & 0x0000ff00) << 8);
	else return t;


}

uint64_t BIGUINT::u64edian_trans(uint64_t t)
{
	if (is_little_edian())
		return (t >> 56) | (t << 56) |
		(t & 0x00ff000000000000) >> 40 |
		(t & 0x0000ff0000000000) >> 24 |
		(t & 0x000000ff00000000) >> 8 |
		(t & 0x00000000ff000000) << 8 |
		(t & 0x0000000000ff0000) << 24 |
		(t & 0x000000000000ff00) << 40;
	else return t;

}

BIGUINT BIGUINT::operator<<(uint32_t m) const
{
	BIGUINT result = 0;
	m %= sizeof(this->byte_part);
	uint32_t shift_bytes = m >> 3;
	if (m & 0x7) {
		uint16_t temp = 0;
		for (uint32_t i = sizeof(result.byte_part) - 1; i > 0; i--) {
			temp |= (this->byte_part[i] << (m & 0x7));
			result.byte_part[i] = temp & 0xff;
			temp >>= 8;
		}
		temp |= (this->byte_part[0] << (m & 0x7));
		result.byte_part[0] = temp & 0xff;
		memcpy_s(result.byte_part, sizeof(result.byte_part) - shift_bytes, &result.byte_part[shift_bytes], sizeof(result.byte_part) - shift_bytes);
		memset(&result.byte_part[sizeof(result.byte_part) - shift_bytes], 0, shift_bytes);
	}
	else {
		memcpy_s(result.byte_part, sizeof(result.byte_part) - shift_bytes, &this->byte_part[shift_bytes], sizeof(this->byte_part) - shift_bytes);
	}
	return result;
}

BIGUINT BIGUINT::operator>>(uint32_t m) const
{
	BIGUINT result;
	m %= sizeof(this->byte_part);
	uint32_t shift_bytes = m >> 3;
	if (m & 0x7) {
		uint16_t temp = 0;
		for (uint32_t i = 0; i < sizeof(result.byte_part); i++) {
			temp |= ((this->byte_part[i] << 8) >> (m & 0x7));
			result.byte_part[i] = temp >> 8;
			temp <<= 8;
		}
		memcpy_s(&result.byte_part[shift_bytes], sizeof(result.byte_part) - shift_bytes, result.byte_part, sizeof(result.byte_part) - shift_bytes);
		memset(result.byte_part, 0, shift_bytes);
	}
	else {
		memcpy_s(&result.byte_part[shift_bytes], sizeof(result.byte_part) - shift_bytes, this->byte_part, sizeof(this->byte_part) - shift_bytes);
	}
	return result;
}

bool BIGUINT::operator<<=(uint32_t m)
{
	*this = *this << m;
	return true;
}

bool BIGUINT::operator>>=(uint32_t m)
{
	*this = *this >> m;
	return true;
}

BIGUINT BIGUINT::operator|(const BIGUINT& b) const
{
	BIGUINT result;
	for (uint32_t i = 0; i < sizeof(byte_part); i++) {
		result.byte_part[i] = this->byte_part[i] | b.byte_part[i];
	}

	return result;
}

BIGUINT BIGUINT::operator&(const BIGUINT& b) const
{
	BIGUINT result;
	for (uint32_t i = 0; i < sizeof(byte_part); i++) {
		result.byte_part[i] = this->byte_part[i] & b.byte_part[i];
	}

	return result;
}

BIGUINT BIGUINT::operator^(const BIGUINT& b) const
{
	BIGUINT result;
	for (uint32_t i = 0; i < sizeof(byte_part); i++) {
		result.byte_part[i] = this->byte_part[i] ^ b.byte_part[i];
	}

	return result;
}

BIGUINT BIGUINT::operator~() const
{
	BIGUINT result;
	for (uint32_t i = 0; i < sizeof(byte_part); i++) {
		result.byte_part[i] = ~this->byte_part[i];
	}

	return result;
}

bool BIGUINT::operator!() const
{
	for (uint32_t i = 0; i < sizeof(byte_part); i++) {
		if (this->byte_part[i])
			return false;
	}

	return true;
}


/*
RSAUINT::operator uint64_t() { return u64le2be(qbyte_part[(RSA_INT_SIZE >> 3) - 1]); }
*/

BIGUINT BIGUINT::operator+(const BIGUINT& b) const
{
	BIGUINT temp;
	//uint32_t d1 = digit_count(*this) ;
	//uint32_t d2 = digit_count(b);
	//uint32_t start = scale - (d1 > d2 ? d1 : d2);
	bool overflow_flag = 0;
	for (int64_t i = sizeof(BIGUINT) - 1; i > -1; i--) {
		temp.byte_part[i] += overflow_flag;
		overflow_flag = (temp.byte_part[i] < overflow_flag);
		temp.byte_part[i] += byte_part[i];
		overflow_flag |= (temp.byte_part[i] < byte_part[i]);
		temp.byte_part[i] += b.byte_part[i];
		overflow_flag |= (temp.byte_part[i] < b.byte_part[i]);
	}
	return temp;

}


BIGUINT BIGUINT::operator-(const BIGUINT& b) const
{
	/*
	RSAUINT temp = 0;
	for (uint32_t i = 0; i < sizeof(RSAUINT); i++) {
		if (this->byte_part[i] == b.byte_part[i])
			continue;
		else if (this->byte_part[i] > b.byte_part[i]) {
			bool lend_flag = 0;
			for (uint32_t j = sizeof(RSAUINT) - 1; j > i; j--) {
				if (this->byte_part[j] >= b.byte_part[j]) {
					temp.byte_part[j] = this->byte_part[j] - b.byte_part[j];
					if (!temp.byte_part[j]) {
						temp.byte_part[j] = 0xff;
						lend_flag = 1;
					}
					else {
						temp.byte_part[j] -= lend_flag;
						lend_flag = 0;
					}


				}
				else {
					temp.byte_part[j] = this->byte_part[j] + (0x100 - b.byte_part[j]) - lend_flag;
					lend_flag = 1;
				}
			}
			temp.byte_part[i] = this->byte_part[i] - b.byte_part[i] - lend_flag;

			return temp;

		}
		else
			return *this + (~b + 1);
	}

	return temp;
	*/
	return *this + (~b + 1);
}


bool BIGUINT::operator+=(const BIGUINT& b)
{
	*this = *this + b;
	return true;
}

bool BIGUINT::operator-=(const BIGUINT& b)
{
	*this = *this - b;
	return true;
}


BIGUINT BIGUINT::operator++()
{
	*this = *this + 1;
	return *this;

}

BIGUINT BIGUINT::operator--()
{
	*this = *this - 1;
	return *this;
}

BIGUINT BIGUINT::operator++(int)
{
	BIGUINT temp(*this);
	*this = *this + 1;
	return temp;
}

BIGUINT BIGUINT::operator--(int)
{
	BIGUINT temp(*this);
	*this = *this - 1;
	return temp;
}

BIGUINT BIGUINT::operator*(const BIGUINT& b) const
{
	//分组
	uint16_t carry = 0, of = 0, result_of = 0;
	bool overflow_flag = 0;
	BIGUINT result;

	//以两字节进行分组，大小端转换，相乘
	//舍弃掉溢出部分的计算
	uint32_t s = sizeof(result) / sizeof(uint16_t);
	for (int64_t index_a = s - 1; index_a > -1; index_a--) {
		if (lbyte_part[index_a] || carry) {
			uint32_t int_a = u16edian_trans(lbyte_part[index_a]);
			uint32_t int_b = 0;
			for (int64_t index_b = s - 1; index_b >= s - index_a - 1; index_b--) {
				if (lbyte_part[index_b] || carry) {
					int_b = u16edian_trans(b.lbyte_part[index_b]);
					int_b *= int_a;
					int_b += carry;
					if (int_b < carry)
						overflow_flag = 1;
					carry = (int_b >> 16) + of;
					if (of > carry)
						overflow_flag = 1;
					if (overflow_flag) {
						of = 1;
						overflow_flag = 0;
					}


					result.lbyte_part[index_b + index_a + 1 - s] += (uint16_t)((int_b & 0xffff) + result_of);
					if ((((int_b & 0xffff) + result_of) > 0xffff) ||
						(result.lbyte_part[index_b + index_a + 1 - s] < ((int_b & 0xffff) + result_of)))
						result_of = 1;
					else
						result_of = 0;

				}
			}
			carry = 0;
			of = 0;
			result_of = 0;

		}
	}

	//大小端转换
	for (int i = 0; i < sizeof(BIGUINT) >> 1; i++)
		if (result.lbyte_part[i])
			result.lbyte_part[i] = u16edian_trans(result.lbyte_part[i]);

	return result;
}

bool BIGUINT::operator*=(const BIGUINT& b)
{
	*this = *this * b;
	return true;
}


void BIGUINT::show_hex()const
{
	uint32_t scale = sizeof(byte_part);
	uint32_t i = 0;
	while (i < scale && byte_part[i] == 0)
		i++;
	if (i >= scale) printf("0");
	else {
		printf("0x");
		while (i < scale) {
			printf("%02x", byte_part[i++]);
		}
	}
}

void BIGUINT::show_binary()const
{
	uint32_t scale = sizeof(byte_part);
	uint32_t i = 0;
	while (i < scale && byte_part[i] == 0)
		i++;
	if (i >= scale) printf("0");
	else {
		printf("0b");
		while (i < scale) {
			for (int j = 0; j < 8; j++)
				printf("%x", (byte_part[i] >> (7 - j)) & 0x1);
			i++;
		}
	}
}


bool BIGUINT::miller_rabin_test() {
	//rsa_int n = p - 1;
	return true;
}

bool BIGUINT::is_prime()
{/*
 素性判断
 */
	int attempts = 10;
	//Miller-Rabbin测试算法
	while (attempts--) {
		if (!this->miller_rabin_test())
			return false;
	}

	//todo:二次探测


	//todo:费马测试



	return true;
}

BIGUINT BIGUINT::operator%(const BIGUINT& modulus)const
{
	/*基于递归的实现 发生栈溢出
	if (modulus == 0) {
		fprintf(stderr, "\n错误: 对于取模运算，模数不能为0!\n");
		abort();
	}
	if (*this == modulus)
		return 0;
	else if (*this < modulus)
		return *this;
	else {
		BIGUINT temp;
		if (this->byte_part[sizeof(byte_part) - 1] & 0x1) {
			temp = (*this - 1);
			temp = temp % modulus;
			return (temp + 1) % modulus;
		}
		else {
			temp = *this >> 1;
			temp = temp % modulus;
			return (temp + temp) % modulus;
		}
	}*/
	BIGUINT this_copy = *this;
	while (this_copy >= modulus ) {
		this_copy -= modulus;
	}
	return this_copy;
}

bool BIGUINT::operator%=(const BIGUINT& modulus)
{
	*this = (*this % modulus);
	return true;
}

BIGUINT BIGUINT::modulus_pow(const BIGUINT& exponent, const BIGUINT& modulus)const
{
	//Montgomery algorithm
	// 
//乘方取模

	//分而治之
	if (exponent == 0)	//指数为0，无脑返回1就好
		return 1;
	else if (*this == 0)
		return 0;
	else if (*this > modulus)
		return (*this % modulus).modulus_pow(exponent, modulus);//太大了，取小一点
	else {
		if (exponent.byte_part[sizeof(BIGUINT) - 1] & 0x1) {
			return this->modulus_mul(this->modulus_pow(exponent - 1, modulus), modulus);//是奇数，把落单的揪出来
		}
		else {
			BIGUINT temp = this->modulus_pow(exponent >> 1, modulus);//偶数，裂开大法
			return temp.modulus_mul(temp, modulus);
		}
	}
}

BIGUINT BIGUINT::modulus_mul(const BIGUINT& b, const BIGUINT& modulus)const
{
	//Montgomery algorithm
//相乘取模
	//分而治之

	if ((b == 0) || (*this == 0))
		return 0;
	else if (b == 1)
		return (*this % modulus);
	else if (*this == 1)
		return (b % modulus);
	else {
		if (*this > modulus)//缩小乘数规模
			return (*this % modulus).modulus_mul(b, modulus);
		else if (b > modulus)
			return this->modulus_mul(b % modulus, modulus);
		else if (b.byte_part[sizeof(b.byte_part) - 1] & 1) //是奇数，把落单的揪出来
			return this->modulus_add(this->modulus_mul(b - 1, modulus), modulus);
		else {//裂开大法好
			BIGUINT temp = this->modulus_mul(b >> 1, modulus);
			return temp.modulus_add(temp, modulus);
		}
	}


	return BIGUINT();
}

BIGUINT BIGUINT::modulus_add(const BIGUINT& b, const BIGUINT& modulus)const
{
	BIGUINT temp1 = (b % modulus);
	BIGUINT temp2 = temp1 + (*this % modulus);
	if (temp2 >= temp1)//没有溢出
		return temp2 % modulus;
	else {		//发生了溢出
		return temp2.modulus_add(BIGUINT::max_rsa_uint(), modulus);
	}

}

BIGUINT BIGUINT::int_dev(const BIGUINT& b) const
{
	if (b == 0) {
		fprintf(stderr, "整数除法出现错误: 除数不能为0\n");
		abort();
	}
	BIGUINT result;
	BIGUINT temp = *this;
	while (temp > b) {
		temp -= b;
		result++;
	}
	if (temp == b)
		result++;
	return result;
}

BIGUINT BIGUINT::gcd(const BIGUINT& b) const
{
	return b == 0 ? *this : b.gcd(*this % b);
}

/*
bool BIGUINT::ext_gcd(const BIGUINT& a, const BIGUINT& b, BIGUINT& d, BIGUINT& x, BIGUINT& y, bool sgn_y, bool sgn_x )
{//extend Euclid algorithm

	//gcd(a,b)=d
	// gcd(a,b)=(a*x + b*y) ( x,y belongs to Z)
	// 这里为计算方便,取a>0,b>0,x>0,y>0,原来的公式被魔改成: a*x = d + b*y, 即 原来的y变成-y
	//
	if (b == 0) {
		d = a; x = 1; y = 0, sgn_y = sgn_x = 0;
		return true;
	}
	else
	{
		ext_gcd(b, a % b, d, y, x);
		bool overflow_flag = 0;
		BIGUINT temp = (a.int_dev(b)) * x;
		if (sgn_x != sgn_y) {
			y += temp;
			if( (y < temp) )
		}
		else {
			y -= temp;
			overflow_flag = (y > temp);
		}

	}
	return BIGUINT();
}*/

BIGUINT BIGUINT::gcd(const BIGUINT& a, const BIGUINT& b)
{//辗转相除法
	return a.gcd(b);
}



BIGUINT BIGUINT::modulus_inv(const BIGUINT& modulus) const
{
	if (*this==0 || modulus==0 || this->gcd(modulus) != 1)//不存在逆元! 返回0
		return 0;
	BIGUINT result(1);
	while ((result < modulus) &&
		this->modulus_mul(result, modulus) != 1)//遍历
		result++;
	if (result != modulus)
		return result;
	else return 0;
}



BIGUINT BIGUINT::zero()
{
	return 0;
}

BIGUINT BIGUINT::max_rsa_uint()
{
	BIGUINT result;
	memset(result.byte_part, 0xff, sizeof(result.byte_part));
	return result;
}

BIGUINT BIGUINT::binary_ones(uint32_t bits)
{
	bits = bits > (sizeof(BIGUINT)) << 3 ? (sizeof(BIGUINT) << 3) : bits;
	BIGUINT temp = 0;
	memset(&temp.byte_part[sizeof(BIGUINT) - (bits >> 3)], 0xff, (bits >> 3));
	if (bits & 0x7) {
		temp.byte_part[sizeof(BIGUINT) - (bits >> 3) - 1] = 0xff;
		temp >>= (8 - (bits & 0x7));
	}
	return temp;
}


BIGUINT BIGUINT::gen_random_integer(uint32_t bits)
{
	//生成随机奇数
	//由于time精度限制,建议两次调用该函数时间间隔不得低于0.5秒
	// 出错返回0
	//Sleep(0x500u);
	uint8_t odd_buffer[sizeof(BIGUINT)];
	if (!bits)
		return 0;
	bits = bits > sizeof(BIGUINT) << 3 ? sizeof(BIGUINT) << 3 : bits;
	uint32_t odd_size = bits >> 3;
	//odd_size = odd_size > RSA_INT_SIZE-1 ? RSA_INT_SIZE-1 : odd_size;
	memset(odd_buffer, 0, sizeof(BIGUINT));
	//填充随机数据
	for (uint32_t i = sizeof(BIGUINT) - odd_size; i < sizeof(BIGUINT); i++) {
		odd_buffer[i] = gen_rand() & 0xff;
	}

	//首位置1
	if ((bits & 0x7)) {
		uint8_t mo = 0x1;
		for (uint32_t i = 1; i < (bits & 0x7); i++) {
			mo |= mo << 1;
		}
		odd_buffer[sizeof(BIGUINT) - odd_size - 1] |= ((gen_rand() & mo) | (0x1 << ((bits & 0x7) - 1)));
	}
	else
		if (odd_size)
			odd_buffer[sizeof(BIGUINT) - odd_size] |= 0x80;
		else //impossible
			return 0;

	//末位置1
	//odd_buffer[sizeof(odd_buffer) - 1] |= 0x1;
	return BIGUINT(odd_buffer, sizeof(BIGUINT));

}
BIGUINT BIGUINT::gen_random_odd(uint32_t bits) {
	BIGUINT result = gen_random_integer(bits);
	result.byte_part[sizeof(BIGUINT) - 1] |= 0x1;
	return result;
}

BIGUINT BIGUINT::gen_random_prime(uint32_t bits, uint32_t attempts)
{
	//生成大素数
	BIGUINT result = 0;
	while (attempts--) {
		result = gen_random_odd(bits);
		if (result.is_prime())
			return result;
		else
			continue;
	}
	return 0;
}


