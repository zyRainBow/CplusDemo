#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#define SEPARATOR_COMMA ","

using namespace std;

/**
 * 功能：通过指定的分隔符分割字符串
 * 参数1 源字符串
 * 参数2 被分割后的字符串容器引用
 * 参数3 分割符号
 */
void split_string(string srcStr,vector<string> &v_splitStr,string splitSymbol)
{
	//清空容器
	vector<string>().swap(v_splitStr);
	while(string::npos != srcStr.find_first_of(splitSymbol)){
		//获取第一个分隔符之前的文件类型
		string strT = srcStr.substr(0,srcStr.find_first_of(splitSymbol));
		//将分割的字符加入到容器列表最后
		v_splitStr.push_back(strT);
		//删除分割字符前面的，保留后面的字符串
		srcStr = srcStr.substr(srcStr.find_first_of(splitSymbol)+1,srcStr.size());
	}
	//最后的一个分割符号后面或者没有匹配到分割符号的字符加入容器列表
	v_splitStr.push_back(srcStr);
}

/**
 * 功能：通过指定的分隔符分割字符串
 * 参数1 源字符串
 * 参数2 被分割后的字符串容器引用
 * 参数3 分割符号
 */
void parseDataByComma(string srcStr, vector<string> &v_data)
{
    // 将语句按逗号分隔成多个字段
    std::stringstream ss(srcStr);
    std::string field;
    while (std::getline(ss, field, ',')) {
        cout<<ss.str()<<endl;
        v_data.push_back(field);
    }
}

uint16_t getDataFieldPoint(uint8_t num, char *buffer)
{
    uint8_t count = 0;
    if (buffer == NULL) {
        printf("%s ,buffer is NULL", __FUNCTION__);
        return 0;
    }
    uint16_t len = strlen(buffer);
    for (uint16_t i = 0; i < len; i++)
    {
        if (buffer[i] == ',') {
            count++;
        }

        if (count == num) {
            /*Returns to the next position of the currently found field position.*/
            return i + 1;
        }
    }

    return 0;
}

void GSAParse(char* frame)
{
    /**
     * dest_gsa[0]: 定位系统标识
     * dest_gsa[1]: 定位模式状态，M=手动，A=自动
     * dest_gsa[2]: 定位类型，1=没有定位，2=2D定位，3=3D定位
     * dest_gsa[3~14]: PRN码（伪随机噪声码），正在用于解算位置的卫星号（前面的0也将被传输）。
     *  参与定位的卫星不足12 颗时不足的区域填空，多于12 颗只输出前12 颗卫星
     * dest_gsa[15]: PDOP位置精度因子（0.5~99.9）
     * dest_gsa[16]: HDOP水平精度因子（0.5~99.9）
     * dest_gsa[17]: VDOP垂直精度因子（0.5~99.9）
     * dest_gsa[18]: NMEA 协议定义的GNSS 系统ID 和校验和
     */ 
    vector<string> dest_gsa;
    split_string(frame, dest_gsa, SEPARATOR_COMMA);
    for(uint8_t i = 0; i < dest_gsa.size(); i++)
    {
        printf("gsa[%hhu]:%s ", i, dest_gsa[i].c_str());
        if (i == dest_gsa.size()-1)
        {
            printf("\n");  
        }
    }
}

