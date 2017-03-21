//
//  main.cpp
//  cpp_test
//
//  Created by Kwon on 24/02/2017.
//  Copyright © 2017 NHN. All rights reserved.
//

#include <cxxabi.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <vector>

using namespace std;

//std::string demangle(const char* mangled)
//{
//    int status;
//    std::unique_ptr<char[], void (*)(void*)> result(abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
//    return result.get() ? std::string(result.get()) : "error occurred";
//}

struct Object
{
    Object() { std::cout << "constructor" << std::endl; }
    
    ~Object() { std::cout << "distructor" << std::endl; }
    
    Object(const std::string& name) { _str = name; }
    
    //Copy
    Object(const Object& rhs) {
        _str = rhs._str;
        std::cout << "name: "<< _str << " copy constructor" << std::endl;
    }
    
    //Move
    Object(Object&& rhs) {
        _str.swap(rhs._str);
        //_str = std::move(rhs._str);
        std::cout << "name: "<< _str << " move constructor" << std::endl;
    }
    
    //Copy
    Object& operator=(const Object& rhs) {
        _str = rhs._str;
        std::cout << "name: "<< _str << " copy operator" << std::endl;
        return *this;
    }
    
    //Move
    Object& operator=(Object&& rhs) {
        _str.swap(rhs._str);
        std::cout << "name: "<< _str << " move operator" << std::endl;
        return *this;
    }
    
    void print() const { std::cout << _str << std::endl; }
    
    std::string _str;
};



void func(Object&& obj)
{
    Object obj2 = obj;
    
    obj2.print();
}


void process(const Object& obj)
{
    //파라미터에서 const를 제거하면 사실은 이 것도 된다.
    //하지만 이것은 타입정의에 의해 약속된 것을 어긴 것이다. 잘못된 API 설계다.
    //Object tmp = std::move(obj);
    obj.print();
}

void process(Object&& obj)
{
    Object tmp = std::move(obj);
    obj.print();
}

template<typename T>
void universal_reference(T&& t)
{
    process(std::forward<T>(t));
}

//이런식으로 사용하면 안된다.
//이미 컴퍼일러의 최적화를 방해 한다.
Object func()
{
    Object obj("local variable");
    return std::move(obj);
}

struct Factory
{
    Factory() : obj("factory") {}
    ~Factory() = default;
    Factory(const Factory &) = default;
    Factory(Factory &&) = default;
    
    Object get1() {
        return obj;
    }
    
    Object&& get2() {
        return std::move(obj);
    }
    
    //    Object&& get2()
    //    {
    //        return obj;
    //    }
    
    //    Object&& get3()
    //    {
    //        return Object("My");
    //    }
    
    Object obj;
};

void lecture1()
{
    if(false) {
        std::vector<Object> v;
        
        v.reserve(10);
        
        Object obj1("obj1"), obj2("obj2"), obj3("obj3"); //lvalue;
        
        //Object(); //rvalue;
        //Object obj; //lvalue;;
        
        v.push_back(obj1); //복사 연산자
        
        //1번
        v.push_back(std::move(obj2)); //이동 연산자
        //2번
        v.push_back(Object("AA")); // 이동 연산자
        
        v.push_back(std::move(obj3));
    }
    
//    void func(Object&& obj)
//    {
//        Object obj2 = obj;
//        
//        obj2.print();
//    }
    
    if(false) {
        //Argument and Return type
        Object obj("obj");
        //func(obj); //컴파일 오류. r-value만 받겠다는 뜻
        //이럴때는 요놈 -> template<typename T> void foo(T&&);
        //obj->print();
        
        func(std::move(obj));
        
        func(Object("obj"));
        
        
        //obj
    }
    
    if(false) {
        //swap을 많이 사용하자. map도 된다.
        std::vector<Object> v1, v2;
        v1.reserve(10);
        Object obj1("obj1"), obj2("obj2"), obj3("obj3");
        
        v1.push_back(std::move(obj1));
        v1.push_back(std::move(obj2));
        //v1.push_back(std::move(obj3));
        
        //v2.push_back(std::move(obj3));
        std::cout << "v1: " << v1.size() << "------------" << "v2: " << v2.size() << std::endl;
        
        v2 = v1;
        //v2 = std::move(v1);
        v1.swap(v2);
        
        std::cout << "v1: " << v1.size() << "------------" << "v2: " << v2.size() << std::endl;
    }
    
    


    
    if(true) {
        //반환 값.
        Factory factory;
        
        {
            Object o1 = factory.get1();
            //복사 생성자가 호출되고 비효율 적이므로 실제로 잘 사용하지 않는다. 차라리 smart pointer를 사용하는 것이....

            Object&& o2 = factory.get1(); //Object get1();
            //복사 생성자가 호출되므로 의해서 이렇게 사용할 이유가 없다.
            o2.print();
        }
        {
            Object o1 = factory.get2();   //정상 동작 factory 같은 개념으로 쓸 수 있음. 아주 나이스 함.
            Object&& o2 = factory.get2(); // Object&& get2();
            //o2는 포인터 같은 레퍼런스이다. private 멤버를 접근하게 되므로 이렇게 사용하지는 말자.
        }
        
        factory.obj.print();
    }

//////////////////////////////////////////////////////
//    template<typename T>
//    void universal_reference(T&& t)
//    {
//        process(std::forward<T>(t));
//    }
    
    if(false) {
        Factory factory;
        
        Object obj("universal_reference");
        universal_reference(std::move(obj));
        //universal_reference(obj);
        //universal_reference(factory.get2());
    }
//////////////////////////////////////////////////////
    
    if(false) {
        Object obj = func();
        obj.print();
    }
}

int main() {
    lecture1();
}

