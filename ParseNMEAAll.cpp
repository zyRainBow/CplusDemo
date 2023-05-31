#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define HEADER_SIZE (3)

// 定义NMEA消息类型的枚举
enum NmeaMessageType : int32_t {
    UNKNOWM = -1,
    GGA,
    GLL,
    GSA,
    GSV,
    RMC,
    VTG
};

// 定义NMEA消息结构体
struct NmeaMessage {
    NmeaMessageType type;   // 消息类型
    std::vector<std::string> fields;    // 消息字段
};

// 解析NMEA消息
NmeaMessage parseNmeaMessage(const std::string& message) {
    NmeaMessage result;
    result.type = NmeaMessageType::UNKNOWM;
    result.fields.clear();

    // 判断消息是否以$开头
    if (message[0] != '$') {
        printf("%s called: NMEA format is error !", __func__);
        return result;
    }

    // 判断消息中*的index
    size_t starPos = message.find('*');
    printf("%s called: starPos=%lu !\n", __func__, starPos);
    if (starPos == std::string::npos) {
        printf("%s called: not ending with * !", __func__);
        return result;
    }

    // 计算校验和: 本条语句从'$'到'*'之间的所有字符进行异或得到的16 进制数
    uint8_t checksum = 0;
    for (size_t i = 1; i < starPos; i++) {
        checksum ^= message[i];
    }
    printf("%s called: checksum[%d] !\n", __func__, checksum);

    // 比较校验和:
    uint8_t expectedChecksum = 0;
    std::stringstream ss;
    ss << std::hex << message.substr(starPos + 1, 2);
    //用stoi函数，将十六进制的字符串转换为十进制
    // expectedChecksum = std::stoi(ss.str(),0,16);
    expectedChecksum = std::stoi(message.substr(starPos + 1, 2),0,16);
    printf("%s called: ss[%s], expectedChecksum[%d] !\n", __func__, ss.str().c_str(), expectedChecksum);
    
    if (checksum != expectedChecksum) {
        printf("%s called: checksum is error !\n", __func__);
        return result;
    }

    // 分割消息字段
    std::stringstream ss2(message);
    std::string field;
    while (std::getline(ss2, field, ',')) {
        result.fields.push_back(field);
    }

    char header[HEADER_SIZE + 1];
    memset(header, 0, HEADER_SIZE + 1);
    std::strncpy(header, result.fields[0].c_str()+3, HEADER_SIZE);
    header[HEADER_SIZE] = 0;
    printf("%s called: header=%s \n", __func__, header);

    // 判断消息类型
    if (!strcmp(header,"GGA")) {
        result.type = NmeaMessageType::GGA;
    } else if (!strcmp(header,"GLL")) {
        result.type = NmeaMessageType::GLL;
    } else if (!strcmp(header,"GSA")) {
        result.type = NmeaMessageType::GSA;
    } else if (!strcmp(header,"GSV")) {
        result.type = NmeaMessageType::GSV;
    } else if (!strcmp(header,"RMC")) {
        result.type = NmeaMessageType::RMC;
    } else if (!strcmp(header,"VTG")) {
        result.type = NmeaMessageType::VTG;
    } else {
        printf("%s called: type error !\n", __func__);
    }
    return result;
}

int main() {
    // 测试数据
    std::string NMEA_GGA = "$GPGGA,025149.00,3849.728159,N,11527.257656,E,1,09,0.7,37.5,M,-10.0,M,,*77";
    std::string NMEA_RMC = "$GPRMC,025148.00,A,3849.728158,N,11527.257657,E,0.0,0.0,080322,5.4,W,A*24";

    // 解析消息
    NmeaMessage msgGGA = parseNmeaMessage(NMEA_GGA);
    NmeaMessage msgRMC = parseNmeaMessage(NMEA_RMC);

    // 输出结果
    std::cout << "msgGGA: type=" << static_cast<int>(msgGGA.type) << ", fields=";
    for (const auto& field : msgGGA.fields) {
        std::cout << field << ",";
    }
    std::cout << std::endl;

    std::cout << "msgRMC: type=" << static_cast<int>(msgRMC.type) << ", fields=";
    for (const auto& field : msgRMC.fields) {
        std::cout << field << ",";
    }
    std::cout << std::endl;

    return 0;
}