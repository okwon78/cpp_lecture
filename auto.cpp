#include <type_traits>
#include <typeinfo>
#include <string>
#include <iostream>

template <typename T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::string r = typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

using namespace std;

int main()
{
    const int& i = 0;
    int a;
    std::cout << "typeid " << typeid(decltype(a)).name() << endl;
    std::cout << "type_name " << type_name<decltype(a)>() << endl;
}