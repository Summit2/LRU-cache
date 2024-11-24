#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//g++ -fprofile-arcs -ftest-coverage     -g -O0 -fsanitize=address -std=c++2a     -Wall -Werror -Wextra test_lru.cpp -o test_lru
//./test_lru
void testInitialization()
{
    LRU_Cache<std::string, std::vector<int>> cache(3, 50);
    assert(cache.size() == 0);
}

void testStoreAndPull()
{
    LRU_Cache<std::string, std::vector<int>> cache(3, 50);

    // Сохранение элемента
    cache.store("key1", { 1, 2, 3 });
    assert(cache.size() == 1);

    // Проверка извлечения существующего элемента
    std::ostringstream capturedOutput;
    std::streambuf *originalCout = std::cout.rdbuf();
    std::cout.rdbuf(capturedOutput.rdbuf());
    cache.pull("key1");
    std::cout.rdbuf(originalCout);
    assert(capturedOutput.str() == "1 2 3 \n");
}

void testReplaceAndEviction()
{
    LRU_Cache<std::string, std::vector<int>> cache(2, 50);

    // Добавление элементов
    cache.store("key1", { 1, 2 });
    cache.store("key2", { 3, 4 });
    assert(cache.size() == 2);

    // Добавление третьего элемента вызывает удаление наименее используемого
    cache.store("key3", { 5, 6 });
    assert(cache.size() == 2);

    // Проверка, что key1 был удален
    std::ostringstream capturedOutput;
    std::streambuf *originalCout = std::cout.rdbuf();
    std::cout.rdbuf(capturedOutput.rdbuf());
    cache.pull("key1");
    std::cout.rdbuf(originalCout);
    assert(capturedOutput.str() == "!NOEMBED!\n");
}

void testStoreOversizedItem()
{
    LRU_Cache<std::string, std::vector<int>> cache(2, 20);

    // Элемент слишком большой для кэша
    std::ostringstream capturedOutput;
    std::streambuf *originalCout = std::cout.rdbuf();
    std::cout.rdbuf(capturedOutput.rdbuf());
    cache.store("oversized", { 1, 2, 3, 4, 5, 6 });
    std::cout.rdbuf(originalCout);
    assert(capturedOutput.str() == "!STORERR!\n");
    assert(cache.size() == 0);
}

void testClear()
{
    LRU_Cache<std::string, std::vector<int>> cache(3, 50);

    cache.store("key1", { 1 });
    cache.store("key2", { 2 });
    assert(cache.size() == 2);

    cache.clear();
    assert(cache.size() == 0);

    std::ostringstream capturedOutput;
    std::streambuf *originalCout = std::cout.rdbuf();
    std::cout.rdbuf(capturedOutput.rdbuf());
    cache.pull("key1");
    std::cout.rdbuf(originalCout);
    assert(capturedOutput.str() == "!NOEMBED!\n");
}

int main()
{
    testInitialization();
    testStoreAndPull();
    testReplaceAndEviction();
    testStoreOversizedItem();
    testClear();

    std::cout << "All tests passed";
    return 0;
}
