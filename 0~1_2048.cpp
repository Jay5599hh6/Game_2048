#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>

const int MAP_SIZE = 4;  // ����Ϊ MAP_SIZE
const int WIN_NUMBER = 2048;
const int CELL_SIZE = 100;  // ÿ����Ԫ���СΪ100��100
int board[MAP_SIZE][MAP_SIZE];  // ʹ�� MAP_SIZE

void initGame() {
    memset(board, 0, sizeof(board)); //�� board �����е�����Ԫ������Ϊ0
    srand((unsigned)time(0)); //�������������������������
    int x1 = rand() % MAP_SIZE, y1 = rand() % MAP_SIZE;
    int x2 = rand() % MAP_SIZE, y2 = rand() % MAP_SIZE;
    while (x1 == x2 && y1 == y2) {  //˵������λ����ͬ�����������ɵڶ���λ�ã�ֱ�����ǲ�ͬΪֹ
        x2 = rand() % MAP_SIZE;
    }
    board[x1][y1] = board[x2][y2] = 2; //��ʼ���������ӵ�ֵ
}

void drawBoard() {
    setfillcolor(WHITE);
    solidrectangle(0, 0, CELL_SIZE * MAP_SIZE, CELL_SIZE * MAP_SIZE);
    settextstyle(40, 0, _T("Consolas")); //�����ı���ʽ������Ϊ Consolas����СΪ 40
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] != 0) {
                // EasyX���е� outtextxy ����������ͼ�δ����л����ı�����Ҫ��������ı������� const wchar_t*�����ַ��ַ����������Ա��������ת��Ϊ���ַ���
                // ��Ϸ�е�ÿ��������ʾ��������һ�����֣����� 2, 4, 8 �ȡ���Щ������ board ����������������ʽ�洢�ģ����ڻ���ʱ��Ҫ���ַ�����ʽ��ʾ
                std::wstringstream wss; // ����һ�����ַ�����
                wss << board[i][j];  // ����Ԫ���е����ַ����ַ�����
                std::wstring str = wss.str(); // ���ַ�����ת��Ϊ���ַ���
                setfillcolor(0xCCCCCC); // ���������ɫΪǳ��ɫ
                solidrectangle(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE); // ����һ������ǳ��ɫ���Σ���ʾ�����Ԫ��
                setbkmode(TRANSPARENT);  // ���ñ���ģʽΪ͸�����Ա��ڻ����ı�ʱ���Ḳ�Ǳ�����ɫ
                settextcolor(BLACK); // �����ı���ɫΪ��ɫ
                outtextxy(j * CELL_SIZE + 40, i * CELL_SIZE + 20, str.c_str());  // �ڵ�Ԫ���л������֣�λ���ǵ�Ԫ�����Ͻ�ƫ��40���غ�20���ص�λ��
            }
            else {
                setfillcolor(0xEEEEEE);  // ���������ɫΪ��ǳ�Ļ�ɫ
                solidrectangle(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE); // ����һ������ǳ��ɫ���Σ���ʾ����յĵ�Ԫ��
            }
        }
    }
}

// �����������ѡ��һ����λ�ã����ڸ�λ�÷���һ���µ����ַ���
void addRandomTile() {
    std::vector<std::pair<int, int>> emptyTiles;  // ����һ����λ������  ���ڴ洢���������п�λ�õ�����
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyTiles.push_back({ i, j });
            }
        }
    }
    if (!emptyTiles.empty()) { //����Ƿ��п�λ��
        int idx = rand() % emptyTiles.size();
        int val = (rand() % 10 == 0) ? 4 : 2;
        board[emptyTiles[idx].first][emptyTiles[idx].second] = val;
    }
}

// �����ƶ�������߼���
// ������˵����������Ϸ���ϵ�ÿһ�У�������ķ��龡���������ƶ����ϲ�������ͬ�ķ��顣��������һ������ֵ����ʾ����һ�������Ƿ��з����ƶ���ϲ�
bool moveLeft() {
    bool moved = false;
    for (int i = 0; i < MAP_SIZE; ++i) {
        int lastMerged = -1; // lastMerged ������ʼֵΪ -1�����ڸ����ڵ�ǰ�������һ�κϲ���λ�ã���ֹͬһ��������ͬһ�غ��ڱ���κϲ�
        for (int j = 1; j < MAP_SIZE; ++j) {
            if (board[i][j] != 0) {
                int k = j;
                while (k > 0 && board[i][k - 1] == 0) {
                    board[i][k - 1] = board[i][k];
                    board[i][k] = 0;
                    k--;
                    moved = true;
                }
                if (k > 0 && board[i][k - 1] == board[i][k] && lastMerged != k - 1) {
                    board[i][k - 1] *= 2;
                    board[i][k] = 0;
                    lastMerged = k - 1;
                    moved = true;
                }
            }
        }
    }
    return moved;
}

