# LRU-cache
Собрать проект с помощью команды
 g++ -std=c++2a -O2 -fsanitize=address -Wall -Werror -Wextra main.cpp -o main

Прогнать тесты:
g++ -fprofile-arcs -ftest-coverage     -g -O0 -fsanitize=address -std=c++2a     -Wall -Werror -Wextra test_lru.cpp -o test_lru  
./test_lru  
Посмотреть покрытие:  
gcov test_lru.gcno   
или   
gcov test_lru.gcda  
Покрытие lru.hpp составит примерно 92 процента    
