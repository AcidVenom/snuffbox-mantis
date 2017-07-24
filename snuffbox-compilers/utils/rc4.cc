#include "rc4.h"
#include <memory>
#include <stdio.h>
#include <string.h>

#define RC4_SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		RC4::RC4()
		{
			Clear();
		}

		//-----------------------------------------------------------------------------------------------
		void RC4::Encrypt(char* to_encrypt, const char* key)
		{
			int i = 0;
			int j = 0;
			int n = 0;

			int len;

			unsigned char k;

			len = static_cast<int>(strlen(key));

			for (int m = 0; m < 256; ++m)
			{
				*(key_ + m) = *(key + (m % len));
				*(chunk_ + m) = m;
			}

			for (int m = 0; m < 256; ++m)
			{
				n = (n + *(chunk_ + m) + *(key_ + m)) & 0xFF;
				RC4_SWAP(*(chunk_ + m), *(chunk_ + n));
			}

			len = static_cast<int>(strlen(to_encrypt));
			for (int m = 0; m < len; ++m)
			{
				i = (i + 1) & 0xFF;
				j = (j + *(chunk_ + i)) & 0xFF;
				RC4_SWAP(*(chunk_ + i), *(chunk_ + j));

				k = *(chunk_ + ((*(chunk_ + i) + *(chunk_ + j)) & 0xFF));
				
				if (k == *(to_encrypt + m))
				{
					k = 0;
				}

				*(to_encrypt + m) ^= k;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void RC4::Decrypt(char* to_encrypt, const char* key)
		{
			Encrypt(to_encrypt, key);
		}

		//-----------------------------------------------------------------------------------------------
		RC4::~RC4()
		{
			Clear();
		}

		//-----------------------------------------------------------------------------------------------
		void RC4::Clear()
		{
			memset(chunk_, NULL, 256);
			memset(key_, NULL, 256);
		}
	}
}
