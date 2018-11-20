#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main()
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

    vec = trans * vec;
    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;

    return 0;
}