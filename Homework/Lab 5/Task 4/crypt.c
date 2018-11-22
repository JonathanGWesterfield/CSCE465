#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// gets the size of an array
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

EVP_MD_CTX *mdctx;
const EVP_MD *md;
char mess1[] = "Test Message\n";
char mess2[] = "Hello World\n";
unsigned char md_value[EVP_MAX_MD_SIZE];
int MAX_SIZE = (int)(65535 / sizeof(int));

// pastHashes[NUMBER_OF_STRINGS][STRING_LENGTH+1];
unsigned char pastHashes[100][6];
int hashIndex = 0;

/**
 * @brief random String generator from stack overflow
 * @param length
 * @return
 */
char *randstring(size_t length)
{

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *randomString;

    if (length) {
        randomString = malloc(length +1);

        if (randomString) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int) (sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }
    // printf("Random Mess1: %s\n", randomString);
    return randomString;
}

/**
 * @brief Get the first 6 chars (24 bits) of the hash
 * @param md_arr
 * @return
 */
char* getSubStr(unsigned char md_arr[EVP_MAX_MD_SIZE])
{
    char subbuff[8];
    memcpy(subbuff, &md_arr[0], 6);
    subbuff[7] = '\0';

    return subbuff;
}

bool compareHash(unsigned char* subHash[6])
{
    printf("Comp hash: ");
    for(int i = 0; i < 6; i++)
        printf("%x ", subHash[i]);
    printf("\n");

    for(int i; i <= hashIndex; i++)
    {
        // Compare our new hash to every single hash in O(n) fashion
        if(strncmp(subHash, pastHashes[i], 6) == 0)
        {
            printf("Hash Match Found!!!\n");
            return true;
        }
    }

    return false;
}
// 24 bits is the first 6 characters
void mesDigest()
{
    int md_len;

    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
    EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    /* printf("Digest is: ");
    for(int i = 0; i < md_len; i++)
        printf("%x ", md_value[i]);
    printf("\n"); */

    char* subHash[6];
    for(int i = 0; i < 6; i++)
        subHash[i] = md_value[i];

    printf("Sub hash: ");
    for(int i = 0; i < 6; i++)
        printf("%x ", subHash[i]);
    printf("\n");

    compareHash(subHash);
    printf("Not found yet\n");

    // store this new hash in our hash arry
    strncpy(pastHashes[hashIndex], md_value, 6);

    printf("Stored Hash: ");
    for (int i = 0; i < 6; i++)
        printf("%x ", pastHashes[hashIndex][i]);
    printf("\n");

    // Increment our hash array
    hashIndex++;


}

main(int argc, char *argv[])
{
    OpenSSL_add_all_digests();

    if(!argv[1]) {
        printf("Usage: ./crypt digestname\n");
        exit(1);
    }

    md = EVP_get_digestbyname(argv[1]);

    if(!md) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
    }

    char* randomString = randstring(12);

    mesDigest();

    /*
    printf("Past Hashes: ");
    for(int i = 0; i < 1; i++) // i < NELEMS(pastHashes); i++)
    {
        for(int j = 0; j < 6; j++)
        {
            printf("%x ", pastHashes[i][j]);
        }
        printf("\n");
    }

    int md_len;

    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
    EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    printf("Digest is: ");
    for(int i = 0; i < md_len; i++)
        printf("%02x", md_value[i]);
    printf("\n"); */

    /* Call this once before exit. */
    EVP_cleanup();
    exit(0);
}