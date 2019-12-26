#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
#include<math.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

#define MAP_ADJ_X 10
#define MAP_ADJ_Y 5

void gotoxy(int x, int y) { //����ͻ��� x,y ��ǥ�� �̵���Ű�� �Լ� 
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c) { //Ŀ�� ǥ�ø� �ٲ��ִ� �Լ�  
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int board[4][4] = { // ������ 
{0,0,0,0},
{0,0,0,0},
{0,0,0,0},
{0,0,0,0}
};

int game_over_on = 0; //���� ���� ����ġ 
int score; //���� �����Լ� 

int made_32; // �� �ش��ϴ� ��ȣ�� ����� ��������� �޼��� ����ֱ� ���� flag��.. 
int made_64;
int made_128;
int made_256;
int made_512;
int made_1024;
int made_2048;
int made_4096;

void reset(void); //������ �ʱ�ȭ 
void draw(void) {
	int i, j, k; // for���� ������ ���� ���� 
	int num; // �������� ���� ȭ���� ĭ�� ǥ���ϱ� ���� ������ �˻��ϰ� �Ǵµ� 
			 // 10000�� ������ ���,  �̰��� �ӽ÷� �� ���ڸ� �����ϴ� ���� 
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
			if (board[i][j] == 0) { //���� 0�϶� �޼��� ���flag�� ���� �ٸ� ����� ������ 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
				printf("      ");
				gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
				printf("  ��  ");
				gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
				printf("      ");
			}
			else {
				num = ((board[i][j] > 10000) ? board[i][j] - 10000 : board[i][j]); //10000�� ������ ��� ���� 
				if (game_over_on == 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				else { //���ӿ����� ��쿡�� ȸ������ 
					for (k = 0; k < 12;) if ((int)pow(2, ++k) == num) break; //���� ���� �ٸ� ������ ����
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);//���� ���� 
				}
				gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
				printf("��������");
				gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
				printf(" %4d ", num);
				gotoxy(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
				printf("��������");
				if (game_over_on == 1) Sleep(200);  //���ӿ����� ��� ��ĭ �׸��� 0.2���� ������ 
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); //���� ����� ��ħ 

	gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y + 11);
	printf("Score : %6d \n", score); //����ǥ�� 
	Sleep(50);
} // �������� �׸� 
void new_num(void); // ���ο� ���� 2�� 4�� �����ǿ� ���� 
int get_key(void); // Ű�Է¹ް� �����̴� �Լ� 
void check_2048(void); // ����� ��������� �޼��� ����ִ� �Լ� 
void check_game_over(void); // ���ӿ����� �Ǵ��ϴ� �Լ� 

main() {
	setcursortype(NOCURSOR); //���콺 Ŀ���� ����� 

	reset(); //������ �ʱ�ȭ 

	while (1) {
		if (get_key()) { //Ű �Է��� �������� �ִ� ��쿡�� 
			check_2048(); // ���üũ 
			new_num();    //���ο� ���ڸ� ���� 
		}
		check_game_over(); // ���ӿ����Ǵ� 
	}
}

void reset(void) {
	int i, j; // for���� ������ ������ 

	system("cls"); //ȭ���� ���� 

	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) board[i][j] = 0; //�������� �ʱ�ȭ 

	//�������� �ʱ�ȭ 
	score = 0;
	game_over_on = 0;

	made_32 = 0;
	made_64 = 0;
	made_128 = 0;
	made_256 = 0;
	made_512 = 0;
	made_1024 = 0;
	made_2048 = 0;
	made_4096 = 0;

	//����� �׸�     
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y - 3);
	printf(" GAME 2048  ");
	
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y - 1);
	printf("������������������������������������������������������");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 1);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 2);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 3);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 4);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 5);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 6);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 7);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 8);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 9);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 10);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 11);
	printf("��                         ��");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 12);
	printf("������������������������������������������������������");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 14);
	printf(" �� ��,��,��,�� : Move");
	gotoxy(MAP_ADJ_X - 2, MAP_ADJ_Y + 16);
	printf(" �� ESC : Quit");
	

	//���ο� ���ڸ� 2�� ���� 
	new_num();
	new_num();
}

