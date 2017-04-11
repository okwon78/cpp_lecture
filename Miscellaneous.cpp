#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;

//since c++14
//여러가지 attribute가 추가되었는데. 개인적으로 deprecated가 제일 맘에 듬.
//It emits a warning message.
[[deprecated("func is deprecated. use other function instead")]]
void func()
{
    cout << "deprecated" << endl;
}

//몇 가지 리터럴이 추가되었습니다. Standard user-defined literals
auto str = "hello world"s; // auto deduces string
auto dur = 60s;            // auto deduces chrono::seconds

static_assert(std::is_same<decltype(str), std::string>(), "str은 std::string 타입이 아니다.");
static_assert(std::is_same<decltype(dur), std::chrono::seconds>(), "str은 std::chrono::seconds 타입이 아니다.");

int main()
{
    func();
}