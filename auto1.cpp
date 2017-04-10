#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    //auto는 type deduction입니다.
    //C#과 python의 var와는 다릅니다.
    int x = 0;    
    
    //BASIC RULES (기본적으로 복사 생성 방식이 기본이된다.)
    //1. 오른쪽이 참조형을 경우 참조는 무시된다.
    //2. const와 volatile 등이 있을 경우에도 해당 키워드는 무시된다.

    const int& const_ref = x;
    auto something = const_ref;    

    x = 10;

    cout << "something: " << something << endl;
    cout << "const_ref: " << const_ref << endl;
    cout << "x: " << x << endl;
    
    //Additional Rule 1: 복사 생성이 안되는 형태에 경우
    const int c = 0;
    //rc는 참조형이기 때문에 앞의 룰을 따를 수 없다.
    //자연스럽게 추론이 되기 위해서는 auto는 const int가 된다. 즉, "const int& rc"가 된다.
    auto& rc = c; 

    //다음과 같이 rc가 const이기 때문에 새로운 값을 할당할 수 없다.
    //rc = 10; 

    //Additional Rule 2: 포인터의 경우
    int value = 0;
    const int* p1 = &value;
    //p1의 타입은 const int이기 때문에 가르키고 있는 값은 const이다. 물론 value 자체는 상수가 아니지만 ..
    //p2가 자연스럽게 성립되기 위해서는 const int* 형이되어야만 자연스럽다.
    auto p2 = p1; 

    //결국 다음 구문은 성립할 수 없게 된다.
    //*p = 10;

    //Additional Rule 3: C++11부터 적용된 collapsing rule
    //It is somehow similar with rule which is called universla reference or forwarding reference

    int y = 0;
    //변수 y는 lvaue이 기 때문에 auto는 int& 형태로 참조 되고 int& && 아기 때문에 int&가 된다. 
    auto&& lvalue = y;
    //정수 10은 rvalue이기 때문에 auto는 int&&로 추론되고 int&& && 이기 때문에 int&&가 된다.
    auto&& rvalue = 10;

    //쉽게, 우변이 lvalue이면 참조 한 개, rvalue이면 참조 두 개로 생각해도 된다.

    //----------------------

    //auto는 여러 가지로 엄청 좋다. 쓸수 있으면 최대로 써라. 누가 한말? 스캇 마이어...
    
    //실제로 auto는 다음에 설명할 decltype과 관련이 있습니다.
    //아래 구문은 동일합니다.
    auto mul1 = x * y;
    decltype(x * y) mul2 = x * y;
}