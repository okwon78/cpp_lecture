#include <iostream>
#include <string>
#include <list>
#include <type_traits>

using namespace std;

struct Object
{
    Object() : _str("default") { cout << "default" << endl; }
    Object(const char* str) : _str(str) {}
    Object(const Object& obj) 
    {
        cout << "copy" << endl;
    }

    Object(Object&& obj)
    {
        _str = move(obj._str);
        cout << "move" << endl;
    }
    std::string _str;

    void print() const { cout << _str << endl; }

    void setString(const char* str) {
        _str = str;
    }
};

template<typename T>
void func(T& arg)
{
    static_assert(std::is_same<T, const int>(), "");
}

int main()
{
    cout << "test" << endl;

    const int x = 5;

    func(x);

    auto a = x;
}