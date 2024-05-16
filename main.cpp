/* 
 * 第二次作业 五子棋 
 * 版本：1.0
 * 作者：2022303251 刘黎可 
 * 时间：20240508 
 */
#include <graphics.h>
#include <conio.h>

#define BOARD_SIZE 15 // 棋盘的大小，即棋盘的行数和列数 
#define CELL_SIZE 40 // 每个棋盘格子的大小，即格子的宽度和高度（像素） 
#define BOARD_OFFSET 20 // 棋盘相对于画布边缘的偏移量，用来确定棋盘在画布中的位置（像素） 

void drawChessBoard() {
    setbkcolor(RGB(249, 214, 91)); // 设置背景色为浅黄色
    cleardevice(); // 清空画布

    // 绘制棋盘背景图片
    IMAGE img;
    loadimage(&img, _T("src/back.jpg"), BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    putimage(BOARD_OFFSET, BOARD_OFFSET, &img);

    // 绘制棋盘网格
    setlinecolor(RGB(0, 0, 0)); // 设置线条颜色为黑色
    for (int i = 0; i <= BOARD_SIZE; i++) {
        line(BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + BOARD_SIZE * CELL_SIZE, BOARD_OFFSET + i * CELL_SIZE); // 绘制横线
        line(BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + BOARD_SIZE * CELL_SIZE); // 绘制竖线
    }
}

int main() {
    initgraph(BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2, BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2); // 初始化画布

    drawChessBoard(); // 绘制棋盘

    getch(); // 等待用户按键退出
    closegraph(); // 关闭画布

    return 0;
}

