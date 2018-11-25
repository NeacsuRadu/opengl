#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Singleton.h"

class a:
    public Singleton<a>
{};

template<>
a* Singleton<a>::m_pInstance = nullptr;

int main()
{
    a& obj = a::Instance();

}