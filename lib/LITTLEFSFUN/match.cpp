// A C program to match wild card characters
#include <stdbool.h>
#include <stdio.h>
 
// The main function that checks if two given strings
// match. The first string may contain wildcard characters
// ref:https://www.geeksforgeeks.org/wildcard-character-matching/amp/

bool match(char* first, char* second)
{

    // If we reach at the end of both strings, we are done

    if (*first == '\0' && *second == '\0')

        return true;
 

    // Make sure to eliminate consecutive '*'

    if (*first == '*') {

        while (*(first + 1) == '*')

            first++;

    }
 

    // Make sure that the characters after '*' are present

    // in second string. This function assumes that the

    // first string will not contain two consecutive '*'

    if (*first == '*' && *(first + 1) != '\0'

        && *second == '\0')

        return false;
 

    // If the first string contains '?', or current

    // characters of both strings match

    if (*first == '?' || *first == *second)

        return match(first + 1, second + 1);
 

    // If there is *, then there are two possibilities

    // a) We consider current character of second string

    // b) We ignore current character of second string.

    if (*first == '*')

        return match(first + 1, second)

               || match(first, second + 1);

    return false;
}

#ifdef _MATCH_DEBUG_
// A function to run test cases

void test(char* first, char* second)
{

    match(first, second) ? puts("Yes") : puts("No");
}
 
// Driver program to test above functions
int main()
{

    test("g*ks", "geeks"); // Yes

    test("ge?ks*", "geeksforgeeks"); // Yes

    test("g*k", "gee"); // No because 'k' is not in second

    test("*pqrs",

         "pqrst"); // No because 't' is not in first

    test("abc*bcd", "abcdhghgbcd"); // Yes

    test("abc*c?d", "abcd"); // No because second must have

                             // 2 instances of 'c'

    test("*c*d", "abcd"); // Yes

    test("*?c*d", "abcd"); // Yes

    test("geeks**", "geeks"); // Yes
 

    return 0;
}
#endif