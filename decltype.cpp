#include <iostream>
using namespace std;

template<typename L, typename R>
void multiply1(L lhs, R rhs)
{
    //C++ 이전에는 GCC에서 다음과 같이 쓸 수 있었다.
    //typeof는 gcc에서만 지원되는 변종 확장 키워드 같은 것이다.
    //typedef typeof(lhs * rhs) product_type; //gcc에서만 컴파일 된다.

    //c++11부터는 typeof를 표준화한 decltype이 생겼다.
    using product_type = decltype(lhs * rhs);    
    product_type result1 = lhs * rhs;
    cout << result1 << endl;
    
    //하지만 아래 처럼 사용하는 것이 정석이다.
    auto result2 = lhs * rhs;
    cout << result2 << endl;
}

//C++11에서 반환 타입을 추론 하기 위해서 decltype을 사용했다. trailing return type 이라고 부른다.
//C++14에서는 auto 정의만으로 되기도 하지만 반환 타입을 약간 변경할 경우 사용될 수 있다.
template<typename L,  typename R>
auto multiply2(L lhs, R rhs) -> decltype(lhs * rhs)
{
    return lhs * rhs;
}

struct Object {
    int _x = 0;
};

void func() 
{
    //분명히 집고 넘어가야 할 점은. auto와 decltype은 타입을 추론한다는 점에서 동일하지만 추론하는 방법에서 큰 차이가 있다.
    const int cx = 0;
    int a = 0;
    const int& crx = a;

    //x_type은 const int를 나타낸다.
    using cx_type = decltype(cx);

    cx_type x = 1;
    //x=2; Deos not compile: the type of x is 'const int'

    auto y = cx;
    y = 2; //the type of y is int

    //the type of crx_type is 'const int&' due to the type of crx.
    using crx_type = decltype(crx);
    crx_type z = a;
    a = 10;
    cout << z << " " << a << endl;

    //포인터 p가 가르키고 있는 Object는 const 타입이다.
    //그래서 p->_x는 값을 변경하지 못한다. 
    const Object* p = new Object();
    //그럼 m_x_type는 const int일까 int일까?
    //정답은 int 타입이다.
    using m_x_type = decltype(p->_x);
    
    //그래서 다음 구문은 컴피일된다.
    m_x_type m = 0;
    m = 1;

}

int main()
{
    //multiply1(2, 1);
    //cout << multiply2(1, 2) << endl;

    func();
}