int get_key(void) {
	int i, j;

	int key = 0; //�Է¹��� Ű�� �����ϴ� ���� 
	int act = 0; //�Է¹��� Ű�� ��ȿ�� �ۿ��� ������ �Լ��� ���� return�ϴ� ���� ���� ���� 
	int mov = -1; //�Է¹��� Ű�� ��ȿ�� �ۿ��� ������ �ݺ����� Ż���Ҽ� �ְ� �ϴ� ���� 

	gotoxy(0, 24); //ESCŰ�� �������� ȭ�� ���� �Ʒ��� ���Ṯ���� ���� ���� Ŀ�� �̵� 
	key = _getch(); //Ű�Է¹��� 
	if (key == ESC) {
		gotoxy(0, 21); //ȭ�� ���� �Ʒ��� ���Ṯ���� ���� ���� Ŀ�� �̵� 
		exit(0); // ESCŰ�� ������ �������� 
	}
	if (key == 0xE0 || key == 0) {
		key = _getch();

		switch (key) {
		case LEFT:     //���ʹ���Ű�� ������� 
			for (; mov != 0;) {     //�Լ� ����� mov���� -1�̹Ƿ� ó���� �ݵ�� ����ǰ� ������ 0���� ����, 
				mov = 0;         //���� �����ǿ� ��ȭ�� �ִ� ��쿡 ��ġ �����Ͽ� �����ǿ� ��ȭ�� ���������� �ݺ� 
				for (i = 0; i < 4; i++) {  // ������� �˻� 
					for (j = 1; j <= 3; j++) { //���� ���ʿ�(0��°��)�� �˻��� �ʿ䰡 �����Ƿ� 1~3������ ������� �˻� 
						if (board[i][j] == 0 || board[i][j] > 10000) continue; //�ڱ� ���� 0 Ȥ�� 10000���� ū ���,
						if (board[i][j - 1] != 0 && board[i][j - 1] != board[i][j]) continue;
						//�ڱ� ���ʰ��� 0�� �ƴϰ� �ڽŰ� �ٸ� ��� �ٽ� �˻�(j++�� �����)
						if (board[i][j - 1] == 0) board[i][j - 1] = board[i][j]; //�ڱ� ���ʰ��� 0�ΰ�� �ڱⰪ ���� 
						else if (board[i][j] == board[i][j - 1]) { //�ڱ� ���ʰ��� ������ ��쿡 
							board[i][j - 1] *= 2; //���ʰ��� 2���(�ڱ� ���� ���ϴ°Ͱ� ����) 
							board[i][j - 1] += 10000; // �ӽ÷� ������ �Լ� �������� �ٽ� ���� ���ҽ�Ŵ 
							score += 2 * (board[i][j]); //������ ����     
						}
						board[i][j] = 0; //�ڱ� ���� 0���� ���� 
						act++; // action�� ������ �˸�(get_key �Լ� ����� �Ǵܵ�) 
						mov++; // mov�� ������ �˸�(for���ȿ����� ���ǰ� ���µ�) 
					}   /* 10000�� �ȴ��ϸ� ���� ���ٿ� ���� 2,2,2,2 �� ��� ����Ű�� ������ �ѹ���       */
				}       /* 8,0,0,0���� �ٲ�.. 10000�� ���ϰ�, ���� 10000���� ū ��� ������ ���� �ʰ� �Ͽ�*/
						/* 2,2,2,2 ���� 4,4,0,0���� �ٲ� */
				if (mov != 0) draw();// ������ ��ȭ�� �ִ� ��� 
			}                      // ���� �ٱ� for���� �ѹ� �������� ȭ���� �׷���(�ִϸ��̼� ȿ��) 
			break;

		case RIGHT: // ������ ����Ű�� �������. �۵������� ���� �¿츸 �ٲ� 
			for (; mov != 0;) {
				mov = 0;
				for (j = 2; j >= 0; j--) { //2����, 1����, 0������ ������� �˻�. 
					for (i = 0; i < 4; i++) {  // ������� �˻�
						if (board[i][j] == 0 || board[i][j] > 10000) continue;
						if (board[i][j + 1] != 0 && board[i][j + 1] != board[i][j]) continue;
						if (board[i][j + 1] == 0) board[i][j + 1] = board[i][j];
						else if (board[i][j] < 10000 && board[i][j] == board[i][j + 1]) {
							board[i][j + 1] *= 2;
							board[i][j + 1] += 10000;
							score += 2 * (board[i][j]);
						}
						board[i][j] = 0;
						mov++;
						act++;
					}
				}
				if (mov != 0) draw();
			}
			break;

		case UP: // ���� ����Ű�� �������. �۵������� ���� 
			for (; mov != 0;) {
				mov = 0;
				for (i = 1; i <= 3; i++) { //1����, 2����, 3������ ������� �˻� 
					for (j = 0; j < 4; j++) { //��� ���� �˻� 
						if (board[i][j] == 0 || board[i][j] > 10000) continue;
						if (board[i - 1][j] != 0 && board[i - 1][j] != board[i][j]) continue;
						if (board[i - 1][j] == 0) board[i - 1][j] = board[i][j];
						else if (board[i][j] < 10000 && board[i][j] == board[i - 1][j]) {
							board[i - 1][j] *= 2;
							board[i - 1][j] += 10000;
							score += 2 * (board[i][j]);
						}
						board[i][j] = 0;
						act++;
						mov++;
					}
				}
				if (mov != 0) draw();
			}
			break;

		case DOWN: // �Ʒ��� ����Ű�� �������. �۵������� ����
			for (; mov != 0;) {
				mov = 0;
				for (i = 2; i >= 0; i--) { //2����, 1����, 0������ ������� �˻�  
					for (j = 0; j < 4; j++) { //��� ���� �˻�  
						if (board[i][j] == 0 || board[i][j] > 10000) continue;
						if (board[i + 1][j] != 0 && board[i + 1][j] != board[i][j]) continue;
						if (board[i + 1][j] == 0) board[i + 1][j] = board[i][j];
						else if (board[i][j] < 10000 && board[i][j] == board[i + 1][j]) {
							board[i + 1][j] *= 2;
							board[i + 1][j] += 10000;
							score += 2 * (board[i][j]);
						}
						board[i][j] = 0;
						act++;
						mov++;
					}
				}
				if (mov != 0) draw();
			}
			break;
		}
	}

	for (i = 0; i < 4; i++) { //�ӽ÷� �������״� 10000�� ����. 
		for (j = 0; j < 4; j++) {
			if (board[i][j] > 10000)board[i][j] -= 10000;
		}
	}

	if (act == 0) return 0; //���� Ű�Է��� ��ȭ�� ������ 0�� return 
	else return 1;  //Ű�Է��� ��ȭ�� ������ 1�� return 
}

