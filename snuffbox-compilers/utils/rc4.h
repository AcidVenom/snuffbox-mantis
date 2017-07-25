#pragma once

namespace snuffbox
{
	namespace compilers
	{
		/**
		* @class snuffbox::compilers::RC4
		* @brief RC4 encryption helper class, used to encrypt raw data into an unreadable format
		* @author Daniel Konings
		*/
		class RC4
		{

		public:

			/**
			* @brief Default constructor, sets the entire chunk and key to 0
			*/
			RC4();

			/**
			* @brief Encrypts the input string with a provided key using RC4 encryption
			* @param[in,out] to_encrypt (char*) The text value to encrypt, this is also where the encrypted value is stored
			* @param[in] len (size_t) The length of the data
			* @param[in] key (const char*) The key salt to encrypt the value with
			*/
			void Encrypt(char* to_encrypt, size_t len, const char* key);

			/**
			* @see snuffbox::compilers::RC4::Encrypt
			* @remarks Basically runs the encryption again, essentially decrypting it
			*/
			void Decrypt(char* to_encrypt, size_t len, const char* key);

			/**
			* @brief Default destructor, sets the entire chunk and key to 0
			*/
			~RC4();

		protected:

			/**
			* @brief Sets the memory of the entire chunk and key to 0
			*/
			void Clear();

		private:

			unsigned char chunk_[256]; //!< The encryption array
			unsigned char key_[256]; //!< The encrypted key
		};
	}
}