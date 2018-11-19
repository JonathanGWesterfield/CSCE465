#include <stdio.h>
#include <openssl/evp.h>

EVP_MD_CTX *mdctx;
const EVP_MD *md;
char mess1[] = "Test Message\n";
char mess2[] = "Hello World\n";
unsigned char md_value[EVP_MAX_MD_SIZE];

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
        printf("%02x", md_value[i]);
    printf("\n");
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
    printf("\n");

    /* Call this once before exit. */
    EVP_cleanup();
    exit(0);
}