#if defined(TEST_APP)
//g++ TestApp.cpp -o ../../TestApp -D TEST_APP

    #include <iostream>
    using namespace std;

    int main(int argc, char *argv[]){
        char *test = new char[1024];
        char *variable = "test";
        cout << "Hello World!" << endl;
        cin >> test;
    }
#endif
