#include"crypt_aes.h"
#include <memory.h>
#include <stdio.h>
#include <random>//for ISO10126 padding
std::random_device rd;

AES_KEY::AES_KEY(const uint8_t* key_buffer, int key_size)
{
	length = key_size;
	key = new uint8_t[length];
	memcpy(key, key_buffer, length);
}
AES_KEY::~AES_KEY()
{
	if (key)
		delete key;
}

const uint8_t crypt_aes::sbox[] = {
//0		1		2		3		4		5		6		7		8		9		A		B		C		D		E		F
0x63,	0x7c,	0x77,	0x7b,	0xf2,	0x6b,	0x6f,	0xc5,	0x30,	0x01,	0x67,	0x2b,	0xfe,	0xd7,	0xab,	0x76,//0
0xca,	0x82,	0xc9,	0x7d,	0xfa,	0x59,	0x47,	0xf0,	0xad,	0xd4,	0xa2,	0xaf,	0x9c,	0xa4,	0x72,	0xc0,//1
0xb7,	0xfd,	0x93,	0x26,	0x36,	0x3f,	0xf7,	0xcc,	0x34,	0xa5,	0xe5,	0xf1,	0x71,	0xd8,	0x31,	0x15,//2
0x04,	0xc7,	0x23,	0xc3,	0x18,	0x96,	0x05,	0x9a,	0x07,	0x12,	0x80,	0xe2,	0xeb,	0x27,	0xb2,	0x75,//3
0x09,	0x83,	0x2c,	0x1a,	0x1b,	0x6e,	0x5a,	0xa0,	0x52,	0x3b,	0xd6,	0xb3,	0x29,	0xe3,	0x2f,	0x84,//4
0x53,	0xd1,	0x00,	0xed,	0x20,	0xfc,	0xb1,	0x5b,	0x6a,	0xcb,	0xbe,	0x39,	0x4a,	0x4c,	0x58,	0xcf,//5
0xd0,	0xef,	0xaa,	0xfb,	0x43,	0x4d,	0x33,	0x85,	0x45,	0xf9,	0x02,	0x7f,	0x50,	0x3c,	0x9f,	0xa8,//6
0x51,	0xa3,	0x40,	0x8f,	0x92,	0x9d,	0x38,	0xf5,	0xbc,	0xb6,	0xda,	0x21,	0x10,	0xff,	0xf3,	0xd2,//7
0xcd,	0x0c,	0x13,	0xec,	0x5f,	0x97,	0x44,	0x17,	0xc4,	0xa7,	0x7e,	0x3d,	0x64,	0x5d,	0x19,	0x73,//8
0x60,	0x81,	0x4f,	0xdc,	0x22,	0x2a,	0x90,	0x88,	0x46,	0xee,	0xb8,	0x14,	0xde,	0x5e,	0x0b,	0xdb,//9
0xe0,	0x32,	0x3a,	0x0a,	0x49,	0x06,	0x24,	0x5c,	0xc2,	0xd3,	0xac,	0x62,	0x91,	0x95,	0xe4,	0x79,//A
0xe7,	0xc8,	0x37,	0x6d,	0x8d,	0xd5,	0x4e,	0xa9,	0x6c,	0x56,	0xf4,	0xea,	0x65,	0x7a,	0xae,	0x08,//B
0xba,	0x78,	0x25,	0x2e,	0x1c,	0xa6,	0xb4,	0xc6,	0xe8,	0xdd,	0x74,	0x1f,	0x4b,	0xbd,	0x8b,	0x8a,//C
0x70,	0x3e,	0xb5,	0x66,	0x48,	0x03,	0xf6,	0x0e,	0x61,	0x35,	0x57,	0xb9,	0x86,	0xc1,	0x1d,	0x9e,//D
0xe1,	0xf8,	0x98,	0x11,	0x69,	0xd9,	0x8e,	0x94,	0x9b,	0x1e,	0x87,	0xe9,	0xce,	0x55,	0x28,	0xdf,//E
0x8c,	0xa1,	0x89,	0x0d,	0xbf,	0xe6,	0x42,	0x68,	0x41,	0x99,	0x2d,	0x0f,	0xb0,	0x54,	0xbb,	0x16 //F
};
const uint8_t crypt_aes::invsbox[] = {
0x52,	0x09,	0x6a,	0xd5,	0x30,	0x36,	0xa5,	0x38,	0xbf,	0x40,	0xa3,	0x9e,	0x81,	0xf3,	0xd7,	0xfb,
0x7c,	0xe3,	0x39,	0x82,	0x9b,	0x2f,	0xff,	0x87,	0x34,	0x8e,	0x43,	0x44,	0xc4,	0xde,	0xe9,	0xcb,
0x54,	0x7b,	0x94,	0x32,	0xa6,	0xc2,	0x23,	0x3d,	0xee,	0x4c,	0x95,	0x0b,	0x42,	0xfa,	0xc3,	0x4e,
0x08,	0x2e,	0xa1,	0x66,	0x28,	0xd9,	0x24,	0xb2,	0x76,	0x5b,	0xa2,	0x49,	0x6d,	0x8b,	0xd1,	0x25,
0x72,	0xf8,	0xf6,	0x64,	0x86,	0x68,	0x98,	0x16,	0xd4,	0xa4,	0x5c,	0xcc,	0x5d,	0x65,	0xb6,	0x92,
0x6c,	0x70,	0x48,	0x50,	0xfd,	0xed,	0xb9,	0xda,	0x5e,	0x15,	0x46,	0x57,	0xa7,	0x8d,	0x9d,	0x84,
0x90,	0xd8,	0xab,	0x00,	0x8c,	0xbc,	0xd3,	0x0a,	0xf7,	0xe4,	0x58,	0x05,	0xb8,	0xb3,	0x45,	0x06,
0xd0,	0x2c,	0x1e,	0x8f,	0xca,	0x3f,	0x0f,	0x02,	0xc1,	0xaf,	0xbd,	0x03,	0x01,	0x13,	0x8a,	0x6b,
0x3a,	0x91,	0x11,	0x41,	0x4f,	0x67,	0xdc,	0xea,	0x97,	0xf2,	0xcf,	0xce,	0xf0,	0xb4,	0xe6,	0x73,
0x96,	0xac,	0x74,	0x22,	0xe7,	0xad,	0x35,	0x85,	0xe2,	0xf9,	0x37,	0xe8,	0x1c,	0x75,	0xdf,	0x6e,
0x47,	0xf1,	0x1a,	0x71,	0x1d,	0x29,	0xc5,	0x89,	0x6f,	0xb7,	0x62,	0x0e,	0xaa,	0x18,	0xbe,	0x1b,
0xfc,	0x56,	0x3e,	0x4b,	0xc6,	0xd2,	0x79,	0x20,	0x9a,	0xdb,	0xc0,	0xfe,	0x78,	0xcd,	0x5a,	0xf4,
0x1f,	0xdd,	0xa8,	0x33,	0x88,	0x07,	0xc7,	0x31,	0xb1,	0x12,	0x10,	0x59,	0x27,	0x80,	0xec,	0x5f,
0x60,	0x51,	0x7f,	0xa9,	0x19,	0xb5,	0x4a,	0x0d,	0x2d,	0xe5,	0x7a,	0x9f,	0x93,	0xc9,	0x9c,	0xef,
0xa0,	0xe0,	0x3b,	0x4d,	0xae,	0x2a,	0xf5,	0xb0,	0xc8,	0xeb,	0xbb,	0x3c,	0x83,	0x53,	0x99,	0x61,
0x17,	0x2b,	0x04,	0x7e,	0xba,	0x77,	0xd6,	0x26,	0xe1,	0x69,	0x14,	0x63,	0x55,	0x21,	0x0c,	0x7d
};
int crypt_aes::work_mode = AES_MODE_ECB;
int crypt_aes::padding_mode = AES_PADDING_PKCS;
bool crypt_aes::zeropadding_warning = true;
char crypt_aes::iv[16] = {0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t crypt_aes::mul2(uint8_t x)
{
	if (x & 0x80) {
		x <<= 1;
		x ^= 0x1b;
	}
	else
		x <<= 1;
	return x;
}
void crypt_aes::set_work_mode(int new_mode)
{
	work_mode = new_mode;
}

void crypt_aes::set_padding_mode(int mode)
{
	padding_mode = mode;
}

void crypt_aes::set_iv(const uint8_t iv_buffer[AES_BLOCK_SIZE])
{
	memcpy(iv, iv_buffer, AES_BLOCK_SIZE);
}

void crypt_aes::show_zeropadding_warning(bool option)
{
	zeropadding_warning = option;
}

int crypt_aes::padding(uint8_t* dst_packaddr, const uint8_t* src_buf, int src_buf_size)
{
	memcpy(dst_packaddr, src_buf, src_buf_size);
	switch (padding_mode)
	{
	case AES_PADDING_PKCS:
		memset(dst_packaddr + src_buf_size, AES_BLOCK_SIZE - src_buf_size, AES_BLOCK_SIZE - src_buf_size);
		break;
	case AES_PADDING_ISO10126:
		memset(dst_packaddr + src_buf_size, rd(), AES_BLOCK_SIZE - src_buf_size - 1);
		memset(dst_packaddr + AES_BLOCK_SIZE - 1, AES_BLOCK_SIZE - src_buf_size, 1);
		break;
	case AES_PADDING_ISO7816_4:
		memset(dst_packaddr + src_buf_size, 0x80,  1);
		memset(dst_packaddr + src_buf_size + 1, 0, AES_BLOCK_SIZE - src_buf_size - 1);
		break;
	case AES_PADDING_X923:
		memset(dst_packaddr + src_buf_size, 0, AES_BLOCK_SIZE - src_buf_size - 1);
		memset(dst_packaddr + AES_BLOCK_SIZE - 1, AES_BLOCK_SIZE - src_buf_size, 1);
		break;
	case AES_PADDING_TBC:
		memset(dst_packaddr + src_buf_size, *(dst_packaddr + src_buf_size - 1) & 1 - 1, AES_BLOCK_SIZE - src_buf_size);
		break;
	case AES_PADDING_ZERO:
		memset(dst_packaddr + src_buf_size, 0, AES_BLOCK_SIZE - src_buf_size);
		break;
	case AES_PADDING_NOPADDING:
		fprintf(stderr, "for  AES_PADDING_NOPADDING,needless to padd anything.\n");
		return 0;
		break;
	default:
		break;
	}
	return AES_BLOCK_SIZE - src_buf_size;
}
int crypt_aes::unpadding(uint8_t* dst_padded_buf)
{
	switch (padding_mode)
	{
	case AES_PADDING_PKCS: {
		int padding_size = dst_padded_buf[AES_BLOCK_SIZE - 1];
		bool warning_flag = 0;
		for (int i = AES_BLOCK_SIZE - padding_size; i < AES_BLOCK_SIZE; i++) {
			if (dst_padded_buf[i] != padding_size)
				warning_flag = true;
			dst_padded_buf[i] = 0;
		}
		if (warning_flag)
			fprintf(stderr, "warning in AES_PADDING_PKCS:wrong padding mode.\n");
		return padding_size;
		break; 
	}
	case AES_PADDING_ISO10126: {
		int padding_size = dst_padded_buf[AES_BLOCK_SIZE - 1];
		for (int i = AES_BLOCK_SIZE - padding_size; i < AES_BLOCK_SIZE; i++)
			dst_padded_buf[i] = 0;
		return padding_size;
		break; 
	}
	case AES_PADDING_ISO7816_4: {
		bool warning_flag = 0;
		for (int i = AES_BLOCK_SIZE; i > 0; i--)
			if (dst_padded_buf[i] == 0)
				continue;
			else if (dst_padded_buf[i] == 0x80) {
				dst_padded_buf[i] = 0;
				if (warning_flag)
					fprintf(stderr, "warning in AES_PADDING_ISO7816-4:wrong padding mode.\n");
				return AES_BLOCK_SIZE - i;
			}
			else {
				warning_flag = true;
				dst_padded_buf[i] = 0;
			}
		fprintf(stderr, "error in AES_PADDING_ISO7816-4:wrong padding mode , 0x80 not found\n");
		exit(-1);
		break;

	}
	case AES_PADDING_X923: {
		for (int i = AES_BLOCK_SIZE - dst_padded_buf[AES_BLOCK_SIZE - 1]; i < AES_BLOCK_SIZE - 1; i++)
			if (dst_padded_buf[i] != 0) {
				fprintf(stderr, "warning in AES_PADDING_X923:wrong padding mode.\n");
				dst_padded_buf[i] = 0;
			}
		int padding_size = dst_padded_buf[AES_BLOCK_SIZE - 1];
		dst_padded_buf[AES_BLOCK_SIZE - 1] = 0;
		return padding_size;
		break;
	}
	case AES_PADDING_TBC: {
		switch (dst_padded_buf[AES_BLOCK_SIZE-1])
		{
		case 0: {
			int i = 0;
			for (i = AES_BLOCK_SIZE; (i != -1) && (dst_padded_buf[i] == 0); i--);
			if (((dst_padded_buf[i] & 1) == 0))
				fprintf(stderr, "warning in AES_PADDING_TBC:wrong padding mode.\n");
			return AES_BLOCK_SIZE - i - 1;
			break;
		}
		case 0xff: {
			int i = 0;
			for (int i = AES_BLOCK_SIZE; (i != -1) && (dst_padded_buf[i] == 0xff); i--);
			if ((dst_padded_buf[i] & 1) == 1)
				fprintf(stderr, "warning in AES_PADDING_TBC:wrong padding mode.\n");
			return AES_BLOCK_SIZE - i - 1;
			break;
		}
		default:
			fprintf(stderr, "error in AES_PADDING_TBC:wrong padding mode.\n");
			exit(-1);
			break;
		}
	}
	case AES_PADDING_ZERO: {
		if(zeropadding_warning)
			fprintf(stderr, "warning: the padding size of AES_PADDING_ZERO my be wrong, some buffer can terminate with 0x00\n hide this message by crypt_aes::show_zeropadding_warning.\n");
		int i = 0;
		for (i = AES_BLOCK_SIZE; (i != -1) && (dst_padded_buf[i] == 0); i--);
		return AES_BLOCK_SIZE - i - 1;
	}
	case AES_PADDING_NOPADDING: {

		return 0;
	}
	}
}

const uint8_t crypt_aes128::rcon[] = {
	1,2,4,8,0x10,0x20,0x40,0x80,0x1B,0x36
};
int crypt_aes128::cryptor(const uint8_t* plaintext_block, const uint8_t* round_keys, uint8_t* state)
{

	//copy plaintext
	//memcpy(state, plaintext, AES_PACK_SIZE);
	for (int i = 0; i < 4; i++)//trans to column mode.
		for (int j = 0; j < 4; j++)
			state[4 * i + j] = plaintext_block[4 * j + i];
	//add round key
	for (int j = 0; j < AES_BLOCK_SIZE; j++)
		state[j] ^= round_keys[j];

	for (int i = 1; i < AES_128_NR; i++) {

		//substitute bytes
		for (int j = 0; j < AES_BLOCK_SIZE; j++)
			state[j] = sbox[state[j]];
		//shift rows
		uint8_t temp = state[1];
		state[1] = state[5];
		state[5] = state[9];
		state[9] = state[13];
		state[13] = temp;

		temp = state[2];
		state[2] = state[10];
		state[10] = temp;
		temp = state[6];
		state[6] = state[14];
		state[14] = temp;

		temp = state[3];
		state[3] = state[15];
		state[15] = state[11];
		state[11] = state[7];
		state[7] = temp;

		//mix cols
		/*mix matrix
		* 2 3 1 1
		* 1 2 3 1
		* 1 1 2 3
		* 3 1 1 2
		*/
		uint8_t state2[AES_BLOCK_SIZE];
		memcpy(state2, state, AES_BLOCK_SIZE);
		for (int k = 0; k <= 12; k += 4) {
			//[02 03 01 01]  *  col[0:4] -> xmul s(0,i/4)
			state[k] = state2[k] ^ state2[k + 1];
			if (state[k] & 0x80) {
				state[k] <<= 1;
				state[k] ^= 0x1b;
			}
			else
				state[k] <<= 1;
			state[k] = state[k] ^ state2[k + 1] ^ state2[k + 2] ^ state2[k + 3];
			//[01 02 03 01]  *  col[0:4] -> xmul s(0,i/4+1)
			state[k + 1] = state2[k + 1] ^ state2[k + 2];
			if (state[k + 1] & 0x80) {
				state[k + 1] <<= 1;
				state[k + 1] ^= 0x1b;
			}
			else
				state[k + 1] <<= 1;
			state[k + 1] = state2[k] ^ state[k + 1] ^ state2[k + 2] ^ state2[k + 3];
			//[01 01 02 03]  *  col[0:4] -> xmul s(0,i/4+2)
			state[k + 2] = state2[k + 2] ^ state2[k + 3];
			if (state[k + 2] & 0x80) {
				state[k + 2] <<= 1;
				state[k + 2] ^= 0x1b;
			}
			else
				state[k + 2] <<= 1;
			state[k + 2] = state2[k] ^ state2[k + 1] ^ state[k + 2] ^ state2[k + 3];
			//[03 01 01 02]  *  col[0:4] -> xmul s(0,i/4+3)
 			state[k + 3] = state2[k + 3] ^ state2[k];
			if (state[k + 3] & 0x80) {
				state[k + 3] <<= 1;
				state[k + 3] ^= 0x1b;
			}
			else
				state[k + 3] <<= 1;
			state[k + 3] = state2[k] ^ state2[k + 1] ^ state2[k + 2] ^ state[k + 3];
		}
		//add round key
		for (int j = 0; j < AES_BLOCK_SIZE; j++)
			state[j] ^= round_keys[AES_BLOCK_SIZE * i + j];
	}
	//for round 10:
	//substitute bytes
	for (int j = 0; j < AES_BLOCK_SIZE; j++)
		state[j] = sbox[state[j]];
	//shift rows
	uint8_t temp = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp;

	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;

	temp = state[3];
	state[3] = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = temp;

	//add round key
	for (int j = 0; j < AES_BLOCK_SIZE; j++)
		state[j] ^= round_keys[AES_128_NR * AES_128_KEY_SIZE + j];

	return 0;
}
int crypt_aes128::decryptor(const uint8_t* ciphertext_block, const uint8_t* round_keys, uint8_t* state)
{
	//copy ciphertext
	//memcpy(state,ciphertext, AES_PACK_SIZE);
	uint8_t temp33[16];
	memcpy(temp33, ciphertext_block, 16);
	for (int i = 0; i < 4; i++)//trans to column mode.
		for (int j = 0; j < 4; j++)
			state[4 * i + j] = ciphertext_block[4 * j + i];

	//add round key (inverse)
	for (int j = 0; j < AES_BLOCK_SIZE; j++)
		state[j] ^= round_keys[AES_128_NR * AES_128_KEY_SIZE + j];
	for (int i = 1; i < AES_128_NR; i++) {
		//inverse shift rows
		uint8_t temp = state[13];
		state[13] = state[9];
		state[9] = state[5];
		state[5] = state[1];
		state[1] = temp;

		temp = state[2];
		state[2] = state[10];
		state[10] = temp;
		temp = state[6];
		state[6] = state[14];
		state[14] = temp;

		temp = state[3];
		state[3] = state[7];
		state[7] = state[11];
		state[11] = state[15];
		state[15] = temp;

		//inverse substitute bytes
		for (int j = 0; j < AES_BLOCK_SIZE; j++)
			state[j] = invsbox[state[j]];

		//add round key
		for (int j = 0; j < AES_BLOCK_SIZE; j++)
			state[j] ^= round_keys[AES_BLOCK_SIZE * (AES_128_NR - i) + j];

		//inv mix cols
		uint8_t state2[AES_BLOCK_SIZE];
		memcpy(state2, state, AES_BLOCK_SIZE);
		uint8_t temp1 = 0, temp2 = 0;
		for (int i = 0; i < 16; i += 4) {
			//[0E 0B 0D 09]  *  col[0:4] -> xmul s(0,i/4)
			//state2[i] * 0xE 0x1110
			temp1 = mul2(state2[i]);
			temp2 = mul2(temp1);
			state[i] = mul2(temp2) ^ temp2 ^ temp1;
			//state2[i+1] * 0xB 0x1011
			temp1 = mul2(state2[i + 1]);
			temp2 = mul2(temp1);
			state[i] ^= mul2(temp2) ^ temp1 ^ state2[i + 1];
			//state2[i+2] * 0xD 0x1101
			temp1 = mul2(state2[i + 2]);
			temp2 = mul2(temp1);
			state[i] ^= mul2(temp2) ^ temp2 ^ state2[i + 2];
			//state2[i+3] * 0x9 0x1001
			temp1 = mul2(state2[i + 3]);
			temp2 = mul2(temp1);
			state[i] ^= mul2(temp2) ^ state2[i + 3];


			//[09 0E 0B 0D]  *  col[0:4] -> xmul s(0,i/4+1)
			//state2[i] * 0x9 0x1001
			temp1 = mul2(state2[i]);
			temp2 = mul2(temp1);
			state[i + 1] = mul2(temp2) ^ state2[i];
			//state2[i+1] * 0xE 0x1110
			temp1 = mul2(state2[i+1]);
			temp2 = mul2(temp1);
			state[i + 1] ^= mul2(temp2) ^ temp2 ^ temp1;
			//state2[i+2] * 0xB 0x1011
			temp1 = mul2(state2[i + 2]);
			temp2 = mul2(temp1);
			state[i + 1] ^= mul2(temp2) ^ temp1 ^ state2[i + 2];
			//state2[i+3] * 0xD 0x1101
			temp1 = mul2(state2[i + 3]);
			temp2 = mul2(temp1);
			state[i + 1] ^= mul2(temp2) ^ temp2 ^ state2[i + 3];


			//[0D 09 0E 0B]  *  col[0:4] -> xmul s(0,i/4+2)
			//state2[i] * 0xD 0x1101
			temp1 = mul2(state2[i]);
			temp2 = mul2(temp1);
			state[i + 2] = mul2(temp2) ^ temp2 ^ state2[i];
			//state2[i+1] * 0x9 0x1001
			temp1 = mul2(state2[i + 1]);
			temp2 = mul2(temp1);
			state[i + 2] ^= mul2(temp2) ^ state2[i + 1];
			//state2[i+2] * 0xE 0x1110
			temp1 = mul2(state2[i + 2]);
			temp2 = mul2(temp1);
			state[i + 2] ^= mul2(temp2) ^ temp2 ^ temp1;
			//state2[i+3] * 0xB 0x1011
			temp1 = mul2(state2[i + 3]);
			temp2 = mul2(temp1);
			state[i + 2] ^= mul2(temp2) ^ temp1 ^ state2[i + 3];


			//[0B 0D 09 0E]  *  col[0:4] -> xmul s(0,i/4+3)
			//state2[i] * 0xB 0x1011
			temp1 = mul2(state2[i]);
			temp2 = mul2(temp1);
			state[i + 3] = mul2(temp2) ^ temp1 ^ state2[i];
			//state2[i+1] * 0xD 0x1101
			temp1 = mul2(state2[i + 1]);
			temp2 = mul2(temp1);
			state[i + 3] ^= mul2(temp2) ^ temp2 ^ state2[i + 1];
			//state2[i+2] * 0x9 0x1001
			temp1 = mul2(state2[i + 2]);
			temp2 = mul2(temp1);
			state[i + 3] ^= mul2(temp2) ^ state2[i + 2];
			//state2[i+3] * 0xE 0x1110
			temp1 = mul2(state2[i + 3]);
			temp2 = mul2(temp1);
			state[i + 3] ^= mul2(temp2) ^ temp2 ^ temp1;
		}

	}


	//for round 10
	//inverse shift rows
	uint8_t temp = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = state[1];
	state[1] = temp;

	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;

	temp = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = temp;

	//inverse substitute bytes
	for (int j = 0; j < AES_BLOCK_SIZE; j++)
		state[j] = invsbox[state[j]];

	//add round key
	for (int j = 0; j < AES_BLOCK_SIZE; j++)
		state[j] ^= round_keys[j];

	return 0;
}
uint64_t crypt_aes128::crypt(const uint8_t* plaintext, uint64_t text_size, const AES_KEY& key, uint8_t* lpcrypted_buf,uint64_t buf_size)
{
	uint8_t *round_keys = new uint8_t[(1 + AES_128_NR) * AES_128_KEY_SIZE];
	memset(round_keys, 0, (1 + AES_128_NR) * AES_128_KEY_SIZE);
	uint8_t* state = new uint8_t[AES_BLOCK_SIZE];
	memset(state, 0, AES_BLOCK_SIZE);
	key_extend(key,round_keys, (1 + AES_128_NR) * AES_128_KEY_SIZE);
	int remain_size =  text_size % AES_BLOCK_SIZE;// padding is necessary even the  remain_size=0  ,which means  padding block is actually meaningless.
	
	uint64_t blocks = text_size / AES_BLOCK_SIZE;
	if (buf_size < text_size + AES_BLOCK_SIZE - remain_size)
		return -1;//error

	switch (work_mode)
	{
	case AES_MODE_ECB: {
		for (int i = 0; i < blocks; i++) {
			cryptor(plaintext + i * AES_BLOCK_SIZE, round_keys, state);
			uint8_t state2[16] = {};
			memcpy(state2, state, 16);
			//state: column mode
			//memcpy(*lpcrypted_buf + i * AES_PACK_SIZE, state, AES_PACK_SIZE);
			for (int j = 0; j < 4; j++)//trans to column mode.
				for (int k = 0; k < 4; k++)
					//state[4 * i + j] = plaintext_pack[4 * j + i];
					(lpcrypted_buf + i * AES_BLOCK_SIZE)[4 * j + k] = state[4 * k + j];
		}

		//padding
		uint8_t* padding_text = new uint8_t[AES_BLOCK_SIZE];
		if (padding_mode == AES_PADDING_NOPADDING) {

			fprintf(stderr, "error:AES_PADDING_NOPADDING dosent work on  work mode AES_MODE_ECB.\n");
			exit(-1);
		}
		padding(padding_text, plaintext + blocks * AES_BLOCK_SIZE, remain_size);
		cryptor(padding_text, round_keys, state);
		//memcpy(*lpcrypted_buf + packs * AES_PACK_SIZE, state, AES_PACK_SIZE);
		for (int j = 0; j < 4; j++)//trans to column mode.
			for (int k = 0; k < 4; k++)
				//state[4 * i + j] = plaintext_pack[4 * j + i];
				(lpcrypted_buf + blocks * AES_BLOCK_SIZE)[4 * j + k] = state[4 * k + j];
		delete[] padding_text;
		break;
	}
	case AES_MODE_CBC: {
		uint8_t* xor_block = new uint8_t[AES_BLOCK_SIZE];
		//uint8_t xor_block[16] = {};
		for (int i = 0; i < AES_BLOCK_SIZE; i++)
			xor_block[i] = iv[i] ^ plaintext[i];
		char state2[16];
		for (int i = 0; i < blocks - 1; i++) {
			cryptor(xor_block, round_keys, state);
			memcpy(state2, state, 16);
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++) {
					xor_block[4 * k + j] = state[4 * j + k] ^ (plaintext + (i + 1) * AES_BLOCK_SIZE)[4 * k + j];
					(lpcrypted_buf + i * AES_BLOCK_SIZE)[4 * k + j] = state[4 * j + k];
				}
		}

		//padding
		cryptor(xor_block, round_keys, state);

		if (padding_mode == AES_PADDING_NOPADDING) {

			fprintf(stderr, "error:AES_PADDING_NOPADDING dosent work on  work mode AES_MODE_CBC.\n");
			exit(-1);
		}
		padding(xor_block, plaintext + blocks * AES_BLOCK_SIZE, remain_size);

 		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++) {
				xor_block[4 * k + j] ^= state[4 * j + k];
				(lpcrypted_buf + (blocks - 1) * AES_BLOCK_SIZE)[4 * k + j] = state[4 * j + k];
			}
	
		cryptor(xor_block, round_keys, state);
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				(lpcrypted_buf + blocks * AES_BLOCK_SIZE)[4 * j + k] = state[4 * k + j];
		delete[] xor_block;
		break;
	}

	//case 0:
	}

	delete[] round_keys;
	delete[] state;
	return text_size + AES_BLOCK_SIZE - remain_size;
}
uint64_t crypt_aes128::decrypt(uint8_t* ciphertext, uint64_t text_size, const AES_KEY& key, uint8_t* lpdecrypted_buf, uint64_t buf_size)
{
	uint8_t* round_keys = new uint8_t[(1 + AES_128_NR) * AES_128_KEY_SIZE];
	memset(round_keys, 0, (1 + AES_128_NR) * AES_128_KEY_SIZE);
	uint8_t* state = new uint8_t[AES_BLOCK_SIZE];
	memset(state, 0, AES_BLOCK_SIZE);
	key_extend(key, round_keys, (1 + AES_128_NR) * AES_128_KEY_SIZE);
	if (text_size % AES_BLOCK_SIZE)
		return -1;//error
	uint64_t packs = text_size / AES_BLOCK_SIZE;
	if (buf_size < text_size)
		return -1;//error

	switch (work_mode)
	{
	case AES_MODE_ECB: {
		for (int i = 0; i < packs; i++) {
			decryptor(ciphertext + i * AES_BLOCK_SIZE, round_keys, state);
			//state: column mode
			//memcpy(*lpcrypted_buf + i * AES_PACK_SIZE, state, AES_PACK_SIZE);
			for (int j = 0; j < 4; j++)//trans to column mode.
				for (int k = 0; k < 4; k++)
					//state[4 * i + j] = plaintext_pack[4 * j + i];
					(lpdecrypted_buf + i * AES_BLOCK_SIZE)[4 * j + k] = state[4 * k + j];
		}
		return  packs * AES_BLOCK_SIZE - unpadding(lpdecrypted_buf + (packs - 1) * AES_BLOCK_SIZE);
		break;
	}
	case AES_MODE_CBC: {
		for (int i = 0; i < packs - 1; i++) {
			decryptor(ciphertext + (i + 1) * AES_BLOCK_SIZE, round_keys, state);
			//state: column mode
			//memcpy(*lpcrypted_buf + i * AES_PACK_SIZE, state, AES_PACK_SIZE);
			for (int j = 0; j < 4; j++)//trans to column mode.
				for (int k = 0; k < 4; k++)
					//state[4 * i + j] = plaintext_pack[4 * j + i];
					(lpdecrypted_buf + (i + 1) * AES_BLOCK_SIZE)[4 * j + k] = state[4 * k + j] ^ (ciphertext + i * AES_BLOCK_SIZE)[4 * j + k];
		}
		decryptor(ciphertext , round_keys, state);
		for (int j = 0; j < 4; j++)//trans to column mode.
			for (int k = 0; k < 4; k++)
				//state[4 * i + j] = plaintext_pack[4 * j + i];
				lpdecrypted_buf[4 * j + k] = state[4 * k + j] ^ iv[4 * j + k];
		return  packs * AES_BLOCK_SIZE - unpadding(lpdecrypted_buf + (packs - 1) * AES_BLOCK_SIZE);
		break;
	}
	}


	delete[] round_keys;
	delete[] state;


	return 0;


}
int crypt_aes128::key_extend(const AES_KEY& key, uint8_t* round_keys,int round_key_size)
{


	//round_keys_init
	memset(round_keys, 0, round_key_size);

	//padding round keys
	//w[0] - w[3]
	memcpy_s(round_keys,round_key_size , key.key, key.length);
	//w[4]-w[44]

	for (int i = 1; i < AES_128_NR + 1; i++) {

		//copt w[4*i-1] to w[4*i]
		memcpy_s(round_keys + AES_128_KEY_SIZE * i, AES_128_WORD_SIZE, round_keys + AES_128_KEY_SIZE * i- AES_128_WORD_SIZE, AES_128_WORD_SIZE);

		//LSHIFT w[4*i]
		uint8_t temp = round_keys[AES_128_KEY_SIZE * i];
		int j = 0;
		while (j < AES_128_WORD_SIZE - 1) {
			round_keys[AES_128_KEY_SIZE  * i + j] = round_keys[AES_128_KEY_SIZE * i + j + 1];
			j++;
		}
		round_keys[AES_128_KEY_SIZE * i + AES_128_WORD_SIZE - 1] = temp;

		//s-box
		for (j = 0; j < AES_128_WORD_SIZE; j++)
			round_keys[AES_128_KEY_SIZE * i + j] = sbox[round_keys[AES_128_KEY_SIZE * i + j]];

		//xor rcon
		round_keys[AES_128_KEY_SIZE * i] ^= rcon[i-1];

		//w[4*(i-1)] xor w[4*(i-1)]
		for (j = 0; j < AES_128_WORD_SIZE; j++)
			round_keys[AES_128_KEY_SIZE * i + j] ^= round_keys[AES_128_KEY_SIZE * (i - 1) + j];
		//w[4*(i)z+(1:3)]
		for (j = AES_128_WORD_SIZE; j < AES_128_KEY_SIZE; j++)
			round_keys[AES_128_KEY_SIZE * i + j] = round_keys[AES_128_KEY_SIZE * (i - 1) + j] ^ round_keys[AES_128_KEY_SIZE * i  + j - AES_128_WORD_SIZE];
	}
	return 0;

}
