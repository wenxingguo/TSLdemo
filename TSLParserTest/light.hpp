/**************************************************
 * 该文件由 TSLParserTest生成
 **************************************************/
#include <string>
#include <vector>

struct lightInfo_result_struct { //  灯的状态
    int lightStatus; // 灯的开关状态

    std::string lightColor; // 灯的颜色

    std::string IP; // IP
};

class light1 {

public:
    // 服务
    void lightON() // 打开灯 打开灯 sync
    {
        // code
    }
    void lightOFF() // 关闭灯 关闭灯 sync
    {
        // code
    }
    lightInfo_result_struct lightInfo() // 查看灯的状态 查看灯的状态 sync
    {
        // code
    }
    int Setcolor(std::string color) // 更改灯的颜色 更改灯的颜色 sync
    {
        // code
    }

public:
    int lightStatus; // 灯的状态 灯的状态

    std::string IP; // IP IP

    std::string lightColor; // 灯的颜色 lightColor
};
