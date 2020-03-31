#include <hello.h>
#include <iostream>
#include <cstdlib>

int printHello()
{
    return std::cout << "hello world!" << std::endl ? EXIT_SUCCESS : EXIT_FAILURE;
}
