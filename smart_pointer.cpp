#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <exception>

//This namespace would be in one of the header files for cpp files.
namespace toast {
    using std::cout;
    using std::endl;
    using std::shared_ptr;
    using std::make_shared;
    using std::enable_shared_from_this;
    using std::default_delete;
    using std::vector;
    using std::weak_ptr;
    using std::bad_weak_ptr;
    using std::dynamic_pointer_cast;
    using std::static_pointer_cast;
} //namespace toast {

namespace toast {

//0. How do we declare class for shared_ptr
class Object : public enable_shared_from_this<Object> {
public:
    Object(const std::string& name) : _name(name) { cout << "          Acquires" << endl; }
    ~Object() { cout << "          Release" << endl; }

    //클래스를 reference형태로 참조해서 사용하고 싶은 경우에는 복사 생성자와 복사 연산자를 막아주는 것이 좋다.
    Object(const Object& other) = delete; 
    Object& operator=(const Object& other) = delete;

    std::shared_ptr<Object> getPointer()
    {
        return shared_from_this();
    }
private:
    std::string _name;
};

//1. RAII (Resource acquisition is initialization) or CADRe(Constructor Acquires, Destructor Release)

void chapter1() {
    cout << "1. RAII (Resource acquisition is initialization) or CADRe(Constructor Acquires, Destructor Release)" << endl;;

    cout << "Start" << endl;
    {
        Object obj("local variable");
    }

    cout << "Before shared pointer" << endl;
    {
        shared_ptr<Object> sp(new Object("heap variable"));
    }

    cout << "End" << endl;


    std::ofstream file("test.txt");
 
    if (!file.is_open())
        throw std::runtime_error("unable to open file"); //std::runtime_error inherits std::exception
 
    file << "hello world!" << std::endl;
}

//2. The difference between using make_shared function and using new keyword
void chapter2() {
    cout << endl << "2. The difference between using make_shared function and using new keyword" << endl;

    shared_ptr<int> sp1 = make_shared<int>(1);
    //shared_ptr<int> sp2(new int(1));

    int buf = 3;
    cout << "before: "<< buf << endl; //3
    //new(&buf) int(2);
    cout << "after: "<< buf << endl; //2
}

//3. shared_pointer with array
void chapter3() {
    cout << endl << "3. shared_pointer with array" << endl;
    shared_ptr<int> sp (new int(1));

    //boost에서는 배열형태를 관리할 수 있는 shared_ptr이 있지만 표준 라이브러리에는 없다.
    //boost::shared_ptr<int []> sp(new int[10]); 
    //boost::shared_array<int> sp_array(new int[10];

    auto deleter = [](int* ptr){
        cout << "Delete array" << endl;
        delete[] ptr;
    };

    //no
    shared_ptr<int> sp_array1(new int[10], deleter);
    shared_ptr<int> sp_array2(new int[10], default_delete<int[]>());

    //good
    shared_ptr<vector<int> > sp_vector = make_shared<vector<int> >();
}

//4. weak pointer
class Node
{
    //shared_ptr<Node> _parent;
    weak_ptr<Node> _parent;
    vector<shared_ptr<Node> > _children;
    std::string _name;
public:
    Node(const std::string& name) : _name(name) {}
    Node(const std::string& name, shared_ptr<Node> parent) : _name(name), _parent(parent) {}
    ~Node() {
        std::cout << _name << " destructor called" << std::endl;
    }
 
    void add_child(shared_ptr<Node> child) {
        _children.push_back(child);
    }

    // std::shared_ptr<Node> getParent() {

    //     shared_ptr<Node> sp = _parent.lock();
        
    //     if(!sp)
    //         cout << "My parent has destroyed" << endl;

    //     return sp;
    // }
};

void chapter4() {
    cout << endl << "4. weak pointer" << endl;
    shared_ptr<Node> root = make_shared<Node>("root");

    shared_ptr<Node> child1 = make_shared<Node>("child1", root);
    root->add_child(child1);

    shared_ptr<Node> child2 = make_shared<Node>("child2", child1);
    child1->add_child(child2);
}

//5. How does 'enable_shared_from_this'' work?
template<typename T>
class my_enable_shared_from_this
{
    mutable weak_ptr<T> _this_ptr;
protected:
    my_enable_shared_from_this() {}
    
    //invoked automatically by shared_ptr
    void _interanl_accept_owner(shared_ptr<T> sp)
    {
        _this_ptr = sp;
    }
 
public:
    shared_ptr<T> shared_from_this()
    {
        return _this_ptr.lock();
    }

    shared_ptr<T const> shared_from_this() const
    {
        return _this_ptr.lock();
    }
};

//6. casting of shared pointer
class A {};
 
class B : public A { };
 
void chapter6()
{
    shared_ptr<B> sp1 = make_shared<B>();
 
    shared_ptr<A> sp2( dynamic_pointer_cast<A>(sp1));
    shared_ptr<A> sp3( static_pointer_cast<A>(sp1));
 
    //shared_ptr<A> sp4(dynamic_cast<A*>(sp1.get())); //Error! Undefined behavior
}


//7. atomic operation of shared pointer
template<class T> 
void atomic_store( shared_ptr<T> * p, shared_ptr<T> r );

template<class T> 
shared_ptr<T> atomic_exchange( shared_ptr<T> * p, shared_ptr<T> r );

template<class T> 
shared_ptr<T> atomic_load( shared_ptr<T> const * p );

template<class T> 
bool atomic_is_lock_free( shared_ptr<T> const * p );

//8. other smart pointer
//unique_ptr

}//namespace toast {

int main(){
    using namespace toast;

    chapter1();
    chapter2();
    chapter3();
    chapter4();
    chapter6();

    return 0;
}