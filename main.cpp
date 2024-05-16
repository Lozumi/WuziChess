/*
 * ���Ĵ���ҵ ������v5
 * �汾��5.0
 * ���ߣ�2022303251 �����
 * ʱ�䣺20240516
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
    settextstyle(20, 0, _T("����"));
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 50, 20, _T("�Ǵ�¶����������"));
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, 60, _T("��������Ϸ"));
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 110, 100, _T("V5.0"));
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, 140, _T("���������"));
}

void drawButton(int x, int y, TCHAR text[]) {
    setfillcolor(RGB(100, 149, 237)); // ǳ��ɫ
    solidrectangle(x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    settextcolor(RGB(255, 255, 255)); // ��ɫ
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("����")); // �޸�������ַ�������
    int textWidth = textwidth(text);
    int textHeight = textheight(text);
    outtextxy(x + (BUTTON_WIDTH - textWidth) / 2, y + (BUTTON_HEIGHT - textHeight) / 2, text);
}

void drawChessBoard() {
    setbkcolor(RGB(249, 214, 91)); // ���ñ���ɫΪǳ��ɫ
    cleardevice(); // ��ջ���

    // �������̱���ͼƬ
    IMAGE boardImg;
    loadimage(&boardImg, _T("src/back.jpg"), BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    putimage(BOARD_OFFSET, BOARD_OFFSET, &boardImg);

    // ������������
    setlinecolor(RGB(0, 0, 0)); // ����������ɫΪ��ɫ
    for (int i = 0; i < BOARD_SIZE; i++) {
        line(BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + (BOARD_SIZE - 1) * CELL_SIZE, BOARD_OFFSET + i * CELL_SIZE); // ���ƺ���
        line(BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET, BOARD_OFFSET + i * CELL_SIZE, BOARD_OFFSET + (BOARD_SIZE - 1) * CELL_SIZE); // ��������
    }

    // ���Ʊ���
    drawTitle();

    // ���ư�ť
    drawButton(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, BOARD_OFFSET + 180, _T("�������"));
    drawButton(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, BOARD_OFFSET + 240, _T("��ȡ���"));
    drawButton(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80, BOARD_OFFSET + 300, _T("����"));
}

void drawAlpha(IMAGE* picture, int picture_x, int picture_y) {
    // ������ʼ��
    DWORD *dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD *draw = GetImageBuffer();
    DWORD *src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++) {
        for (int ix = 0; ix < picture_width; ix++) {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
            }
        }
    }
}

void drawCharacter(Position pos, IMAGE *characterImg) {
    //���������ϵ����� 
    int x = BOARD_OFFSET + pos.x * CELL_SIZE - CELL_SIZE / 2;
    int y = BOARD_OFFSET + pos.y * CELL_SIZE - CELL_SIZE / 2;
    drawAlpha(characterImg, x, y);
}

void drawBoardState() {
    // ���������ϵ���������
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
        printf("�������ʧ��\n");
    }
}

void loadBoardState(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fread(board, sizeof(board), 1, file);
        fclose(file);
    } else {
        printf("��ȡ���ʧ��\n");
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
    // ����Ƿ��γɽ���
	// ˫�����֣�Double Three������һ��λ�����Ӻ�ͬʱ�γ��������������������������������������������飩��
	// ˫�Ľ��֣�Double Four������һ��λ�����Ӻ�ͬʱ�γ��������ģ��������������������������ĸ������飩��
	// �������֣�Overline������һ��λ�����Ӻ��γɳ������ӵ����飨�����ӻ�������������ӣ���
    if (isDoubleThree(x, y) || isDoubleFour(x, y) || isOverline(x, y)) {
        return true;
    }
    return false;
}

void showResult(TCHAR result[]) {
    settextstyle(40, 0, _T("����"));
    settextcolor(RGB(255, 0, 0));
    setbkmode(TRANSPARENT);
    outtextxy(BOARD_OFFSET + BOARD_SIZE * CELL_SIZE / 2 - 100, BOARD_OFFSET + BOARD_SIZE * CELL_SIZE / 2 - 20, result);
}

int main() {
    initgraph(BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2 + 220, BOARD_SIZE * CELL_SIZE + BOARD_OFFSET * 2); // ��ʼ������

    IMAGE character1Img, character2Img;
    loadimage(&character1Img, _T("src/character1.png"), CELL_SIZE, CELL_SIZE); // ��������1��ͼ
    loadimage(&character2Img, _T("src/character2.png"), CELL_SIZE, CELL_SIZE); // ��������2��ͼ

    Position characterPos = {7, 7}; // ��ʼ������λ������������

    drawChessBoard(); // ��������
    drawCharacter(characterPos, isPlayer1Turn ? &character1Img : &character2Img); // ��������
    drawBoardState(); // �������

    bool gameOver = false;

    while (!gameOver) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg(); // ��ȡ�����Ϣ
            if (msg.uMsg == WM_LBUTTONDOWN) {
                int clickX = (msg.x - BOARD_OFFSET + CELL_SIZE / 2) / CELL_SIZE;
                int clickY = (msg.y - BOARD_OFFSET + CELL_SIZE / 2) / CELL_SIZE;

                // ����Ƿ����˱��水ť
                if (msg.x >= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 && msg.x <= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 + BUTTON_WIDTH &&
                    msg.y >= BOARD_OFFSET + 180 && msg.y <= BOARD_OFFSET + 180 + BUTTON_HEIGHT) {
                    saveBoardState("board_state.dat");
                    continue;
                }

                // ����Ƿ����˶�ȡ��ť
                if (msg.x >= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 && msg.x <= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 + BUTTON_WIDTH &&
                    msg.y >= BOARD_OFFSET + 240 && msg.y <= BOARD_OFFSET + 240 + BUTTON_HEIGHT) {
                    loadBoardState("board_state.dat");
                    drawChessBoard();
                    drawBoardState();
                    drawCharacter(characterPos, isPlayer1Turn ? &character1Img : &character2Img);
                    continue;
                }

                // ����Ƿ��������Ӱ�ť
                if (msg.x >= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 && msg.x <= BOARD_OFFSET + BOARD_SIZE * CELL_SIZE + 80 + BUTTON_WIDTH &&
                    msg.y >= BOARD_OFFSET + 300 && msg.y <= BOARD_OFFSET + 300 + BUTTON_HEIGHT) {
                    if (board[characterPos.x][characterPos.y] == EMPTY) {
                        board[characterPos.x][characterPos.y] = isPlayer1Turn ? PLAYER1 : PLAYER2;

                        if (isPlayer1Turn && checkForbiddenMove(characterPos.x, characterPos.y)) {
                            showResult(_T("�Ǵ�¶�����壩���֣����ȱ��������壩ʤ��"));
                            gameOver = true;
                        } else if (checkWin(characterPos.x, characterPos.y, isPlayer1Turn ? PLAYER1 : PLAYER2)) {
                        	if (isPlayer1Turn == 0)
                        	{
                        		showResult(_T("�Ǵ�¶�����壩ʤ��"));
							}
							else
							{
								showResult(_T("���ȱ��������壩ʤ��"));
							}
//                            showResult(isPlayer1Turn ? _T("����ʤ��") : _T("����ʤ��"));
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

                // �������̵��
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
            if (ch == 27) { // ����ESC���˳�
                break;
            }
        }
	}

    closegraph(); // �ر�ͼ�δ���
    return 0;
}

