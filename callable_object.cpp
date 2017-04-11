#include <vector>
#include <iostream>

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//1. 함수 포인터

void global_print()
{ 
    cout << "Global Print" << endl; 
}

struct Object
{
    void member_print(){ cout << "Member Print" << endl; }
};

void chapter1()
{
    cout << endl << "1. 함수 포인터" << endl;
    using PFunc = void(*)();

    PFunc pFunc = &global_print;
    pFunc();

    using MFunc = void(Object::*)();
    MFunc mFunc = &Object::member_print;

    Object o;
    (o.*mFunc)();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2. Functor 펑터 

struct Functor
{
    void operator()(const string& str)
    {
        cout << str << endl;
    }
};

void chapter2_1()
{
    cout << endl << "2.1 Functor 펑터 " << endl;
    Functor functor;
    functor("Hello Functor");   
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Functor1
{
    Functor1(const string& str) : _str(str) 
    {
    }

    void operator()()
    {
        cout << _str << endl;
    }
    
    string _str;
};

void chapter2_2()
{
    cout << endl << "2.2 Functor 펑터 " << endl;
    Functor1 functor("Second Functor");
    functor();   
}

void chapter2_3()
{
    cout << endl << "2.3 Functor 펑터 " << endl;

    class Sum
    {
        int _total;
    public:
        Sum() : _total(0) {}
        void operator()(int elem)
        {
            _total += elem;
        }
 
        int total()
        {
            return _total;
        }
    };
 
    vector<int> v = { 1, 2, 3};
    
    Sum sum = for_each(begin(v), end(v), Sum());
    cout << sum.total() << endl;

    // for(auto Iter = v.begin();Iter != v.end(); Iter++)
    // {

    // }

    int total = 0;

    for(auto&& elem : v) {
        total += elem;
    }
    
    cout << total << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//3. std::bind

void func(const string& str){
    cout << str << endl;
}

void mul(int& lhs, int rhs);

void chapter3() {

    cout << endl << "3. Bind " << endl;
     auto f1 = bind(func, "hello bind");
     f1();   

     auto f2 = bind(&func, placeholders::_1);
     f2("hello bind");

     vector<int> v = { 1, 2, 3};

     for_each(begin(v), end(v), bind(&mul, placeholders::_1, 2));

     for(auto elem : v) 
     { 
         cout << elem << endl; 
     }
}

void mul(int& lhs, int rhs){
    lhs *= rhs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//4. Lambda expression
void chapter4_1() {
    cout << endl << "4.1 Lambda" << endl;
    //[](){}
    
    auto f1 = [](int x)
    { 
        cout << x << endl;
    };
    
    vector<int> v = { 1, 2, 3};
    
    int sum = 0;
    //참조 형태로 외부 변수를 사용한다.
    for_each(begin(v), end(v), [&sum](int elem){
        sum += elem;
    });

    cout << sum << endl;
    int num = 2;
    //값 복사 방식으로 외부 변수를 사용한다.
    for_each(begin(v), end(v), [num](int& elem){
        elem += num;
    });
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void chapter4_2() {
    cout << endl << "4.2 Lambda" << endl;

    vector<int> v = { 1, 2, 3};

    int sum = 0;
    //sum 은 값 복사에 의해 상수이다. 
    for_each(begin(v), end(v), [=](int elem) mutable {
        sum += elem;    
    });

    cout <<"[=] " << sum << endl;

    for_each(begin(v), end(v), [&](int elem){
        sum += elem;
    });

    cout <<"[&] " << sum << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Callable {
    
    template<typename T> 
    void operator()(T x) {
        cout << x << endl;
    }
};

void chapter4_3()
{
    cout << endl << "4.3 Lambda" << endl;
    {
        //C++14 Generic lambdas
        auto f = [](auto x){
            cout << x << endl;
        };

        f(1);
        f("what's up lambda'");
    }
    
    {
        auto f =  Callable();

        f(2);
        f("what's up functor'");
    }
}

void chapter4_4()
{
    cout << endl << "4.4 Lambda" << endl;
    vector<int> v = { 1, 2, 3};

    int num = 5;
    for_each(begin(v), end(v), [num](auto&& elem){
        elem += num;
    });

    for_each(begin(v), end(v), [](auto elem){
        cout << elem << endl;
    });
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void chapter4_5()
{
    std::function<void()> func;
    
    {
        Object obj;

        func = [&](){
            //obj를 참조
            obj.member_print();
        };
    }
    
    //obj는 파괴되었기 때문에 존재하지 않는다.
    func();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//4.6 람다 리턴 타입
void chapter4_6()
{
    //다음과 같이 반환 타입을 지정할 수 있다.
    auto lambda1 = []() -> std::string {
        std::string str("with returne type");
        return str;
    };

    string str1 = lambda1();

    cout << str1 << endl;

    //하지만 타입 deduction이 수행되므로 선언하지 않아도 된다.
    auto lambda2 = []() {
        std::string str("without returne type");
        return str;
    };
    
    string str2 = lambda2();

    cout << str2 << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//5. std::function
void call(std::function<void()> callable)
{
    callable();
}

void chapter5()
{
    cout << endl << "5.function" << endl;

    call(&global_print);
    
    Object o;
    call(bind(&Object::member_print, &o));
    
    call([](){ cout << "Lambda print" << endl; });
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    std::cout << "## Callable Object ##" << std::endl;
    
    //chapter1();
    //chapter2_1();
    //chapter2_2();
    //chapter2_3();
    //chapter3();
    //chapter4_1();
    //chapter4_2();
    //chapter4_3();
    //chapter4_4();
    //chapter4_5();
    chapter4_6();
    //chapter5();

    return 0;
}