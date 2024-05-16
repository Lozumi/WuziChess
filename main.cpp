/*
 * 第四次作业 五子棋v5
 * 版本：5.0
 * 作者：2022303251 刘黎可
 * 时间：20240516
 */
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

#define BOARD_SIZE 15
#define CELL_SIZE 40
#define BOARD_OFFSET 20
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 40

typedef struct {
    int x;
    int y;
} Position;

typedef enum {
    EMPTY,
    PLAYER1,
    PLAYER2
} CellState;

CellState board[BOARD_SIZE][BOARD_SIZE];
bool isPlayer1Turn = true;

void drawTitle() {
    settextstyle(20, 0, _T("宋体"));
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 50, 20, _T("星戴露和她的朋友"));
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, 60, _T("五子棋游戏"));
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 110, 100, _T("V5.0"));
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, 140, _T("刘黎可制作"));
}

void drawButton(int x, int y, TCHAR text[]) {
    setfillcolor(RGB(100, 149, 237)); // 浅蓝色
    solidrectangle(x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    settextcolor(RGB(255, 255, 255)); // 白色
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("宋体")); // 修改这里的字符串常量
    int textWidth = textwidth(text);
    int textHeight = textheight(text);
    outtextxy(x + (BUTTON_WIDTH - textWidth) / 2, y + (BUTTON_HEIGHT - textHeight) / 2, text);
}

void drawChessBoard() {
    setbkcolor(RGB(249, 214, 91)); // 设置背景色为浅黄色
    cleardevice(); // 清空画布

    // 绘制棋盘背景图片
    IMAGE boardImg;
    loadimage(&boardImg, _T("src/back.jpg"), BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    putimage(BOARD_OFFSET, BOARD_OFFSET, &boardImg);

    // 绘制棋盘网格
    setlinecolor(RGB(0, 0, 0)); // 设置线条颜色为黑色
    for (int i = 0; i < BOARD_SIZE; i++) {
        line(BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + (BOARD_SIZE - 1) * CELL_SIZE, BOARD_OFFSET + i * CELL_SIZE); // 绘制横线
        line(BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + (BOARD_SIZE - 1) * CELL_SIZE); // 绘制竖线
    }

    // 绘制标题
    drawTitle();

    // 绘制按钮
    drawButton(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, BOARD_OFFSET + 180, _T("保存棋局"));
    drawButton(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, BOARD_OFFSET + 240, _T("读取棋局"));
    drawButton(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, BOARD_OFFSET + 300, _T("下子"));
}

void drawAlpha(IMAGE* picture, int picture_x, int picture_y) {
    // 变量初始化
    DWORD *dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD *draw = GetImageBuffer();
    DWORD *src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++) {
        for (int ix = 0; ix < picture_width; ix++) {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

void drawCharacter(Position pos, IMAGE *characterImg) {
    //绘制棋盘上的人物 
    int x = BOARD_OFFSET + pos.x * CELL_SIZE - CELL_SIZE / 2;
    int y = BOARD_OFFSET + pos.y * CELL_SIZE - CELL_SIZE / 2;
    drawAlpha(characterImg, x, y);
}

void drawBoardState() {
    // 绘制棋盘上的所有棋子
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == PLAYER1) {
                setfillcolor(BLACK);
                solidcircle(BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + j * CELL_SIZE, CELL_SIZE / 4);
            } else if (board[i][j] == PLAYER2) {
                setfillcolor(WHITE);
                solidcircle(BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + j * CELL_SIZE, CELL_SIZE / 4);
            }
        }
    }
}

void saveBoardState(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(board, sizeof(board), 1, file);
        fclose(file);
    } else {
        printf("保存棋局失败\n");
    }
}

void loadBoardState(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fread(board, sizeof(board), 1, file);
        fclose(file);
    } else {
        printf("读取棋局失败\n");
    }
}

