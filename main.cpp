/* 
 * �ڶ�����ҵ ������ 
 * �汾��1.0
 * ���ߣ�2022303251 ����� 
 * ʱ�䣺20240508 
 */
#include <graphics.h>
#include <conio.h>

#define BOARD_SIZE 15 // ���̵Ĵ�С�������̵����������� 
#define CELL_SIZE 40 // ÿ�����̸��ӵĴ�С�������ӵĿ�Ⱥ͸߶ȣ����أ� 
#define BOARD_OFFSET 20 // ��������ڻ�����Ե��ƫ����������ȷ�������ڻ����е�λ�ã����أ� 

void drawChessBoard() {
    setbkcolor(RGB(249, 214, 91)); // ���ñ���ɫΪǳ��ɫ
    cleardevice(); // ��ջ���

    // �������̱���ͼƬ
    IMAGE img;
    loadimage(&img, _T("src/back.jpg"), BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    putimage(BOARD_OFFSET, BOARD_OFFSET, &img);

    // ������������
    setlinecolor(RGB(0, 0, 0)); // ����������ɫΪ��ɫ
    for (int i = 0; i <= BOARD_SIZE; i++) {
        line(BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + BOARD_SIZE * CELL_SIZE, BOARD_OFFSET + i * CELL_SIZE); // ���ƺ���
        line(BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + BOARD_SIZE * CELL_SIZE); // ��������
    }
}

int main() {
    initgraph(BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2, BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2); // ��ʼ������

    drawChessBoard(); // ��������

    getch(); // �ȴ��û������˳�
    closegraph(); // �رջ���

    return 0;
}

