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
	uint16_t* test = new uint16_t;
	*test = 0x0001;
	return *(uint8_t*)test;
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
	if (m == 0)
		return *this;
	BIGUINT result = 0;
	if (m >= sizeof(this->byte_part))
		return 0;
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
	if (m == 0)
		return *this;
	BIGUINT result;
	if (m >= sizeof(this->byte_part))
		return 0;
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
	int overflow_flag = 0;
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


bool BIGUINT::miller_rabin_test(uint32_t attempts)const {

	if (*this == 2)
		return true;
	else if ((byte_part[sizeof(BIGUINT) - 1] & 0x1) && (*this != 1)) {//非1奇数
		// gen k,q
		//n-1 = 2**k * q
		uint32_t k = 0;
		BIGUINT q = *this - 1;
		while ( (q.byte_part[sizeof(BIGUINT) - 1] & 0x1) == 0) {//q偶数时
			if (q.byte_part[sizeof(BIGUINT) - 1]) {
				q >>= 1;
				k++;
			}
			else {
				q >>= 8;
				k += 8;
			}
		}

		//calculate
		BIGUINT a = 0;
		BIGUINT result = 0;
		uint32_t j = 0;
		while (attempts) {
			j = 0;
			attempts--;
			do {
				//gen a
				//get this bits
				a = gen_random_integer(this->get_binary_bits()) % *this;
			} while (a < 2);
			while (j < k) {//存在j,使得  a ** (2**j * q） mod n = n-1
				result = a.modulus_pow(q, *this);
				if ((result == (*this - 1)) || (result == 1)) {
					if (attempts == 0)
						return true;
					else
						break;
				}
				else {
					if (j == k - 1)
						return false;
					j++;

					a = a.modulus_mul(a, *this);//a取平方,进行二次探测
				}

			}

		}
	}
	return false;
}

bool BIGUINT::prime_enum_test() const
{
	if (*this == 2 || *this == 3)
		return true;
	else if ((*this == 1) || (((this->byte_part[sizeof(byte_part) - 1]) & 0x1) == 0))
		return false;
	else {
		for (BIGUINT i = 3; i < *this; i += 2)
			if (*this % i == 0)
				return false;
		return true;
	}

}

bool BIGUINT::is_prime()const
{/*
 素性判断
 */
	if (*this == 2)
		return true;
	else if ((*this >> BIGUINT_ISPRIME_ENUM_BITS_BORDER) == 0) //比较小的数字，自动换为枚举法
		return prime_enum_test();
	else {
		//Miller-Rabin测试算法
		if (!this->miller_rabin_test(BIGUINT_ISPRIME_MILLER_RABIN_ATTEMPS))
			return false;

		//todo: 可以在后面加上其它判别法

	}
	return true;
}

uint32_t BIGUINT::get_binary_bits()const
{
	uint32_t bytes_index = 0;
	while ((byte_part[bytes_index] == 0) &&
		(bytes_index < (sizeof(byte_part))))
		bytes_index++;
	if (bytes_index == sizeof(byte_part))
		return 0;
	else {
		int i = 7;
		while ( i >= 0 && !(byte_part[bytes_index] >> i & 1))
			i--;
		return ((sizeof(byte_part) - bytes_index - 1) << 3) + (i + 1);
		
	}
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
	BIGUINT *modulus_copy = new BIGUINT(modulus);
	while (this_copy >= modulus) {
		while (this_copy >= *modulus_copy) {//将算法时间优化至o(log n)
			this_copy -= *modulus_copy;
			*modulus_copy += *modulus_copy;
		}
		*modulus_copy = modulus;
	}
	delete modulus_copy;
	return this_copy;
}

bool BIGUINT::operator%=(const BIGUINT& modulus)
{
	*this = (*this % modulus);
	return true;
}

BIGUINT BIGUINT::modulus_pow(const BIGUINT& exponent, const BIGUINT& modulus) const
{
	return modulus_pow2(exponent,modulus);
}

