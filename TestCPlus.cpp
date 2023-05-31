#include <stdio.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <vector>

#undef TEST_MSB_LSB
// #define TEST_MSB_LSB 1
#undef TEST_AA

using namespace std;

/**
 * @brief 使用find_first_of通过指定的分隔符分割字符串
 * @param srcStr 源字符串
 * @param v_data 被分割后的字符串容器引用
 * @param splitSymbol 分割符号
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
 * @brief 使用getline通过指定的分隔符分割字符串
 * @param srcStr 源字符串
 * @param v_data 被分割后的字符串容器引用
 * @param delimiter 分割符号
 */
void getDataByDelimiter(string srcStr, vector<string> &v_data, char delimiter)
{
    // 将语句按逗号分隔成多个字段
    stringstream ss(srcStr);
    string field;
    while (std::getline(ss, field, delimiter)) { //getline（）返回的istream是隐式调用它的操作符void *（）方法的，它返回流是否已经遇到错误。 因此，它比eof（）的调用进行更多的检查。
    // while (!std::getline(ss, field, delimiter).eof()) {
        std::cout << "field:" << field << std::endl;
        v_data.push_back(field);
    }
}

/**
 * 测试大、小端字节序
 */
void test_MSB_LSB()
{
	//小端字节序的存储规则(低低低)：数据的低位存储在地址低位，数据的高位存储在地址高位
    //大端字节序的存储规则(低高高)：数据的低位存储在地址高位，数据的高位存储在地址低位
	cout<<"******************************测试大、小端字节序******************************"<<endl;
    #ifndef TEST_MSB_LSB
    int aa = 0x12345678;
    assert(aa>0);
    char *p = NULL;
    
    p = (char*)&aa;
    if (*p == 0x78) {
        cout<<"***小端字节序***"<<endl;  
    } else if (*p == 0x12) {
        cout<<"***大端字节序***"<<endl;  
    } else {
    }

    for (int i=0;i<4;i++) {
        printf("address[%p], p[%d]=[%x]\n", (p+i), i, *(p+i));
    }
    #endif
}

/**
 * 测试预定义宏
 */
void testPreDefineMacros() {
	cout<<"******************************测试预定义宏******************************"<<endl;
    cout<<"**参考链接：https://blog.csdn.net/qq_33706673/article/details/78628202**"<<endl;
    cout<<"******************************测试预定义宏******************************\n"<<endl;
    printf("func called: %s\n", __func__);
    printf("FUNCTION called: %s\n", __FUNCTION__);
    printf("PRETTY-FUNCTION called: %s\n", __PRETTY_FUNCTION__);
    printf("===============================================\n");
    printf("Compiled Data:[%s] Time:[%s]\n", __DATE__,__TIME__);//Data:"Mmm dd yyTime:"hh:mm:ss"
    printf("===============================================\n");
    printf("line:%d in file:%s", __LINE__, __FILE__);
    
}

void testString() {
	string nmea_rmc = "$GPRMC,,A,3849.728158,N,11527.257657,E,0.0,0.0,080322,5.4,W,A*24";
    const char *frame_rmc = nmea_rmc.c_str();

    std::vector<std::string> v_data1;
    std::vector<std::string> v_data2;
    split_string(frame_rmc, v_data1, ",");
    getDataByDelimiter(frame_rmc, v_data2,',');

	std::stringstream ss("12");
    //按十进制输出整型
    printf("\n%s called: ss[%d]!\n", __func__, stoi(ss.str(),0));
    //按八进制输出整型
    printf("%s called: ss-o[%d]!\n", __func__, stoi(ss.str(),0,8));
    //按十六进制输出整型
    printf("%s called: ss-hex[%d]!\n", __func__, stoi(ss.str(),0,16));
}


void testStructAndEnum() {
    struct TEST_STRUCT {
	   uint64_t a;
	   uint64_t b;
	};
	
	struct TEST_VECTORSTRUCT {
	   vector<int> data;
	};
	TEST_VECTORSTRUCT vectorStruct;
	for(int i = 0; i < 10; i++){
	    vectorStruct.data.push_back(i);
	}
	for(auto a:vectorStruct.data) {
	    printf("a: %d\n", a);
	}
	
	
	enum TEST_ENUM:uint8_t {
	    type = 0,
	    type2 =1
	};
	
	typedef uint64_t testDef;
	
	vector<TEST_STRUCT> testVector;
	for (int i = 0; i < 10; i++){
	    TEST_STRUCT test;
	    test.a=i+1;
	    test.b=2*i;
	    printf("before-test: a[%ld]\tb[%ld]\n", test.a,test.b);
        testVector.push_back(move(test));
        printf("after-test: a[%ld]\tb[%ld]\n", test.a,test.b);
    }
    
    vector<TEST_STRUCT>::iterator it;
    for(it = testVector.begin(); it != testVector.end(); it ++){
        printf("struct: a[%ld]\tb[%ld]\n", (*it).a,(*it).b);
    }
    printf("testVector[0]: a[%ld]\tb[%ld]\n", testVector[0].a,testVector[0].b);
	
	TEST_STRUCT testStruct;
	TEST_ENUM testEnum;
	uint8_t a = 0xff;
	//vector所占字节数是固定的
	printf("a:%d\tstructSize:%ld\tenumSize:%ld\ttesDef:%ld\ttestVector-len:%ld\ttestVector:%ld\t", 
	    a, 
	    sizeof(testStruct),
	    sizeof(testEnum),
	    sizeof(testDef),
	    testVector.size(),
	    sizeof(testVector));
}

int main(){
    cout<<"***Test---C++!***"<<endl;

	// test_MSB_LSB();

	// testPreDefineMacros();

	testString();

	// testStructAndEnum();

    return 0;
}