void RMCParse(char *frame)
{
    /**
     * dest_rmc[0]: 定位系统标识
     * dest_rmc[1]: UTC时间，hhmmss（时分秒）格式
     * dest_rmc[2]: 定位状态，A=有效定位，V=无效定位
     * dest_rmc[3]: 纬度ddmm.mmmm（度分）格式
     * dest_rmc[4]: 纬度半球N（北半球）或S（南半球）
     * dest_rmc[5]: 经度dddmm.mmmm（度分）格式
     * dest_rmc[6]: 经度半球E（东经）或W（西经）
     * dest_rmc[7]: 地面速率（000.0~999.9节，前面的0也将被传输）1节kn = 1.85km/h
     * dest_rmc[8]: 地面航向（000.0~359.9度
     * dest_rmc[9]: UTC日期，ddmmyy（日月年）格式
     * dest_rmc[10]: 磁偏角（000.0~180.0度，前面的0也将被传输）
     * dest_rmc[11]: 磁偏角方向，E（东）或W（西
     * dest_rmc[12]: 定位模式（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）和校验和
     */ 
    vector<string> dest_rmc;
    // split_string(frame, dest_rmc, SEPARATOR_COMMA);
    parseDataByComma(frame, dest_rmc);
    for(uint8_t i = 0; i < dest_rmc.size(); i++)
    {
        printf("%s called: rmc[%hhu]:%s ", __func__, i, dest_rmc[i].c_str());
        if (dest_rmc[i] == "")
        {
            printf("rmc[%hhu]:%s error\n", i, dest_rmc[i].c_str());
        }
        if (i == dest_rmc.size()-1)
        {
            printf("\n");  
        }
    }
}

void GSVParse(char *frame)
{
    /**
     * dest_gsv[0]: 定位系统标识
     * dest_gsv[1]: GSV语句的总数
     * dest_gsv[2]: 本句GSV的编号
     * dest_gsv[3]: 可见卫星的总数（00~12，前面的0也将被传输）
     * dest_gsv[4]: 第1颗卫星的PRN码（伪随机噪声码）（01~32，前面的0也将被传输），可理解为卫星编号
     * dest_gsv[5]: 第1颗卫星的卫星仰角（00~90度，前面的0也将被传输）
     * dest_gsv[6]: 第1颗卫星的卫星方位角（000~359度，前面的0也将被传输）
     * dest_gsv[7]: 第1颗卫星的信噪比cn（00~99dB，没有跟踪到卫星时为空，前面的0也将被传输）
     * dest_gsv[8~11]: 第2颗卫星的PRN码、卫星仰角、卫星方位角、信噪比cn
     * dest_gsv[12~15]: 第3颗卫星的PRN码、卫星仰角、卫星方位角、信噪比cn
     * dest_gsv[16~19]: 第4颗卫星的PRN码、卫星仰角、卫星方位角、信噪比cn
     * dest_gsv[20]: NMEA 协议定义的信号ID和校验和
     */ 
    vector<string> dest_gsv;
    split_string(frame, dest_gsv, SEPARATOR_COMMA);
    for(uint8_t i = 0; i < dest_gsv.size(); i++)
    {
        printf("gsv[%hhu]:%s ", i, dest_gsv[i].c_str());
    }
}

void GGAParse(char *frame)
{
    /**
     * dest_gga[0]: 定位系统标识
     * dest_gga[1]: UTC 时间，格式为hhmmss.ss
     * dest_gga[2]: 纬度，格式为ddmm.mmmmm
     * dest_gga[3]: 纬度半球N（北半球）或S（南半球）
     * dest_gga[4]: 经度dddmm.mmmm（度分）格式
     * dest_gga[5]: 经度半球E（东经）或W（西经）
     * dest_gga[6]: 定位状态标识 0=未定位，1=单点定位，2=SBAS差分定位，4=RTK固定解，5=RTK浮点解，6=惯导定位
     * dest_gga[7]: 参与定位的卫星数量(00~12)
     * dest_gga[8]: 水平精度因子，0.00 - 99.99
     * dest_gga[9]: 海拔高度（-9999.9~99999.9）米
     * dest_gga[10]: 地球椭球面相对大地水准面的高度（-9999.9~99999.9）米
     * dest_gga[11]: 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空
     * dest_gga[12]: 差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）和校验和
     */ 
    vector<string> dest_gga;
    split_string(frame, dest_gga, SEPARATOR_COMMA);
    for(uint8_t i = 0; i < dest_gga.size(); i++)
    {
        printf("gga[%hhu]:%s ", i, dest_gga[i].c_str());
        if (i == dest_gga.size()-1)
        {
            printf("\n");  
        }
    }
}

