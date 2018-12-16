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
 * Iterates through the array of stored hashes and compares each element
 * to the subHash that is passed in.
 * @param subHash
 * @return
 */
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
            printf("Found after %d tries!\n", (hashIndex + 1));
            return true;
        }
    }

    return false;
}

/**
 * Generates a hash based on a randomized string passed in and a known, default string.
 * Then compares it to previous hashes that were generated from a different set of
 * strings and stores the hash if it didn't match.
 * @param mess1
 * @param mess2
 * @return
 */
bool knownMesDigest(char mess1[])
{
    // Mess 2 length = 13
    char mess2[] = "Known Message";
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

    // 24 bits is the first 6 characters

    char* subHash[6];
    for(int i = 0; i < 6; i++)
        subHash[i] = md_value[i];

    printf("Sub hash: ");
    for(int i = 0; i < 6; i++)
        printf("%x ", subHash[i]);
    printf("\n");

    if(compareHash(subHash) == true)
    {
        printf("\nMessages that matched: \n");
        printf("%s\n", mess1);
        printf("%s\n", mess2);
        return true;
    }


    // store this new hash in our hash array
    strncpy(pastHashes[hashIndex], md_value, 6);

    printf("Stored Hash: ");
    for (int i = 0; i < 6; i++)
        printf("%x ", pastHashes[hashIndex][i]);
    printf("\n");

    // Increment our hash array
    hashIndex++;

    printf("Not found yet\n\n");
    return false;
}

/**
 * Generates a hash based on the randomized strings passed in. Then compares it to
 * previous hashes that were generated from a different set of random strings and stores
 * the hash if it didn't match.
 * @param mess1
 * @param mess2
 * @return
 */
bool randMesDigest(char mess1[], char mess2[])
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

    // 24 bits is the first 6 characters

    char* subHash[6];
    for(int i = 0; i < 6; i++)
        subHash[i] = md_value[i];

    printf("Sub hash: ");
    for(int i = 0; i < 6; i++)
        printf("%x ", subHash[i]);
    printf("\n");

    if(compareHash(subHash) == true)
    {
        printf("\nMessages that matched: \n");
        printf("%s\n", mess1);
        printf("%s\n", mess2);
        return true;
    }


    // store this new hash in our hash array
    strncpy(pastHashes[hashIndex], md_value, 6);

    printf("Stored Hash: ");
    for (int i = 0; i < 6; i++)
        printf("%x ", pastHashes[hashIndex][i]);
    printf("\n");

    // Increment our hash array
    hashIndex++;

    printf("Not found yet\n\n");
    return false;
}

/** Generates a pair of randomized strings and then calls the mesDigest() function
 * to see if it can find a pair of randomized strings that generates the same
 * hash as a different pair.X
 * @param argc
 * @param argv
 * @return
 */
main(int argc, char *argv[])
{
    OpenSSL_add_all_digests();

    if(!argv[1] || !argv[2]) {
        printf("Usage: ./crypt digestname messageLength\n");
        exit(1);
    }

    md = EVP_get_digestbyname(argv[1]);
    int messLength = atoi(argv[2]);

    if(!md) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
    }

    bool hashMatched = false;
    while(!hashMatched)
    {
        if (messLength == 0)
        {
            // length is set to 13 because that is the length of the known message
            hashMatched = knownMesDigest(randstring(22));
            printf("Message Length: 13\n");
        }
        else
        {
            hashMatched = randMesDigest(randstring(messLength), randstring(messLength));
            printf("Message Length: %d\n", messLength);
        }
    }


    /* Call this once before exit. */
    EVP_cleanup();
    exit(0);
}