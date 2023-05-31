#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <vector>

#undef TEST_MSB_LSB
// #define TEST_MSB_LSB 1
#undef TEST_AA

using namespace std;

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

int main(){
    cout<<"***Test---C++!***"<<endl;

	test_MSB_LSB();

	testPreDefineMacros();
    
    /********************Struct/Enum Test**********************/
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
    /********************Struct/Enum Test**********************/
    
    return 0;
}