bool checkWin(int x, int y, CellState player) {
    const int dx[4] = {1, 0, 1, 1};
    const int dy[4] = {0, 1, 1, -1};

    for (int i = 0; i < 4; i++) {
        int count = 1;
        for (int j = 1; j < 5; j++) {
            int nx = x + j * dx[i];
            int ny = y + j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) {
                count++;
            } else {
                break;
            }
        }
        for (int j = 1; j < 5; j++) {
            int nx = x - j * dx

[i];
            int ny = y - j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) {
            return true;
        }
    }
    return false;
}

bool isDoubleThree(int x, int y) {
    const int dx[4] = {1, 0, 1, 1};
    const int dy[4] = {0, 1, 1, -1};
    int threeCount = 0;

    for (int i = 0; i < 4; i++) {
        int count = 1;
        bool openStart = false, openEnd = false;

        for (int j = 1; j < 5; j++) {
            int nx = x + j * dx[i];
            int ny = y + j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == PLAYER1) {
                count++;
            } else if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == EMPTY) {
                openEnd = true;
                break;
            } else {
                break;
            }
        }

        for (int j = 1; j < 5; j++) {
            int nx = x - j * dx[i];
            int ny = y - j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == PLAYER1) {
                count++;
            } else if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == EMPTY) {
                openStart = true;
                break;
            } else {
                break;
            }
        }

        if (count == 3 && openStart && openEnd) {
            threeCount++;
        }

        if (threeCount >= 2) {
            return true;
        }
    }
    return false;
}

bool isDoubleFour(int x, int y) {
    const int dx[4] = {1, 0, 1, 1};
    const int dy[4] = {0, 1, 1, -1};
    int fourCount = 0;

    for (int i = 0; i < 4; i++) {
        int count = 1;
        bool openStart = false, openEnd = false;

        for (int j = 1; j < 5; j++) {
            int nx = x + j * dx[i];
            int ny = y + j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == PLAYER1) {
                count++;
            } else if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == EMPTY) {
                openEnd = true;
                break;
            } else {
                break;
            }
        }

        for (int j = 1; j < 5; j++) {
            int nx = x - j * dx[i];
            int ny = y - j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == PLAYER1) {
                count++;
            } else if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == EMPTY) {
                openStart = true;
                break;
            } else {
                break;
            }
        }

        if (count == 4 && (openStart || openEnd)) {
            fourCount++;
        }

        if (fourCount >= 2) {
            return true;
        }
    }
    return false;
}

bool isOverline(int x, int y) {
    const int dx[4] = {1, 0, 1, 1};
    const int dy[4] = {0, 1, 1, -1};

    for (int i = 0; i < 4; i++) {
        int count = 1;
        for (int j = 1; j < 6; j++) {
            int nx = x + j * dx[i];
            int ny = y + j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == PLAYER1) {
                count++;
            } else {
                break;
            }
        }
        for (int j = 1; j < 6; j++) {
            int nx = x - j * dx[i];
            int ny = y - j * dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == PLAYER1) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 6) {
            return true;
        }
    }
    return false;
}

bool checkForbiddenMove(int x, int y) {
    // 检查是否形成禁手
	// 双三禁手（Double Three）：在一个位置下子后同时形成两个活三（即两个可以延伸成五子连珠的三个子连珠）。
	// 双四禁手（Double Four）：在一个位置下子后同时形成两个活四（即两个可以延伸成五子连珠的四个子连珠）。
	// 长连禁手（Overline）：在一个位置下子后形成超过五子的连珠（即六子或更多连续的棋子）。
    if (isDoubleThree(x, y) || isDoubleFour(x, y) || isOverline(x, y)) {
        return true;
    }
    return false;
}

void showResult(TCHAR result[]) {
    settextstyle(40, 0, _T("宋体"));
    settextcolor(RGB(255, 0, 0));
    setbkmode(TRANSPARENT);
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE / 2 - 100, BOARD_OFFSET + BOARD_SIZE * CELL_SIZE / 2 - 20, result);
}

