#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>

const int MAP_SIZE = 4;  // 更改为 MAP_SIZE
const int WIN_NUMBER = 2048;
const int CELL_SIZE = 100;  // 每个单元格大小为100×100
int board[MAP_SIZE][MAP_SIZE];  // 使用 MAP_SIZE

void initGame() {
    memset(board, 0, sizeof(board)); //将 board 数组中的所有元素设置为0
    srand((unsigned)time(0)); //用于设置随机数生成器的种子
    int x1 = rand() % MAP_SIZE, y1 = rand() % MAP_SIZE;
    int x2 = rand() % MAP_SIZE, y2 = rand() % MAP_SIZE;
    while (x1 == x2 && y1 == y2) {  //说明两个位置相同，则重新生成第二个位置，直到它们不同为止
        x2 = rand() % MAP_SIZE;
    }
    board[x1][y1] = board[x2][y2] = 2; //初始化两个格子的值
}

void drawBoard() {
    setfillcolor(WHITE);
    solidrectangle(0, 0, CELL_SIZE * MAP_SIZE, CELL_SIZE * MAP_SIZE);
    settextstyle(40, 0, _T("Consolas")); //设置文本样式，字体为 Consolas，大小为 40
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] != 0) {
                // EasyX库中的 outtextxy 函数用于在图形窗口中绘制文本，它要求输入的文本类型是 const wchar_t*（宽字符字符串）。所以必须把整数转换为宽字符串
                // 游戏中的每个方块显示的内容是一个数字，例如 2, 4, 8 等。这些数字在 board 数组中是以整数形式存储的，但在绘制时需要以字符串形式显示
                std::wstringstream wss; // 创建一个宽字符串流
                wss << board[i][j];  // 将单元格中的数字放入字符串流
                std::wstring str = wss.str(); // 将字符串流转换为宽字符串
                setfillcolor(0xCCCCCC); // 设置填充颜色为浅灰色
                solidrectangle(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE); // 绘制一个填充的浅灰色矩形，表示这个单元格
                setbkmode(TRANSPARENT);  // 设置背景模式为透明，以便在绘制文本时不会覆盖背景颜色
                settextcolor(BLACK); // 设置文本颜色为黑色
                outtextxy(j * CELL_SIZE + 40, i * CELL_SIZE + 20, str.c_str());  // 在单元格中绘制数字，位置是单元格左上角偏移40像素和20像素的位置
            }
            else {
                setfillcolor(0xEEEEEE);  // 设置填充颜色为更浅的灰色
                solidrectangle(j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE); // 绘制一个填充的浅灰色矩形，表示这个空的单元格
            }
        }
    }
}

// 在棋盘上随机选择一个空位置，并在该位置放置一个新的数字方块
void addRandomTile() {
    std::vector<std::pair<int, int>> emptyTiles;  // 定义一个空位置向量  用于存储棋盘上所有空位置的坐标
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyTiles.push_back({ i, j });
            }
        }
    }
    if (!emptyTiles.empty()) { //检查是否有空位置
        int idx = rand() % emptyTiles.size();
        int val = (rand() % 10 == 0) ? 4 : 2;
        board[emptyTiles[idx].first][emptyTiles[idx].second] = val;
    }
}

// 向左移动方块的逻辑。
// 具体来说，它遍历游戏板上的每一行，将非零的方块尽可能向左移动，合并相邻相同的方块。函数返回一个布尔值，表示在这一操作中是否有方块移动或合并
bool moveLeft() {
    bool moved = false;
    for (int i = 0; i < MAP_SIZE; ++i) {
        int lastMerged = -1; // lastMerged 变量初始值为 -1，用于跟踪在当前行中最近一次合并的位置，防止同一个方块在同一回合内被多次合并
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
    initgraph(CELL_SIZE * MAP_SIZE, CELL_SIZE * MAP_SIZE); // 创建画布 
    initGame();
    drawBoard();
    while (true) {
        if (_kbhit()) {  // 是一个检测键盘输入的函数
            char ch = _getch(); // 获取按下的键
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
    closegraph(); // 关闭当前画布
    return 0;
}
