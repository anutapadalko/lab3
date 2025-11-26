// build: g++ -std=c++20 -O2 -pthread scheme7.cpp -o scheme7
// run:   ./scheme7   (або scheme7.exe > results.txt)

#include <chrono>
#include <iomanip>
#include <iostream>
#include <latch>
#include <string>
#include <syncstream>
#include <thread>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace std::chrono;

void f(char x, int i) {
    osyncstream(cout) << "З набору " << x << " виконано дію " << i << ".\n";
}

void run_set(char x, int count) {
    for (int i = 1; i <= count; ++i) f(x, i);
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
    osyncstream(cout) << "Обчислення розпочато.\n";

    const int A = 7, B = 8, C = 6, D = 7, E = 6, F = 8, G = 6, H = 4, I = 5, J = 5;

    latch after_a(1);
    latch after_b_c(2);
    latch after_d_e(2);
    latch after_f_g(2);

    jthread t1([&] {
        run_set('a', A);
        after_a.count_down();
        
        run_set('d', D);
        after_d_e.count_down();  

        after_d_e.wait();
        run_set('h', H);
    });

    jthread t2([&] {
        run_set('b', B);
        after_b_c.count_down(); 

        after_b_c.wait();
        run_set('e', E);
        after_d_e.count_down();
        
        after_d_e.wait();
        run_set('i', I);
    });

    jthread t3([&] {
        run_set('c', C);
        after_b_c.count_down();  

        after_b_c.wait();
        run_set('g', G);
        after_f_g.count_down();
        
        after_f_g.wait();
        run_set('j', J);
    });

    jthread t4([&] {
        after_a.wait();
        run_set('f', F);
        after_f_g.count_down();
    });

    osyncstream(cout) << "Обчислення завершено.\n";
    return 0;
}
