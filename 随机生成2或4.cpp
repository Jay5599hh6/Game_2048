#include <iostream>
#include <random>

int generateRandomNumber() {
    // 创建一个随机设备对象，用于生成种子
    std::random_device rd;

    // 创建一个Mersenne Twister随机数生成器，并使用随机设备生成的种子初始化
    std::mt19937 gen(rd());

    // 定义一个离散分布，指定生成2和4的概率
    std::discrete_distribution<> dist({ 3, 1 });

    // 使用分布对象生成随机数
    int number = dist(gen);
    return (number == 0) ? 2 : 4; // 将分布结果映射到2和4
}