void new_num() {
	int i, j; //for���� ������ ����

	int cnt; //�����ǿ� ��ĭ�� ���� �����ϴ� ���� 
	int rdm; //����ǥ�� ���� ������ ���� �����ϴ� ���� 
	int n_num; // ���ο� ���� 2, 4�� �����ϴ� ���� 
	int* p0[16] = { 0 }; //��ĭ�� ��ġ�� ����� ������ �迭 

	for (i = 0, cnt = 0; i < 4; i++) { //��ü �������� �˻�
		for (j = 0; j < 4; j++) {
			if (board[i][j] == 0) { //��ĭ�� ������ 
				p0[cnt++] = &board[i][j]; //�� ��ġ�� ������ �迭�� ����, cnt�� ���� 
			}
		}
	}

	srand(time(0)); //���� ���� 
	rdm = rand() % (cnt); //������ �迭cnt������ ���� ������ ����. 
	n_num = (rand() % 100 < 80) ? 2 : 4; //80%Ȯ���� 2�� �����, 20%Ȯ���� 4�� ���� 

	*p0[rdm] = n_num; //��ĭ�� ���� ���� 
	draw();
	*p0[rdm] = 0;        //�ִϸ��̼� ȿ�� �ι������� 
	draw();
	*p0[rdm] = n_num;
	draw();
}

