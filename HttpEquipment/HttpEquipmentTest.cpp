#include "HttpEquipment.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Equipment* eq1 = new HttpEquipment("../../TSLParserTest/light.json", "localhost:8000");
    // std::cout << eq1->getUrlbyIdentifier(argv[1]) << std::endl;
    std::vector<std::string> argvs;
    for (int i = 2; i < argc; ++i) {
        argvs.push_back(argv[i]);
    }

    std::cout << eq1->invokeService(argv[1], argvs) << std::endl;

    // std::cout << eq1->getProPerty(argv[1]) << std::endl;

    return 0;
}
