#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <vector>

using namespace std;

//1. attribute
//since c++14
//여러가지 attribute가 추가되었는데. 개인적으로 deprecated가 제일 맘에 듬.
//It emits a warning message.
[[deprecated("func is deprecated. use other function instead")]]
void func1()
{
    cout << "deprecated" << endl;
}

//몇 가지 리터럴이 추가되었습니다. Standard user-defined literals
auto str = "hello world"s; // auto deduces string
auto dur = 60s;            // auto deduces chrono::seconds

static_assert(std::is_same<decltype(str), std::string>(), "str은 std::string 타입이 아니다.");
static_assert(std::is_same<decltype(dur), std::chrono::seconds>(), "str은 std::chrono::seconds 타입이 아니다.");

//2 push_back vs emplace_back
// - push_back will copy or move the object.
// - emplace_back constructs the object in place

struct Person {
    Person(std::string str ,int num) : _str(str), _num(num) { 
        cout << str << " constructor" << endl; 
    }
    
    Person(const Person& person) {
        _str = person._str;
        _num = person._num;
        cout << person._str << " copy constructor" << endl; 
    }
    
    Person(Person&& person) {
        _str.swap(person._str);
        swap(_num, person._num);

        cout << _str << " move constructor" << endl; 
    }
    std::string _str;
    int _num;
};

void func2()
{
    std::vector<Person> v;
    v.reserve(10);
    //Specifying Person in the push_back function is a unnecessary redundance
    //push_back will only call implicit constructors
    v.push_back(Person("push_back", 1));
    
    //emplace_back accepts any number of arguments and forwards.
    //emplace_back will call explicit constructors
    v.emplace_back("emplace_back", 2);
}

//3. Relaxed constexpr restrictions.
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

//4. Variadic arguements
int main()
{
    //func1();
    func2();
}