void check_2048(void) { //32������ ���ο� ū ���� ó�� ��������� �޼��� ǥ��. 
	int i, j, k;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (board[i][j] == 32 && made_32 == 0) {
				made_32 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("Yon made 32 : ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("Good job!");
					Sleep(100);
				}
				gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y + 2);
				printf("Keep play to make 2048");
			}
			if (board[i][j] == 64 && made_64 == 0) {
				made_64 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("Yon made 64 : ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("Great!   ");
					Sleep(100);
				}
			}
			if (board[i][j] == 128 && made_128 == 0) {
				made_128 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					printf("Yon made 128 : ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("Hooray!!");
					Sleep(100);
				}
			}
			if (board[i][j] == 256 && made_256 == 0) {
				made_256 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("Yon made 256 : ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("I believe you can do it!");
					Sleep(100);
				}
			}
			if (board[i][j] == 512 && made_512 == 0) {
				made_512 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf("Yon made 512 : ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("awesome!! Keep going!!");
					Sleep(100);
				}
			}
			if (board[i][j] == 1024 && made_1024 == 0) {
				made_1024 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf("Yon made 1024!! : ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("Almost done!!");
					Sleep(100);
				}
			}
			if (board[i][j] == 2048 && made_2048 == 0) {
				made_2048 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					printf("Congratulations! Yon made 2048 !!");
					Sleep(100);
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y + 2);
				printf("You can keep play for a high score;)");
			}
			if (board[i][j] == 4096 && made_4096 == 0) {
				made_4096 = 1;
				for (k = 0; k < 3; k++) {
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					printf("                                        ");
					Sleep(100);
					gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
					printf("Yon made 4086 !!! : I'm impressed    ");
					Sleep(100);
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y + 2);
				printf("You can keep play for a high score;)");
			}
		}
	}
}

void check_game_over(void) { //���ӿ������� üũ 
	int i, j;

	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) if (board[i][j] == 0) return; //�ϴ� ��ĭ�� ������ ���ӿ����� �ƴ� 
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1]) return;
	//0,0���� 2,2���� �����ʰ� �Ʒ��� �˻� 
	for (i = 0; i < 3; i++) if (board[i][3] == board[i + 1][3]) return; //0~2������ �Ʒ��� �˻�(�����ʰ��� �����Ƿ�) 
	for (j = 0; j < 3; j++) if (board[3][j] == board[3][j + 1]) return; //0~2������ �����ʸ� �˻�(�Ʒ��ʰ��� �����Ƿ�) 

	game_over_on = 1;    //���ӿ��� flag�� on. draw�Լ����� �ִϸ��̼� ȿ���� �ֱ� ���� 

	gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y);
	printf("Game Over..                             ");
	gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y + 2);
	printf("                                        ");
	draw();
	gotoxy(MAP_ADJ_X + 28, MAP_ADJ_Y + 2);
	printf("Restart? (Y/N:quit)                     "); // ������ �ٽ��Ұ��� ��� 

	while (1) {
		switch (_getch()) {
		case 121: // 'Y'
		case 89: // 'y'
			reset(); //������ �ʱ�ȭ  
			break;
		case 110: // 'N'
		case 78: //'n'
		case ESC:
			gotoxy(0, 21); //ȭ�� ���� �Ʒ��� ���Ṯ���� ���� ���� Ŀ�� �̵� 
			exit(0);
		}
		break;
	}
}