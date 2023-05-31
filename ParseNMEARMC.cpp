#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// 解析NMEA RMC语句的函数
void parseRMC(string sentence) {
    // 将语句按逗号分隔成多个字段
    vector<string> fields;
    stringstream ss(sentence);
    string field;
    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }

    // 检查语句是否合法
    if (fields.size() < 13 || fields[0].substr(0, 3) != "$GP") {
        cout << "Invalid RMC sentence" << endl;
        return;
    }

    // 解析语句中的各个字段
    string time = fields[1];
    string status = fields[2];
    double latitude = stod(fields[3].substr(0, 2)) + stod(fields[3].substr(2)) / 60.0;
    if (fields[4] == "S") {
        latitude = -latitude;
    }
    double longitude = stod(fields[5].substr(0, 3)) + stod(fields[5].substr(3)) / 60.0;
    if (fields[6] == "W") {
        longitude = -longitude;
    }
    double speed = stod(fields[7]);
    double course = stod(fields[8]);
    string date = fields[9];
    double variation = stod(fields[10]);
    if (fields[11] == "W") {
        variation = -variation;
    }
    string mode = fields[12];

    // 输出解析结果
    cout << "Time: " << time << endl;
    cout << "Status: " << status << endl;
    cout << "Latitude: " << latitude << endl;
    cout << "Longitude: " << longitude << endl;
    cout << "Speed: " << speed << endl;
    cout << "Course: " << course << endl;
    cout << "Date: " << date << endl;
    cout << "Variation: " << variation << endl;
    cout << "Mode: " << mode << endl;
}

int main() {
    // 测试解析函数
    string sentence = "$GPRMC,,A,3849.728158,N,11527.257657,E,0.0,0.0,080322,5.4,W,A*24";;
    parseRMC(sentence);
    return 0;
}
