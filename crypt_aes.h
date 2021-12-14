#pragma once
/*
author:SupreME
*/
#include <stdint.h>

#define AES_BLOCK_SIZE 16

//working mode
#define AES_MODE_ECB  0 //default,not safe
#define AES_MODE_CBC  1
#define AES_MODE_CFB  2
#define AES_MODE_OFB  3
#define AES_MODE_CTR  4
#define AES_MODE_PCBC 5

//padding method
#define AES_PADDING_PKCS	  1 //pkcs7,default
#define AES_PADDING_ISO10126  2
#define AES_PADDING_ISO7816_4 3
#define AES_PADDING_X923	  4
#define AES_PADDING_TBC		  5
#define AES_PADDING_ZERO	  6
#define AES_PADDING_NOPADDING 0//for CFB,OFB,CTR

//aes128
#define AES_128_KEY_SIZE 16 //aes128
#define AES_128_NR 10
#define AES_128_WORD_SIZE 4


typedef class AES_KEY {
	uint8_t* key;
	int length;
public:
	AES_KEY(const uint8_t* key_buffer, int key_size);
	~AES_KEY();
	friend  class crypt_aes128;
	friend  class crypt_aes_192;
	friend  class crypt_aes_256;


}aes_key;

class crypt_aes {
protected:
	static const uint8_t sbox[];
	static const uint8_t invsbox[];
	static int work_mode;
	static int padding_mode;
	static bool zeropadding_warning;
	
	 
	//GF(2^8) alg
	static uint8_t mul2(uint8_t x);

	static int padding(uint8_t* dst_packaddr, const uint8_t* src_buf, int src_buf_size);
	static int unpadding(uint8_t* dst_padded_buf);
	static char iv[AES_BLOCK_SIZE];
public:
	static void set_work_mode(int mode);
	static void set_padding_mode(int mode);
	static void set_iv(const uint8_t iv_buffer[AES_BLOCK_SIZE]);
	static void show_zeropadding_warning(bool option);
};


class crypt_aes128 :protected crypt_aes
{
	static const uint8_t rcon[AES_128_NR];
	static int key_extend(const AES_KEY& key,uint8_t *round_keys, int round_key_size);
	static int cryptor(const uint8_t* plaintext_block, const uint8_t* round_keys, uint8_t* state);
	static int decryptor(const uint8_t* ciphertext_block, const uint8_t* round_keys, uint8_t* state);
public:

	//crypt messages
	static uint64_t crypt(
		const uint8_t* plaintext, //[in] plain text buffer to encrypt
		uint64_t text_size,		  //[in] bytes of plain text
		const AES_KEY& key,		  //[in] aes key with which to encrypt the plain text
		uint8_t* lpcrypted_buf,   //[out]a buffer space which is specified to receive the encrypted  buffer
		uint64_t buf_size );	  //[in] bytes of specified buffer 

	//decrypt messages
	static uint64_t decrypt(
		uint8_t* ciphertext,		//[in] plain text buffer to decrypt
		uint64_t text_size,			//[in] bytes of cipher text
		const AES_KEY& key,			//[in] aes key with which to decrypt the plain text
		uint8_t* lpdecrypted_buf,	//[out]a buffer space which is specified to receive the encrypted  buffer
		uint64_t buf_size);			//[in] bytes of specified buffer 
};