BIGUINT BIGUINT::modulus_pow1(const BIGUINT& exponent, const BIGUINT& modulus)const
{
	//Montgomery algorithm
	// 
//乘方取模

	//分而治之
	if (exponent == 0)	//指数为0，无脑返回1就好
		if (*this != 0)
			return 1;
		else {
			fprintf(stderr, "错误，调用乘方取模时，底数与指数不能同时为0");
			abort();
		}
	else if (*this == 0)
		return 0;
	else if (*this > modulus) {
		BIGUINT temp = *this % modulus;
		if (temp != 0)
			return temp.modulus_pow(exponent, modulus);//太大了，取小一点
		else
			return 0;
	}
	else {
		if (exponent.byte_part[sizeof(BIGUINT) - 1] & 0x1) {
			return this->modulus_mul1(this->modulus_pow(exponent - 1, modulus), modulus);//是奇数，把落单的揪出来
		}
		else {
			BIGUINT temp = this->modulus_pow(exponent >> 1, modulus);//偶数，裂开大法
			return temp.modulus_mul1(temp, modulus);
		}
	}
}
BIGUINT BIGUINT::modulus_pow2(const BIGUINT& exponent, const BIGUINT& modulus)const {

	BIGUINT f(*this % modulus);
	if (exponent == 0)	//指数为0，无脑返回1就好
		if (!*this)
			return 1;
		else{
			fprintf(stderr, "错误，调用乘方取模时，底数与指数不能同时为0");
			abort();
		}
	else if (f == 0)
		return 0;
	else {
		uint32_t d = 0;
		BIGUINT* q = new BIGUINT(exponent),*g=new BIGUINT(f);
		int i = 0;
		while ((i < sizeof(q->byte_part) - 1) && (q->byte_part[i] == 0))
			i++;
		int index = 7;
		d = (q->byte_part[i] >> index) & 1;
		while (d == 0) {
			index--;
			d = (q->byte_part[i] >> index) & 1;
		}
		if (index)
			while (index) {
				index--;
				d = (q->byte_part[i] >> index) & 1;
				f = f.modulus_mul2(f, modulus);
				if (d)
					f = f.modulus_mul(*g, modulus);
			}
		i++;

		while (i < sizeof(q->byte_part)) {
			index = 8;
			while (index) {
				index--;
				d = (q->byte_part[i] >> index) & 1;
				f = f.modulus_mul(f, modulus);
				if (d)
					f = f.modulus_mul(*g, modulus);
			}
			i++;
		}
		delete q;
		delete g;
		return f;
	}

}


BIGUINT BIGUINT::modulus_mul(const BIGUINT& b, const BIGUINT& modulus) const
{
	return modulus_mul2(b,modulus);//选取算法2作为默认算法
}

BIGUINT BIGUINT::modulus_mul1(const BIGUINT& b, const BIGUINT& modulus)const
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
			return (*this % modulus).modulus_mul1(b, modulus);
		else if (b > modulus)
			return this->modulus_mul1(b % modulus, modulus);
		else if (b.byte_part[sizeof(b.byte_part) - 1] & 1) //是奇数，把落单的揪出来
			return this->modulus_add(this->modulus_mul1(b - 1, modulus), modulus);
		else {//裂开大法好
			BIGUINT temp = this->modulus_mul1(b >> 1, modulus);
			return temp.modulus_add(temp, modulus);
		}
	}
}

BIGUINT BIGUINT::modulus_mul2(const BIGUINT& b, const BIGUINT& modulus) const
{
	if ((b == 0) || (*this == 0))
		return 0;
	else if (b == 1)
		return (*this % modulus);
	else if (*this == 1)
		return (b % modulus);
	else {

		BIGUINT f = *this % modulus;
		uint32_t d = 0;
		BIGUINT* q = new BIGUINT(b % modulus), * g = new BIGUINT(f);
		int i = 0;
		while ((i < sizeof(q->byte_part) - 1) && (q->byte_part[i] == 0))
			i++;
		int index = 7;
		d = (q->byte_part[i] >> index) & 1;
		while (d == 0) {
			index--;
			d = (q->byte_part[i] >> index) & 1;
		}
		if (index)
			while (index) {
				index--;
				d = (q->byte_part[i] >> index) & 1;
				f = f.modulus_add(f, modulus);
				if (d)
					f = f.modulus_add(*g, modulus);
			}
		i++;

		while (i < sizeof(q->byte_part)) {
			index = 8;
			while (index) {
				index--;
				d = (q->byte_part[i] >> index) & 1;
				f = f.modulus_add(f, modulus);
				if (d)
					f = f.modulus_add(*g, modulus);
			}
			i++;
		}
		delete q;
		delete g;
		return f;
	}
}

