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
//auto == int
static_assert(std::is_same<decltype(func1()), int>(), "func1 함수의 반환 타입이 int가 아닙니다.");

auto func2() {
    string test("hello C++14");
    return test;
}

//auto == std::string
static_assert(std::is_same<decltype(func2()), std::string>(), "func2 함수의 반환 타입이 std::string이 아닙니다.");

const auto& func3() {
    static string test("hello C++14");
    return test;
}

//auto  == std::string
static_assert(std::is_same<decltype(func3()), const std::string&>(), "func3 함수의 반환 타입이 const std::string&이 아닙니다.");

auto& func4() {
    static const string test("hello C++14");
    return test;
}

//auto  == const std::string
static_assert(std::is_same<decltype(func4()), const std::string&>(), "func4 함수의 반환 타입이 const std::string&이 아닙니다.");

template<typename T>
struct SomeContainerWrapper
{
    vector<T> v;

    //C++03 때는 이렇게 사용해야 했다.
    typename vector<T>::iterator inCPP03() {
        return begin(v);
    }

    //C++11 뒤에 decltype을 사용해서 반환형을 정의 할 수 있음.
    auto inCPP11() -> decltype(std::begin(v)) {
        return begin(v);
    }

    auto beginCPP14() {
        return std::begin(v);
    } 
};

//The following is very redundant
template<typename T1, typename T2, typename T3>
auto FuncCPP11(T1 t1, T2 t2, T3 t3) -> decltype((t1 * t2) - (t2 * t3)) {
    return (t1 * t2) - (t2 * t3);
}

//In C++14, the compiler automatically deduces the correct return type.
template<typename T1, typename T2, typename T3>
auto FuncCPP11(T1 t1, T2 t2, T3 t3) {
    return (t1 * t2) - (t2 * t3);
}

//2. decltype(auto)
// auto는 참조형이 아닌 타입을 나타낸다.
// auto&&는 참조형 타입을 나타낸다.
// decltype(auto)는 상황에 맞게 참조형 또는 밸류형태의 타입을 나타낸다.

auto func5() {
    static const std::string str("Hello");
    auto& result(str); //강제로 참조형으로 변경하였다. auto == const std::string
    return result;
}

//auto == std::string 
//레퍼런스 타입을 반환했지만, 여전히 밸류 타입의 반환형을 가진다. 
static_assert(std::is_same<decltype(func5()), std::string>(), "func5 함수의 반환 타입이 std::string이 아닙니다.");

decltype(auto) func6() {
    static const std::string str("Hello");
    auto& result(str); //auto == const std::string
    return result;
}

//auto == std::string& 
//decltype(auto)는 적당히게 참조형으로 반환형을 지정하였다.
static_assert(std::is_same<decltype(func6()), const std::string&>(), "func6 함수의 반환 타입이 std::string&이 아닙니다.");

decltype(auto) func7(std::string&& str) {
    return std::move(str);
}
//decltype(auto)는 적당히게 rvalue형으로 반환형을 지정하였다.
static_assert(std::is_same<decltype(func7("test")), std::string&&>(), "func7 함수의 반환 타입이 std::string&&이 아닙니다.");

//3. Generic lambda
auto lambda1 = [](auto x) { return x * 2;};
static_assert(std::is_same<decltype(lambda1(1)), int>(), "lambda1의 반환 타입이 int 아닙니다.");
static_assert(std::is_same<decltype(lambda1(1.f)), float>(), "lambda1의 반환 타입이 float 아닙니다.");
static_assert(std::is_same<decltype(lambda1(1.)), double>(), "lambda1의 반환 타입이 double 아닙니다.");

//The following is almost the same of the lambda above.
template<typename T>
struct Functor
{
    T operator()(T x) 
    {
        return x * 2;
    }
};

//Appendix. Relaxed constexpr restrictions.
//이건 auto랑은 상관 없지만 type deduction하고 관련이 있는 놈이라 여기서 잠깐 다룹니다.
//컴파일 시간에 실행됩니다. 즉 아래 함수는 그냥 int 숫자 하나로 치환됩니다.
constexpr int compute_something_in_compile_time(int x)
{
    int result(x);

    for(int i(0); i <10; i++)
        result += i;

    if(result > 5) result += 10;

    return result;
}

//템플릿의 정의는 컴파일 시간에 지정됩니다.
template<int V> struct Test {};

//이게 컴파일이 됩니다. 놀랍지 않으신가요?
//이걸 옛날에는 엄청 복잡하게 짰어야 했거든요.
Test<compute_something_in_compile_time(5)> instance;

int main()
{

}