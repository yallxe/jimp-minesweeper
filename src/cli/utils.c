#include "utils.h"
#include <stdlib.h>

void clearterm()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}