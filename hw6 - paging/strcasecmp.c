#include <stdio.h>
#include <string.h>

int main ()
{
    char * a = "BINGO";
    char * b = "bingo";

    /* Compare using strcmp. */

    printf ("%s and %s are ", a, b);
    if (strcmp (a, b) == 0) {
        printf ("the same");
    }
    else {
        printf ("different");
    }
    printf (" according to 'strcmp';\nthey are ");
    if (strcasecmp (a, b) == 0) {
        printf ("the same");
    }
    else {
        printf ("different");
    }
    printf (" according to 'strcasecmp'.\n");
    return 0;
}