BIGUINT BIGUINT::modulus_add(const BIGUINT& b, const BIGUINT& modulus)const
{
	BIGUINT *temp1 = new BIGUINT(b % modulus);
	BIGUINT temp2 = *this % modulus;
	temp2 = *temp1 + temp2;
	if (temp2 >= *temp1) {//没有溢出
return_normally:
		delete temp1;
		return temp2 % modulus;
	}
	else {		//发生了溢出
		/*这么做不会溢出.
		* 证明:(以下全为正整数)
		* 对于 (a+b)%n,
		* 令a,b < n ,能表示最大数字为max, 则当a+b溢出时,有
		* 
		* 2n > a+b > max+1,
		* max%n <= max-n
		* 
		* 证明不会溢出，令n>3 有
		* a+b - (max+1) + max%n < (n-1) + (n-1) - (max+1) + max -n
		*					    =n - 3 <= max
		* 证明完毕
		*/
		delete temp1;
		return BIGUINT::max_biguint() % modulus + temp2 + 1;
	}

}

BIGUINT BIGUINT::int_dev(const BIGUINT& b) const
{
	if (b == 0) {
		fprintf(stderr, "整数除法出现错误: 除数不能为0\n");
		abort();
	}
	BIGUINT result, *count_b = new BIGUINT(1);
	BIGUINT* temp1 = new BIGUINT(*this), * temp_b = new BIGUINT(b);
	while (*temp1 > b) {
		while (*temp1 > *temp_b) {
			*temp1 -= *temp_b;
			result += *count_b;

			*temp_b += *temp_b;
			*count_b += *count_b;
			
		}
		*temp_b = b;
		*count_b = 1;
	}
	if (*temp1 == b)
		result++;
	delete temp1;
	delete temp_b;
	delete count_b;
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
	return modulus_inv2(modulus);
}

BIGUINT BIGUINT::modulus_inv1(const BIGUINT& modulus) const
{
	if (*this == 0 || modulus == 0 || this->gcd(modulus) != 1)//不存在逆元! 返回0
		return 0;
	BIGUINT result(1);
	while ((result < modulus) &&
		this->modulus_mul(result, modulus) != 1)//遍历
		result++;
	if (result != modulus)
		return result;
	else return 0;
}

BIGUINT BIGUINT::modulus_inv2(const BIGUINT& modulus) const
{
	//解同余方程:ax=1 (mod n)  ->  解方程ax = 1 + bn , 其中 a,n已知，b,x都为小于n的正整数,需要返回的是x
	// 问题转换成（1+bn)%a=0的时候，求(1+bn)/a, 即(1 + b*modulus)/*this
	if (*this == 1)
		return 1;
	else if (modulus == 2)
		if (this->byte_part[sizeof(this->byte_part) - 1] & 1)
			return 1;
		else return 0;
	if (*this == 0 || modulus == 0 || this->gcd(modulus) != 1)//不存在逆元! 返回0
		return 0;

	BIGUINT temp1 = 1,*b=new BIGUINT(0), * temp2 = new BIGUINT(modulus%*this), count = BIGUINT(0),*inc=new BIGUINT(modulus.int_dev(*this));
	while (*b < *this) {//o(nlog(n)
		*b = *b + 1;//用++导致出现错误
		temp1 += *temp2;
		count += *inc;
		count += temp1.int_dev(*this);
		temp1 %= *this;
		//*temp2 = *this;
		if (temp1  == 0) {
			delete b;
			delete temp2;
			delete inc;
			//delete count;
			return count;
		}
	}
}



BIGUINT BIGUINT::zero()
{
	return 0;
}

BIGUINT BIGUINT::max_biguint()
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

	return BIGUINT(odd_buffer, sizeof(BIGUINT));

}
BIGUINT BIGUINT::gen_random_odd(uint32_t bits) {
	BIGUINT result = gen_random_integer(bits);
	result.byte_part[sizeof(BIGUINT) - 1] |= 0x1;
	return result;
}

BIGUINT BIGUINT::gen_random_prime(uint32_t bits, uint32_t attempts)
{
	//生成大素数 失败返回0
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


