//
//  main.cpp
//  cpp_test
//
//  Created by Kwon on 24/02/2017.
//  Copyright © 2017 NHN. All rights reserved.
//

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

struct Object
{
    //Object() { cout << _str << " constructor" << endl; }
    
    ~Object() { cout << _str <<" distructor" << endl; }
    
    Object(const string& name) { 
        _str = name; 
    }
    
    //Copy
    Object(const Object& rhs) {
        _str = rhs._str;
        cout << "name: "<< _str << " copy constructor" << endl;
    }
    
    //Move
    Object(Object&& rhs) {
        _str.swap(rhs._str);
        //_str = move(rhs._str);
        cout << "name: "<< _str << " move constructor" << endl;
    }
    
    //Copy
    Object& operator=(const Object& rhs) {
        _str = rhs._str;
        cout << "name: "<< _str << " copy operator" << endl;
        return *this;
    }
    
    //Move
    Object& operator=(Object&& rhs) {
        _str.swap(rhs._str);
        cout << "name: "<< _str << " move operator" << endl;
        return *this;
    }
    
    void print() const { cout << _str << endl; }

private:
    string _str;
};



void func(Object& obj) {
    //파라미터에서 const를 제거하면 사실은 이 것도 된다.
    //하지만 이것은 타입정의에 의해 약속된 것을 어긴 것이다. 잘못된 API 설계다.
    Object tmp = move(obj);
}

void func(Object&& obj){
    Object tmp = obj;
    tmp.print();
}


void process(const Object& obj) {
    obj.print();
}

void process(Object&& obj) {
    Object tmp = move(obj);
    obj.print();
}

template<typename T>
void universal_reference(T&& t) {
    process(forward<T>(t));
}

//이런식으로 사용하면 안된다.
//컴퍼일러의 최적화를 방해 한다.
//https://en.wikipedia.org/wiki/Return_value_optimization
Object func()
{
    Object obj("local variable");
    return move(obj);
}

struct Factory
{
    Factory() : obj("factory") {}
    ~Factory() = default;
    Factory(const Factory &) = default;
    Factory(Factory &&) = default;
    
    Object get1() { return obj; }
    
    Object&& get2() { return move(obj); }
    
    //    Object&& get2() {
    //        return obj;
    //    }
    
    //    Object&& get3()
    //    {
    //        return Object("My");
    //    }
    
    Object obj;
};

int main() {

    if(false) { //lvalue vs rvalue
        vector<Object> v;
        
        v.reserve(10);
        
        Object obj1("obj1"), obj2("obj2"); //obj1, obj2는 lvalue 다.
        
        //Object(); //rvalue;
        //Object obj; //lvalue;;
        
        v.push_back(obj1); //복사 연산자 호출됨
        v.push_back(move(obj2)); //이동 연산자 호출됨. lvalue를 rvalue로 형 변환 하였기 때문.
        v.push_back(Object("obj3")); // obj3은 rvalue이기 때문에 이동 연산자 호출됨.
    }
    

//    void func(Object&& obj){
//        Object obj2 = obj;
//        obj2.print();
//    }
    
    if(false) { //Rvalue reference
        //Argument and Return type
        Object obj("obj");
        
        //func(obj); //컴파일 오류. r-value만 받겠다는 뜻. 하지만 obj는 현재 lvalue임.
        func(move(obj)); //OK
        func(Object("obj")); //OK
    }
    
    if(true) { //move sementic with stl
        Object obj1("obj1"), obj2("obj2"), obj3("obj3");
        
        vector<Object> v1; // {move(obj1), move(obj2), move(obj3)};
        
        v1.reserve(3);
        v1.push_back(move(obj1)); v1.push_back(move(obj2)); v1.push_back(move(obj3));

        vector<Object> v2;
        cout << "[v1]: " << v1.size() << " " << "[v2]: " << v2.size() << endl;

        //v2 = v1;
        v2 = move(v1);
        //v1.swap(v2);
        
        cout << "[v1]: " << v1.size() << " " << "[v2]: " << v2.size() << endl;
    }

    // Object get1() { return obj; }
    // Object&& get2() { return move(obj); }

    if(false) {//반환 값.
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
    }

//    template<typename T>
//    void universal_reference(T&& t)
//    {
//        process(forward<T>(t));
//    }
    
    if(false) { //universal reference
        Factory factory;
        
        Object obj("universal_reference");
        universal_reference(move(obj));
        //universal_reference(obj);
        //universal_reference(factory.get2());
    }
    
    if(false) { //RVO
        Object obj = func();
        obj.print();
    }
}
