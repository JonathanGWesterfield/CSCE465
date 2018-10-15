#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
/*
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
 */

using namespace std;


const unsigned char key[] = "0123456789abcdeF";
const unsigned char iv[] = "0000000000000000";


// Read all of our words from the file
vector<string> readFile(string filename)
{
    string word;
    ifstream Data; // Input filestream object
    vector<string> wordList;

    Data.open(filename);
    if (!Data.is_open())
    {
        // If file could not be found or opened then the
        // program shuts down
        cout << "ERROR: FILE COUD NOT BE OPENED" << endl;
        cout << "EXITING PROGRAM" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "File found and good" << endl;
        while (!Data.eof())
        {
            Data >> word;
            wordList.push_back(word);
        }

        Data.close();
    }

    /* Print for testing
    for(int i = 0; i < wordList.size(); i++)
    {
        cout << wordList.at(i) << endl;
    }
    */

    return wordList;
}

// append 0x20 to every word in the vector so that the length of each word is 128bit
vector<string> append0x20(vector<string> wordlist)
{
    char binaryWord[128];
    vector<char[]> binarylist;
    for(int i = 0; i < wordlist.size(); i++)
    {
        binaryWord = bitset<128>(wordlist.at(i).c_str());

    }
}

/** We are going to brute force the password. From the given word list, we append 0x20
 * to each word in order to make the key length 128 bit. From there we apply every
 * single word to break the encryption and see if the key we provide works.
 * @return
 */
int main() {
    string fileName = "words.txt";

    vector<string> words = readFile(fileName);
    return 0;
}

/*
int do_crypt(FILE *in, FILE *out, int do_encrypt)
{
    // Allow enough space in output buffer for additional block
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    EVP_CIPHER_CTX *ctx;

    // Bogus key and IV: we'd normally set these from
    // another source.

    unsigned char key[] = "0123456789abcdeF";
    unsigned char iv[] = "1234567887654321";

    // Don't set key or IV right away; we want to check lengths
    ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL,
                      do_encrypt);
    OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
    OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);

    // Now we can set key and IV
    EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, do_encrypt);

    for (;;) {
        inlen = fread(inbuf, 1, 1024, in);
        if (inlen <= 0)
            break;
        if (!EVP_CipherUpdate(ctx, outbuf, &outlen, inbuf, inlen)) {
            // Error
            EVP_CIPHER_CTX_free(ctx);
            return 0;
        }
        fwrite(outbuf, 1, outlen, out);
    }
    if (!EVP_CipherFinal_ex(ctx, outbuf, &outlen)) {
        // Error
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_free(ctx);
    return 1;
} */