bool moveRight() {
    bool moved = false;
    for (int i = 0; i < MAP_SIZE; ++i) {
        int lastMerged = -1;
        for (int j = MAP_SIZE - 2; j >= 0; --j) {
            if (board[i][j] != 0) {
                int k = j;
                while (k < MAP_SIZE - 1 && board[i][k + 1] == 0) {
                    board[i][k + 1] = board[i][k];
                    board[i][k] = 0;
                    k++;
                    moved = true;
                }
                if (k < MAP_SIZE - 1 && board[i][k + 1] == board[i][k] && lastMerged != k + 1) {
                    board[i][k + 1] *= 2;
                    board[i][k] = 0;
                    lastMerged = k + 1;
                    moved = true;
                }
            }
        }
    }
    return moved;
}

bool moveUp() {
    bool moved = false;
    for (int j = 0; j < MAP_SIZE; ++j) {
        int lastMerged = -1;
        for (int i = 1; i < MAP_SIZE; ++i) {
            if (board[i][j] != 0) {
                int k = i;
                while (k > 0 && board[k - 1][j] == 0) {
                    board[k - 1][j] = board[k][j];
                    board[k][j] = 0;
                    k--;
                    moved = true;
                }
                if (k > 0 && board[k - 1][j] == board[k][j] && lastMerged != k - 1) {
                    board[k - 1][j] *= 2;
                    board[k][j] = 0;
                    lastMerged = k - 1;
                    moved = true;
                }
            }
        }
    }
    return moved;
}

bool moveDown() {
    bool moved = false;
    for (int j = 0; j < MAP_SIZE; ++j) {
        int lastMerged = -1;
        for (int i = MAP_SIZE - 2; i >= 0; --i) {
            if (board[i][j] != 0) {
                int k = i;
                while (k < MAP_SIZE - 1 && board[k + 1][j] == 0) {
                    board[k + 1][j] = board[k][j];
                    board[k][j] = 0;
                    k++;
                    moved = true;
                }
                if (k < MAP_SIZE - 1 && board[k + 1][j] == board[k][j] && lastMerged != k + 1) {
                    board[k + 1][j] *= 2;
                    board[k][j] = 0;
                    lastMerged = k + 1;
                    moved = true;
                }
            }
        }
    }
    return moved;
}

bool canMove() {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] == 0) return true;
            if (i > 0 && board[i][j] == board[i - 1][j]) return true;
            if (i < MAP_SIZE - 1 && board[i][j] == board[i + 1][j]) return true;
            if (j > 0 && board[i][j] == board[i][j - 1]) return true;
            if (j < MAP_SIZE - 1 && board[i][j] == board[i][j + 1]) return true;
        }
    }
    return false;
}

bool checkWin() {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] == WIN_NUMBER) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    initgraph(CELL_SIZE * MAP_SIZE, CELL_SIZE * MAP_SIZE); // �������� 
    initGame();
    drawBoard();
    while (true) {
        if (_kbhit()) {  // ��һ������������ĺ���
            char ch = _getch(); // ��ȡ���µļ�
            bool moved = false;
            switch (ch) {
            case 'w':
                moved = moveUp();
                break;
            case 's':
                moved = moveDown();
                break;
            case 'a':
                moved = moveLeft();
                break;
            case 'd':
                moved = moveRight();
                break;
            }
            if (moved) {
                addRandomTile();
                drawBoard();
                if (checkWin()) {
                    MessageBox(GetHWnd(), _T("You Win!"), _T("2048"), MB_OK);
                    break;
                }
                if (!canMove()) {
                    MessageBox(GetHWnd(), _T("Game Over!"), _T("2048"), MB_OK);
                    break;
                }
            }
        }
    }
    closegraph(); // �رյ�ǰ����
    return 0;
}
