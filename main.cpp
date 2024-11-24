#include "lru.hpp"
#include <iostream>
#include <sstream>
#include <vector>

//   g++ -std=c++2a -O2 -fsanitize=address -Wall -Werror -Wextra main.cpp -o main
// clang-format-10 --Werror --style=file -i *.cpp *.hpp
// clang-format-10 --dry-run --Werror --style=file *.cpp *.hpp

int main()
{
    size_t N, Size;
    std::cin >> N >> Size;

    LRU_Cache<std::string, std::vector<float>> cache(N, Size);

    std::cin.ignore();

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string query;
        std::vector<float> embedding;

        if (!(iss >> query)) {
            std::cerr << "!STORERR!" << std::endl;
            continue;
        }

        float num;
        while (iss >> num) {
            embedding.push_back(num);
        }

        if (!embedding.empty() && embedding.size() != 64) {
            std::cerr << "!STORERR!" << std::endl;
            continue;
        }

        // Если эмбеддинг пустой, это pull-запрос
        if (embedding.empty()) {
            cache.pull(query);
        } else {
            cache.store(query, embedding);
        }
    }

    return 0;
}