void VTGParse(char *frame)
{
    /**
     * dest_vtg[0]: 定位系统标识
     * dest_vtg[1]: 以真北为参考基准的地面航向（000~359度，前面的0也将被传输）
     * dest_vtg[2]: 航向标志，固定填T
     * dest_vtg[3]: 以磁北为参考基准的地面航向（000~359度，前面的0也将被传输）
     * dest_vtg[4]: 航向标志，固定填M
     * dest_vtg[5]: 地面速率（000.0~999.9节，前面的0也将被传输
     * dest_vtg[6]: 速率单位，固定填N
     * dest_vtg[7]: 地面速率（0000.0~1851.8公里/小时，前面的0也将被传输）
     * dest_vtg[8]: 速率单位，固定填K
     * dest_vtg[9]: 定位模式（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）和校验和
     */ 
    vector<string> dest_vtg;
    split_string(frame, dest_vtg, SEPARATOR_COMMA);
    for(uint8_t i = 0; i < dest_vtg.size(); i++)
    {
        printf("vtg[%hhu]:%s ", i, dest_vtg[i].c_str());
        if (i == dest_vtg.size()-1)
        {
            printf("\n");  
        }
    }
}

void GLLParse(char *frame)
{
    /**
     * dest_gll[0]: 定位系统标识
     * dest_gll[1]: 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
     * dest_gll[2]: 纬度半球N（北半球）或S（南半球）
     * dest_gll[3]: 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
     * dest_gll[4]: 经度半球E（东经）或W（西经）
     * dest_gll[5]: UTC时间，hhmmss（时分秒）格式
     * dest_gll[6]: 定位状态，A=有效定位，V=无效定位
     * dest_gll[7]: 定位模式（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）和校验和
     */ 
    vector<string> dest_gll;
    split_string(frame, dest_gll, SEPARATOR_COMMA);
    for(uint8_t i = 0; i < dest_gll.size(); i++)
    {
        printf("gll[%hhu]:%s ", i, dest_gll[i].c_str());
        if (i == dest_gll.size()-1)
        {
            printf("\n");  
        }
    }
}

int main(){
    cout<<"Parse NMEA !"<<endl;
    string nmea_gsa = "$GPGSA,A,2,03,04,09,16,22,26,27,29,31,,,,1.0,0.7,0.7,1*2E";
    string nmea_rmc = "$GPRMC,025148.00,A,3849.728158,N,11527.257657,E,0.0,0.0,080322,5.4,W,A*24";
    string nmea_gsv = "$GPGSV,4,1,13,03,15,248,40,04,46,302,49,08,02,195,30,09,14,317,31,1*6B";
    string nmea_gga = "$GPGGA,025149.00,3849.728159,N,11527.257656,E,1,09,0.7,37.5,M,-10.0,M,,*77";
    string nmea_vtg = "$GPVTG,0.0,T,5.4,M,0.0,N,0.0,K,A*22";
    string nmea_gll = "$GPGLL,4004.74005,N,11614.19613,E,060845,A,A*6F";
    
    const char *frame_gsa = nmea_gsa.c_str();
    const char *frame_rmc = nmea_rmc.c_str();
    const char *frame_gsv = nmea_gsv.c_str();
    const char *frame_gga = nmea_gga.c_str();
    const char *frame_vtg = nmea_vtg.c_str();
    const char *frame_gll = nmea_gll.c_str();
    
    GSAParse(const_cast<char*>(frame_gsa));
    RMCParse(const_cast<char*>(frame_rmc));
    GSVParse(const_cast<char*>(frame_gsv));
    GGAParse(const_cast<char*>(frame_gga));
    VTGParse(const_cast<char*>(frame_vtg));
    GLLParse(const_cast<char*>(frame_gll));
    
    // char buf[128] ;
    // memset(buf, 0, 128);
    // strncpy(buf, &frame[7], 2);
    // int value = atoi(buf);
    // printf("buf=%s value=%d \n", buf,value);
    
    return 0;
}