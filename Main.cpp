#include "stdafx.h"
#include "MemoryDispatcher.cpp"

int main() {
    MemoryDispatcher::MemoryDispatcher memoryDispatcher = *(new MemoryDispatcher::MemoryDispatcher());
    memoryDispatcher.run();
    system("pause");
    return 0;
}