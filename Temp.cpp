#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>

const int MAP_SIZE = 4;
const int WIN_NUMBER = 2048;
const int CELL_SIZE = 100;
const int WIDTH = CELL_SIZE * MAP_SIZE;
const int HEIGHT = CELL_SIZE * MAP_SIZE + 100; // 预留100像素高度用于显示分数
int board[MAP_SIZE][MAP_SIZE];
int score = 0;
int best = 0;

void initGame() {
    memset(board, 0, sizeof(board));
    score = 0;
    srand((unsigned)time(0));
    int x1 = rand() % MAP_SIZE, y1 = rand() % MAP_SIZE;
    int x2 = rand() % MAP_SIZE, y2 = rand() % MAP_SIZE;
    while (x1 == x2 && y1 == y2) {
        x2 = rand() % MAP_SIZE;
    }
    board[x1][y1] = board[x2][y2] = 2;
}

COLORREF getColor(int number) {
    switch (number) {
    case 2: return RGB(238, 228, 218);
    case 4: return RGB(237, 224, 200);
    case 8: return RGB(242, 177, 121);
    case 16: return RGB(245, 149, 99);
    case 32: return RGB(246, 124, 95);
    case 64: return RGB(246, 94, 59);
    case 128: return RGB(237, 207, 114);
    case 256: return RGB(237, 204, 97);
    case 512: return RGB(237, 200, 80);
    case 1024: return RGB(237, 197, 63);
    case 2048: return RGB(237, 194, 46);
    default: return RGB(205, 193, 180);
    }
}

void drawBoard() {
    setfillcolor(RGB(187, 173, 160));
    solidrectangle(0, 0, WIDTH, HEIGHT);

    setfillcolor(RGB(119, 110, 101));
    solidrectangle(0, HEIGHT - 100, WIDTH, HEIGHT);

    std::wstring scoreStr = L"Score: " + std::to_wstring(score);
    std::wstring bestStr = L"Best: " + std::to_wstring(best);
    settextcolor(WHITE);
    settextstyle(30, 0, _T("Consolas"));
    outtextxy(20, HEIGHT - 80, scoreStr.c_str());
    outtextxy(WIDTH - 180, HEIGHT - 80, bestStr.c_str());

    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            int number = board[i][j];
            setfillcolor(getColor(number));
            solidrectangle(j * CELL_SIZE + 5, i * CELL_SIZE + 5, (j + 1) * CELL_SIZE - 5, (i + 1) * CELL_SIZE - 5);
            if (number != 0) {
                std::wstringstream wss;
                wss << number;
                std::wstring str = wss.str();
                setbkmode(TRANSPARENT);
                settextcolor(number > 4 ? RGB(255, 255, 255) : RGB(119, 110, 101));
                settextstyle(40, 0, _T("Consolas"));
                outtextxy(j * CELL_SIZE + (CELL_SIZE - textwidth(str.c_str())) / 2, i * CELL_SIZE + (CELL_SIZE - textheight(str.c_str())) / 2, str.c_str());
            }
        }
    }
}

void addRandomTile() {
    std::vector<std::pair<int, int>> emptyTiles;
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyTiles.push_back({ i, j });
            }
        }
    }
    if (!emptyTiles.empty()) {
        int idx = rand() % emptyTiles.size();
        int val = (rand() % 10 == 0) ? 4 : 2;
        board[emptyTiles[idx].first][emptyTiles[idx].second] = val;
    }
}

bool moveLeft() {
    bool moved = false;
    for (int i = 0; i < MAP_SIZE; ++i) {
        int lastMerged = -1;
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
                    score += board[i][k - 1];
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
                    score += board[i][k + 1];
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
                    score += board[k - 1][j];
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
                    score += board[k + 1][j];
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

void newGame() {
    initGame();
    if (score > best) {
        best = score;
    }
    score = 0;
    drawBoard();
}

int main() {
    initgraph(WIDTH, HEIGHT);
    newGame();

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
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
            case 'r': // 按 'r' 键重新开始游戏
                newGame();
                continue;
            }
            if (moved) {
                addRandomTile();
                drawBoard();
                if (checkWin()) {
                    MessageBox(GetHWnd(), _T("You Win! Press 'r' to start a new game."), _T("2048"), MB_OK);
                }
                if (!canMove()) {
                    MessageBox(GetHWnd(), _T("Game Over! Press 'r' to start a new game."), _T("2048"), MB_OK);
                }
            }
        }
    }
    closegraph();
    return 0;
}
