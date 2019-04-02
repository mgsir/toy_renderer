#include <iostream>

struct test {
    test() : x(10), y(10) {
        std::cout<< "调用构造函数" << '\n'; 
    }
    test(const test& t) : x(t.x), y(t.y) {
        std::cout << "调用拷贝构造" << '\n';
    }
    int x, y; 
};

void fuck(test t) {

    std::cout << t.x << t.y << '\n';
}

int main() {

    test t;
    t.x = 1;
    t.y = 2;

    fuck(t);

    return 0;
}