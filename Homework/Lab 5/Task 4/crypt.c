#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>
#include <time.h>
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))



EVP_MD_CTX *mdctx;
const EVP_MD *md;
char mess1[] = "Test Message\n";
char mess2[] = "Hello World\n";
unsigned char md_value[EVP_MAX_MD_SIZE];
int MAX_SIZE = (int)(65535 / sizeof(int));

// pastHashes[NUMBER_OF_STRINGS][STRING_LENGTH+1];
char pastHashes[100][6];
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

void storeHash(char subHash)
{
    /* for(int i = 0; i < 6; i++)
    {
        pastHashes[hashIndex][i] = subHash[i];
    } */

    hashIndex++;
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

    printf("Digest is: ");
    for(int i = 0; i < md_len; i++)
        printf("%x ", md_value[i]);
    printf("\n");

    char* subHash[6];
    for(int i = 0; i < 6; i++)
    {
        subHash[i] = md_value[i];
    }
    /* for(int i = 0; i < 6; i++)
    {
        pastHashes[hashIndex][i] = md_value[i];
    } */

    printf("Sub hash: ");
    for(int i = 0; i < 6; i++)
        printf("%x ", subHash[i]);
    printf("\n");

    printf("size of subhash: %d\n", NELEMS(subHash));


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

    printf("Past Hashes: ");
    for(int i = 0; i < 1; i++) // i < NELEMS(pastHashes); i++)
    {
        for(int j = 0; j < 6; j++)
        {
            printf("%x ", pastHashes[i][j]);
        }
        printf("\n");
    }

    /* int md_len;

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