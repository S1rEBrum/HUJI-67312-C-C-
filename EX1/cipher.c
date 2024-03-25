#include "cipher.h"

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.
#define ALPHABET 26
#define LOWER_A 'a'
#define UPPER_A 'A'

// See full documentation in header file
void encode (char s[], int k)
{
  // your code goes here
  for (int i = 0; s[i] != '\0'; ++i)
  {
    if ('a' <= s[i] && s[i] <= 'z')
    {  // lowercase
      s[i] = LOWER_A
             + (s[i] - LOWER_A + (k >= 0 ? k : ALPHABET - (-k) % ALPHABET))
               % ALPHABET;
    }
    if ('A' <= s[i] && s[i] <= 'Z')
    { // uppercase
      s[i] = UPPER_A
             + (s[i] - UPPER_A + (k >= 0 ? k : ALPHABET - (-k) % ALPHABET))
               % ALPHABET;
    }
  }
}

// See full documentation in header file
void decode (char s[], int k)
{
  encode(s, -k);
}
