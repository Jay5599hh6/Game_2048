#include <iostream>
#include <random>

int generateRandomNumber() {
    // ����һ������豸����������������
    std::random_device rd;

    // ����һ��Mersenne Twister���������������ʹ������豸���ɵ����ӳ�ʼ��
    std::mt19937 gen(rd());

    // ����һ����ɢ�ֲ���ָ������2��4�ĸ���
    std::discrete_distribution<> dist({ 3, 1 });

    // ʹ�÷ֲ��������������
    int number = dist(gen);
    return (number == 0) ? 2 : 4; // ���ֲ����ӳ�䵽2��4
}