#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "Singleton.h"

#include <iostream>
class a:
    public Singleton<a>
{};

template<>
a* Singleton<a>::m_pInstance = nullptr;

int main()
{
    glm::vec3 a(-0.5f, -0.5f, -0.5f);
    std::cout << a.length() << std::endl;
    return 0;
}