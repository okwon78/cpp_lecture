#include <type_traits>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//C++14은 2014 12월 발표되어습니다.
//clang++ 3.5와 g++ 5.0에서 C++14 풀스팩을 지원하고 있습니다.

//1. Function return type deduction
auto func1() {
    return 0;
}

static_assert(std::is_same<decltype(func1()), int>(), "func1 함수의 반환 타입이 int가 아닙니다.");

auto func2() {
    string test("hello C++14");
    return test;
}

static_assert(std::is_same<decltype(func2()), std::string>(), "func2 함수의 반환 타입이 std::string이 아닙니다.");

const auto& func3() {
    static string test("hello C++14");
    return test;
}

static_assert(std::is_same<decltype(func3()), const std::string&>(), "func3 함수의 반환 타입이 const std::string&이 아닙니다.");


template<typename T>
struct SomeContainerWrapper
{
    using Vector = vector<const T*>;
    Vector v;

    typename Vector::iterator inCPP03() const {
        return begin(v);
    }

    auto inCPP11() const -> decltype
    {

    }
};

int main()
{
}