int main() {
    initgraph(BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2 + 220, BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2); // 初始化画布

    IMAGE character1Img, character2Img;
    loadimage(&character1Img, _T("src/character1.png"), CELL_SIZE, CELL_SIZE); // 加载人物1贴图
    loadimage(&character2Img, _T("src/character2.png"), CELL_SIZE, CELL_SIZE); // 加载人物2贴图

    Position characterPos = {7, 7}; // 初始化人物位置在棋盘中央

    drawChessBoard(); // 绘制棋盘
    drawCharacter(characterPos, isPlayer1Turn ? &character1Img : &character2Img); // 绘制人物
    drawBoardState(); // 绘制棋局

    bool gameOver = false;

    while (!gameOver) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg(); // 获取鼠标消息
            if (msg.uMsg == WM_LBUTTONDOWN) {
                int clickX = (msg.x - BOARD_OFFSET + CELL_SIZE / 2) / CELL_SIZE;
                int clickY = (msg.y - BOARD_OFFSET + CELL_SIZE / 2) / CELL_SIZE;

                // 检查是否点击了保存按钮
                if (msg.x >= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 && msg.x <= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 + BUTTON_WIDTH &&
                    msg.y >= BOARD_OFFSET + 180 && msg.y <= BOARD_OFFSET + 180 + BUTTON_HEIGHT) {
                    saveBoardState("board_state.dat");
                    continue;
                }

                // 检查是否点击了读取按钮
                if (msg.x >= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 && msg.x <= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 + BUTTON_WIDTH &&
                    msg.y >= BOARD_OFFSET + 240 && msg.y <= BOARD_OFFSET + 240 + BUTTON_HEIGHT) {
                    loadBoardState("board_state.dat");
                    drawChessBoard();
                    drawBoardState();
                    drawCharacter(characterPos, isPlayer1Turn ? &character1Img : &character2Img);
                    continue;
                }

                // 检查是否点击了下子按钮
                if (msg.x >= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 && msg.x <= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 + BUTTON_WIDTH &&
                    msg.y >= BOARD_OFFSET + 300 && msg.y <= BOARD_OFFSET + 300 + BUTTON_HEIGHT) {
                    if (board[characterPos.x][characterPos.y] == EMPTY) {
                        board[characterPos.x][characterPos.y] = isPlayer1Turn ? PLAYER1 : PLAYER2;

                        if (isPlayer1Turn && checkForbiddenMove(characterPos.x, characterPos.y)) {
                            showResult(_T("星戴露（黑棋）禁手，玲娜贝儿（白棋）胜！"));
                            gameOver = true;
                        } else if (checkWin(characterPos.x, characterPos.y, isPlayer1Turn ? PLAYER1 : PLAYER2)) {
                        	if (isPlayer1Turn == 0)
                        	{
                        		showResult(_T("星戴露（黑棋）胜！"));
							}
							else
							{
								showResult(_T("玲娜贝儿（白棋）胜！"));
							}
//                            showResult(isPlayer1Turn ? _T("黑棋胜！") : _T("白棋胜！"));
                            gameOver = true; 
                        } else {
                            isPlayer1Turn = !isPlayer1Turn;
                        }

                        drawChessBoard();
                        drawBoardState();
                        drawCharacter(characterPos, isPlayer1Turn ? &character1Img : &character2Img);
                    }
                    continue;
                }

                // 处理棋盘点击
                if (clickX >= 0 && clickX < BOARD_SIZE && clickY >= 0 && clickY < BOARD_SIZE) {
                    characterPos.x = clickX;
                    characterPos.y = clickY;

                    drawChessBoard();
                    drawBoardState();
                    drawCharacter(characterPos, isPlayer1Turn ? &character1Img : &character2Img);
                }
            }
        }

        if (_kbhit()) {
            _getch();
        }
    }
    
    while(1){
    	if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // 按下ESC键退出
                break;
            }
        }
	}

    closegraph(); // 关闭图形窗口
    return 0;
}

