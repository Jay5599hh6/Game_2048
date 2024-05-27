#include <iostream>  
using namespace std; 

# define  Map_size 4

void Initlize(int(&MAP)[Map_size][Map_size]) {
    cout << "----------初始化已完成----------" << endl;
    for (int i = 0; i < Map_size; i++) {
        for (int j = 0; j < Map_size; j++) {
            MAP[i][j] = 0;
        }
    }
}