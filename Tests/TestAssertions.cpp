#include <SD/Assertions.h>

int main(int argc, char* argv[])
{
    if (false) {
        ASSERT_NOT_REACHED();
    }

    ASSERT(true);

    return 0;
}
