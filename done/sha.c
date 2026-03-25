/**
 * @file sha.c
 * @brief A simple example of SSL SHA.
 *
 * @author J.-C Chappelier and Mia Primorac
 * @date 16 Feb 2020
 */

#include <stdio.h>
#include <string.h> // for strlen()
#include <openssl/sha.h> // for SHA256_DIGEST_LENGTH and SHA256()

#define MAX_INPUT_SIZE 256

// ======================================================================
/**
 * @brief Human-readable SHA.
 *
 * @param SHA the SHA to be displayed
 */
void
print_SHA(const unsigned char* SHA)
{
    if (SHA == NULL) { // Safety protection
        return;
    }
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        printf("%02x", SHA[i]);
    }
}

// ======================================================================
int main(void)
{
    char input[MAX_INPUT_SIZE];
    fputs("Type a sentence: ", stdout);
    fgets(input, MAX_INPUT_SIZE, stdin);

    unsigned char sha[SHA256_DIGEST_LENGTH];

    puts("The SHA256 code of");
    printf("\"%s\"\n", input);
    puts("is: ");
    print_SHA(sha);
    putchar('\n');

    return 0;
}
