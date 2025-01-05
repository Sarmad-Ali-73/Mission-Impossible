#pragma comment(lib, "winmm.lib")

#include<iostream>
#include<iomanip>
#include<conio.h>
#include<cstdlib> 
#include <cstdio>
#include<sstream> 
#include<cctype>
#include<Windows.h>
#include<ctime> 
#include<fstream>
#include<string>

using namespace std;

const int ROW = 30, COL = 50;
char map[ROW][COL], bullet_input, b_direction, b2_direction, b3_direction, b4_direction, b5_direction, b6_direction;
int ethan_hunt_RP = 0, ethan_hunt_CP = 0, level = 1, life = 3, score = 0, soloman_RP = 20, soloman_CP = 20, bullet_RP, bullet_CP, enemy_RP, enemy_CP, E1_RP, E1_CP, E2_RP, E2_CP, E3_RP, E3_CP, E4_RP, E4_CP, E5_RP, E5_CP, E6_RP, E6_CP, choice;
int obs_num, enemy_num, speed, enemyb = 1, E1bullet_CP, E1bullet_RP, E2bullet_CP, E2bullet_RP, E3bullet_RP, E3bullet_CP, E4bullet_RP, E4bullet_CP, E5bullet_RP, E5bullet_CP, E6bullet_RP, E6bullet_CP, missile_RP, missile_CP, rand_no, target = 0;
bool flag = true, loop = true, isnot_active = true, is_active1 = false, is_active2 = false, is_active3 = false, is_active4 = false, is_active5 = false, is_active6 = false, play = true, fire = false, firE1 = true, firE2 = true, firE3 = true, firE4 = true, firE5 = true, firE6 = true;
bool killed_E1 = false, killed_E2 = false, killed_E3 = false, killed_E4 = false, killed_E5 = false, killed_E6 = false, missile_active = false, initial = true, random = true;
//file handling variables
const string fileName = "highscores.txt";
string newName;
string names[10];
int scores[10];


void hideCursor();
void showCursor();
void moveCursorToTop();

void map1();
void initialize_ethan();
void enemy();
void movement_ethan_hunt(char input);
void ethan_hunt();
void obstacles();
void print_map();
void soloman();
void movement_soloman_lane();
void space_ethan();
void space_soloman();
void enemy_movement();
void space_E1();
void space_E2();
void space_E3();
void space_E4();
void space_E5();
void space_E6();
void enemy_initialize();
bool ethan_overlaps_soloman();
void input_check(char input);
void bullet_direction(char bullet_input);
void print_bullet();
void bullet_movement();
void scoreboard();
void win_message();
void menu();
void bye();
void instructions();
void scoreleader();
void exit_doors();
void level_check();
void level_upgrade();
void level_initialization();
void missile();
void enemy_bullet();
void Ebullet_move();
void Ebullet_print();
void name();
void play_againF();
void display_score();
void kill();

int main() {
	char input, back, play_again;
	hideCursor();
	srand(time(0));

	while (loop) {
		menu();
		system("cls");
		switch (choice) {
		case 1:
			while (loop) {
				name();
				level_upgrade();
				map1();
				enemy_initialize();
				initialize_ethan();
				obstacles();
				soloman();
				exit_doors();
				print_map();
				PlaySound(TEXT("buzzer_x"), NULL, SND_FILENAME | SND_ASYNC);
				while (play) {
					if (target >= 10) {
						cout << "\t\t\t\t\t\t\tMissile Ready...press Sapce to LAUNCH!";
					}

					if (_kbhit())   // checks if the key is pressed on the keyboard
					{
						input = _getch();
						input = tolower(input);
						input_check(input);
						level_check();
						ethan_hunt();
					}
					if (fire) {
						bullet_movement();
						print_bullet();
					}

					movement_soloman_lane();
					enemy_movement();
					enemy();

					enemy_bullet();
					Ebullet_move();
					if (missile_active)
						missile();
					Ebullet_print();
					print_map();
					scoreboard();
					Sleep(speed);  // increase the value to reduce speed & will take more time between actions
				}


				win_message();
				scoreleader();
				cout << "\t\t\t\t\t\t\tDo You Want to Play Again? (Y/N) ";
			play_agaiN:
				play_again = _getch();
				play_again = tolower(play_again);
				if (play_again == 'y') {
					play_againF();
					continue;
				}
				else if (play_again == 'n')
					break;
				else
					goto play_agaiN;
				break;
			}
			break;
		case 2:
			system("cls");
			instructions();
			break;
		case 3:
			system("cls");
			display_score();
			break;
		case 4:
			system("cls");
			bye();
			loop = false;
			break;
		default:
			system("cls");
			continue;
		}
		if (loop) {
			cout << endl << "\t\t\t\t\t\t\tDo You Want to go back to Main Menu? (Y/N) ";
		bacK:
			back = _getch();
			back = tolower(back);

			if (back == 'y') {
				play_againF();
				continue;
			}
			else if (back == 'n') {
				system("cls");
				bye();
				break;
			}
			else
				goto bacK;
			break;
		}

	}

	showCursor();
	cout << endl;
	system("pause");
	return 0;
}

void input_check(char input) {
	if (input == -32) {			//activating special character
		input = _getch();
		movement_ethan_hunt(input);
	}
	else if (input == 'w' || input == 's' || input == 'a' || input == 'd') {
		fire = true;
		if (isnot_active) {
			isnot_active = false;
			bullet_input = input;
			bullet_direction(bullet_input);
		}
	}
	else if (input == 32 && !missile_active && target >= 10 ) {
		PlaySound(TEXT("missile"), NULL, SND_FILENAME | SND_ASYNC);
		target = 0;
		missile_active = true;
	}
}


/// ////////////////////// ENEMY FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////
void enemy_initialize() {
	enemyb = 1;
	for (int i = 1; i <= enemy_num; i++)
	{
		int start = 2, end1 = 14 - 2;
		int end2 = COL - 5;
		enemy_RP = start + rand() % (end1 - start + 1);
		enemy_CP = start + rand() % (end2 - start + 1);
		if (map[enemy_RP][enemy_CP] == ' ' &&
			map[enemy_RP][enemy_CP - 1] == ' ' &&
			map[enemy_RP][enemy_CP + 1] == ' ' &&
			map[enemy_RP - 1][enemy_CP - 1] == ' ' &&
			map[enemy_RP - 1][enemy_CP + 1] == ' ' &&
			map[enemy_RP - 1][enemy_CP] == ' ')
		{
			if (enemyb == 1) {
				E1_RP = enemy_RP;
				E1_CP = enemy_CP;

				map[E1_RP][E1_CP] = 'V';
				map[E1_RP][E1_CP - 1] = '[';
				map[E1_RP][E1_CP + 1] = ']';
				map[E1_RP - 1][E1_CP - 1] = '[';
				map[E1_RP - 1][E1_CP + 1] = ']';
				map[E1_RP - 1][E1_CP] = '|';
			}
			else if (enemyb == 2) {
				E2_RP = enemy_RP;
				E2_CP = enemy_CP;

				map[E2_RP][E2_CP] = 'V';
				map[E2_RP][E2_CP - 1] = '[';
				map[E2_RP][E2_CP + 1] = ']';
				map[E2_RP - 1][E2_CP - 1] = '[';
				map[E2_RP - 1][E2_CP + 1] = ']';
				map[E2_RP - 1][E2_CP] = '|';
			}
			else if (enemyb == 3) {
				E3_RP = enemy_RP;
				E3_CP = enemy_CP;

				map[E3_RP][E3_CP] = 'V';
				map[E3_RP][E3_CP - 1] = '[';
				map[E3_RP][E3_CP + 1] = ']';
				map[E3_RP - 1][E3_CP - 1] = '[';
				map[E3_RP - 1][E3_CP + 1] = ']';
				map[E3_RP - 1][E3_CP] = '|';
			}
			else if (enemyb == 4) {
				E4_RP = enemy_RP;
				E4_CP = enemy_CP;

				map[E4_RP][E4_CP] = 'V';
				map[E4_RP][E4_CP - 1] = '[';
				map[E4_RP][E4_CP + 1] = ']';
				map[E4_RP - 1][E4_CP - 1] = '[';
				map[E4_RP - 1][E4_CP + 1] = ']';
				map[E4_RP - 1][E4_CP] = '|';
			}
			else if (enemyb == 5) {
				E5_RP = enemy_RP;
				E5_CP = enemy_CP;

				map[E5_RP][E5_CP] = 'V';
				map[E5_RP][E5_CP - 1] = '[';
				map[E5_RP][E5_CP + 1] = ']';
				map[E5_RP - 1][E5_CP - 1] = '[';
				map[E5_RP - 1][E5_CP + 1] = ']';
				map[E5_RP - 1][E5_CP] = '|';
			}
			else if (enemyb == 6) {
				E6_RP = enemy_RP;
				E6_CP = enemy_CP;

				map[E6_RP][E6_CP] = 'V';
				map[E6_RP][E6_CP - 1] = '[';
				map[E6_RP][E6_CP + 1] = ']';
				map[E6_RP - 1][E6_CP - 1] = '[';
				map[E6_RP - 1][E6_CP + 1] = ']';
				map[E6_RP - 1][E6_CP] = '|';
			}
			enemyb++;
		}
		else
			i--;
	}



}
void enemy() {
	if (!killed_E1) {
		// printing Enemy 1
		if (map[E1_RP][E1_CP] == ' ' &&
			map[E1_RP][E1_CP - 1] == ' ' &&
			map[E1_RP][E1_CP + 1] == ' ' &&
			map[E1_RP - 1][E1_CP - 1] == ' ' &&
			map[E1_RP - 1][E1_CP + 1] == ' ' &&
			map[E1_RP - 1][E1_CP] == ' '
			)
		{
			map[E1_RP][E1_CP] = 'V';
			map[E1_RP][E1_CP - 1] = '[';
			map[E1_RP][E1_CP + 1] = ']';
			map[E1_RP - 1][E1_CP - 1] = '[';
			map[E1_RP - 1][E1_CP + 1] = ']';
			map[E1_RP - 1][E1_CP] = '|';
		}
	}
	if (!killed_E2) {
		// printing Enemy 2
		if (map[E2_RP][E2_CP] == ' ' &&
			map[E2_RP][E2_CP - 1] == ' ' &&
			map[E2_RP][E2_CP + 1] == ' ' &&
			map[E2_RP - 1][E2_CP - 1] == ' ' &&
			map[E2_RP - 1][E2_CP + 1] == ' ' &&
			map[E2_RP - 1][E2_CP] == ' ')
		{
			map[E2_RP][E2_CP] = 'V';
			map[E2_RP][E2_CP - 1] = '[';
			map[E2_RP][E2_CP + 1] = ']';
			map[E2_RP - 1][E2_CP - 1] = '[';
			map[E2_RP - 1][E2_CP + 1] = ']';
			map[E2_RP - 1][E2_CP] = '|';
		}
	}
	if (!killed_E3) {
		// printing Enemy 3
		if (map[E3_RP][E3_CP] == ' ' &&
			map[E3_RP][E3_CP - 1] == ' ' &&
			map[E3_RP][E3_CP + 1] == ' ' &&
			map[E3_RP - 1][E3_CP - 1] == ' ' &&
			map[E3_RP - 1][E3_CP + 1] == ' ' &&
			map[E3_RP - 1][E3_CP] == ' ')
		{
			map[E3_RP][E3_CP] = 'V';
			map[E3_RP][E3_CP - 1] = '[';
			map[E3_RP][E3_CP + 1] = ']';
			map[E3_RP - 1][E3_CP - 1] = '[';
			map[E3_RP - 1][E3_CP + 1] = ']';
			map[E3_RP - 1][E3_CP] = '|';
		}
	}
	if (!killed_E4) {
		// printing Enemy 4
		if (map[E4_RP][E4_CP] == ' ' &&
			map[E4_RP][E4_CP - 1] == ' ' &&
			map[E4_RP][E4_CP + 1] == ' ' &&
			map[E4_RP - 1][E4_CP - 1] == ' ' &&
			map[E4_RP - 1][E4_CP + 1] == ' ' &&
			map[E4_RP - 1][E4_CP] == ' ')
		{
			map[E4_RP][E4_CP] = 'V';
			map[E4_RP][E4_CP - 1] = '[';
			map[E4_RP][E4_CP + 1] = ']';
			map[E4_RP - 1][E4_CP - 1] = '[';
			map[E4_RP - 1][E4_CP + 1] = ']';
			map[E4_RP - 1][E4_CP] = '|';
		}
	}
	if (!killed_E5) {
		// printing Enemy 5
		if (map[E5_RP][E5_CP] == ' ' &&
			map[E5_RP][E5_CP - 1] == ' ' &&
			map[E5_RP][E5_CP + 1] == ' ' &&
			map[E5_RP - 1][E5_CP - 1] == ' ' &&
			map[E5_RP - 1][E5_CP + 1] == ' ' &&
			map[E5_RP - 1][E5_CP] == ' ')
		{
			map[E5_RP][E5_CP] = 'V';
			map[E5_RP][E5_CP - 1] = '[';
			map[E5_RP][E5_CP + 1] = ']';
			map[E5_RP - 1][E5_CP - 1] = '[';
			map[E5_RP - 1][E5_CP + 1] = ']';
			map[E5_RP - 1][E5_CP] = '|';
		}
	}
	if (!killed_E6) {
		// printing Enemy 6
		if (map[E6_RP][E6_CP] == ' ' &&
			map[E6_RP][E6_CP - 1] == ' ' &&
			map[E6_RP][E6_CP + 1] == ' ' &&
			map[E6_RP - 1][E6_CP - 1] == ' ' &&
			map[E6_RP - 1][E6_CP + 1] == ' ' &&
			map[E6_RP - 1][E6_CP] == ' ')
		{
			map[E6_RP][E6_CP] = 'V';
			map[E6_RP][E6_CP - 1] = '[';
			map[E6_RP][E6_CP + 1] = ']';
			map[E6_RP - 1][E6_CP - 1] = '[';
			map[E6_RP - 1][E6_CP + 1] = ']';
			map[E6_RP - 1][E6_CP] = '|';
		}
	}
}
void enemy_movement() {
	int rand_mov = rand() % 2;
	if (rand_mov) {
		//1st enemy
		if (!killed_E1) {
			if (ethan_hunt_RP - 7 != E1_RP &&
				ethan_hunt_RP > E1_RP &&
				map[E1_RP + 1][E1_CP] == ' ' &&
				map[E1_RP + 1][E1_CP - 1] == ' ' &&
				map[E1_RP + 1][E1_CP + 1] == ' ')
			{
				space_E1();
				E1_RP++;
			}
			else if (E1_RP != ethan_hunt_RP + 7 &&
				ethan_hunt_RP < E1_RP &&
				map[E1_RP - 2][E1_CP - 1] == ' ' &&
				map[E1_RP - 2][E1_CP + 1] == ' ' &&
				map[E1_RP - 2][E1_CP] == ' ') {
				space_E1();
				E1_RP--;
			}
			if (ethan_hunt_RP == E1_RP)
			{
				if ((ethan_hunt_CP - 7) - E1_CP >= 7 &&
					ethan_hunt_CP > E1_CP &&
					map[E1_RP][E1_CP + 2] == ' ' &&
					map[E1_RP - 1][E1_CP + 2] == ' ') {
					space_E1();
					E1_CP++;
				}
				else if (E1_CP - (ethan_hunt_CP + 7) >= 7 &&
					ethan_hunt_CP < E1_CP &&
					map[E1_RP][E1_CP - 2] == ' ' &&
					map[E1_RP - 1][E1_CP - 2] == ' ') {
					space_E1();
					E1_CP--;
				}
			}
		}

		//2nd enemy
		if (!killed_E2) {
			if (ethan_hunt_CP - 7 != E2_CP &&
				ethan_hunt_CP > E2_CP &&
				map[E2_RP][E2_CP + 2] == ' ' &&
				map[E2_RP - 1][E2_CP + 2] == ' ') {
				space_E2();
				E2_CP++;
			}
			else if (E2_CP != ethan_hunt_CP + 7 &&
				ethan_hunt_CP < E2_CP &&
				map[E2_RP][E2_CP - 2] == ' ' &&
				map[E2_RP - 1][E2_CP - 2] == ' ')
			{
				space_E2();
				E2_CP--;
			}
			if (ethan_hunt_CP == E2_CP)
			{
				if ((ethan_hunt_RP - 7) - E2_RP >= 7 &&
					ethan_hunt_RP > E2_RP &&
					map[E2_RP + 1][E2_CP] == ' ' &&
					map[E2_RP + 1][E2_CP - 1] == ' ' &&
					map[E2_RP + 1][E2_CP + 1] == ' ')
				{
					space_E2();
					E2_RP++;
				}
				else if (E2_RP - (ethan_hunt_RP + 7) >= 7 &&
					ethan_hunt_RP < E2_RP &&
					map[E2_RP - 2][E2_CP - 1] == ' ' &&
					map[E2_RP - 2][E2_CP + 1] == ' ' &&
					map[E2_RP - 2][E2_CP] == ' ') {
					space_E2();
					E2_RP--;
				}
			}
		}
		//3rd enemy
		if (!killed_E3) {
			if (ethan_hunt_RP - 7 != E3_RP &&
				ethan_hunt_RP > E3_RP &&
				map[E3_RP + 1][E3_CP] == ' ' &&
				map[E3_RP + 1][E3_CP - 1] == ' ' &&
				map[E3_RP + 1][E3_CP + 1] == ' ')
			{
				space_E3();
				E3_RP++;
			}
			else if (E3_RP != ethan_hunt_RP + 7 &&
				ethan_hunt_RP < E3_RP &&
				map[E3_RP - 2][E3_CP - 1] == ' ' &&
				map[E3_RP - 2][E3_CP + 1] == ' ' &&
				map[E3_RP - 2][E3_CP] == ' ') {
				space_E3();
				E3_RP--;
			}
			if (ethan_hunt_RP == E3_RP)
			{
				if ((ethan_hunt_CP - 7) - E3_CP >= 7 &&
					ethan_hunt_CP > E3_CP &&
					map[E3_RP][E3_CP + 2] == ' ' &&
					map[E3_RP - 1][E3_CP + 2] == ' ') {
					space_E3();
					E3_CP++;
				}
				else if (E3_CP - (ethan_hunt_CP + 7) >= 7 &&
					ethan_hunt_CP < E3_CP &&
					map[E3_RP][E3_CP - 2] == ' ' &&
					map[E3_RP - 1][E3_CP - 2] == ' ') {
					space_E3();
					E3_CP--;
				}
			}
		}
		//4th enemy
		if (!killed_E4) {
			if (ethan_hunt_CP - 7 != E4_CP &&
				ethan_hunt_CP > E4_CP &&
				map[E4_RP][E4_CP + 2] == ' ' &&
				map[E4_RP - 1][E4_CP + 2] == ' ') {
				space_E4();
				E4_CP++;
			}
			else if (E4_CP != ethan_hunt_CP + 7 &&
				ethan_hunt_CP < E4_CP &&
				map[E4_RP][E4_CP - 2] == ' ' &&
				map[E4_RP - 1][E4_CP - 2] == ' ')
			{
				space_E4();
				E4_CP--;
			}
			if (ethan_hunt_CP == E4_CP)
			{
				if ((ethan_hunt_RP - 7) - E4_RP >= 7 &&
					ethan_hunt_RP > E4_RP &&
					map[E4_RP + 1][E4_CP] == ' ' &&
					map[E4_RP + 1][E4_CP - 1] == ' ' &&
					map[E4_RP + 1][E4_CP + 1] == ' ')
				{
					space_E4();
					E4_RP++;
				}
				else if (E4_RP - (ethan_hunt_RP + 7) >= 7 &&
					ethan_hunt_RP < E4_RP &&
					map[E4_RP - 2][E4_CP - 1] == ' ' &&
					map[E4_RP - 2][E4_CP + 1] == ' ' &&
					map[E4_RP - 2][E4_CP] == ' ') {
					space_E4();
					E4_RP--;
				}
			}
		}
		//5th enemy
		if (!killed_E5) {
			if (ethan_hunt_RP - 7 != E5_RP &&
				ethan_hunt_RP > E5_RP &&
				map[E5_RP + 1][E5_CP] == ' ' &&
				map[E5_RP + 1][E5_CP - 1] == ' ' &&
				map[E5_RP + 1][E5_CP + 1] == ' ')
			{
				space_E5();
				E5_RP++;
			}
			else if (E5_RP != ethan_hunt_RP + 7 &&
				ethan_hunt_RP < E5_RP &&
				map[E5_RP - 2][E5_CP - 1] == ' ' &&
				map[E5_RP - 2][E5_CP + 1] == ' ' &&
				map[E5_RP - 2][E5_CP] == ' ') {
				space_E5();
				E5_RP--;
			}
			if (ethan_hunt_RP == E5_RP)
			{
				if ((ethan_hunt_CP - 7) - E5_CP >= 7 &&
					ethan_hunt_CP > E5_CP &&
					map[E5_RP][E5_CP + 2] == ' ' &&
					map[E5_RP - 1][E5_CP + 2] == ' ') {
					space_E5();
					E5_CP++;
				}
				else if (E5_CP - (ethan_hunt_CP + 7) >= 7 &&
					ethan_hunt_CP < E5_CP &&
					map[E5_RP][E5_CP - 2] == ' ' &&
					map[E5_RP - 1][E5_CP - 2] == ' ') {
					space_E5();
					E5_CP--;
				}
			}
		}
		//6th enemy
		if (!killed_E6) {
			if (ethan_hunt_CP - 7 != E6_CP &&
				ethan_hunt_CP > E6_CP &&
				map[E6_RP][E6_CP + 2] == ' ' &&
				map[E6_RP - 1][E6_CP + 2] == ' ') {
				space_E6();
				E6_CP++;
			}
			else if (E6_CP != ethan_hunt_CP + 7 &&
				ethan_hunt_CP < E6_CP &&
				map[E6_RP][E6_CP - 2] == ' ' &&
				map[E6_RP - 1][E6_CP - 2] == ' ')
			{
				space_E6();
				E6_CP--;
			}
			if (ethan_hunt_CP == E6_CP)
			{
				if ((ethan_hunt_RP - 7) - E6_RP >= 7 &&
					ethan_hunt_RP > E6_RP &&
					map[E6_RP + 1][E6_CP] == ' ' &&
					map[E6_RP + 1][E6_CP - 1] == ' ' &&
					map[E6_RP + 1][E6_CP + 1] == ' ')
				{
					space_E6();
					E6_RP++;
				}
				else if (E6_RP - (ethan_hunt_RP + 7) >= 7 &&
					ethan_hunt_RP < E6_RP &&
					map[E6_RP - 2][E6_CP - 1] == ' ' &&
					map[E6_RP - 2][E6_CP + 1] == ' ' &&
					map[E6_RP - 2][E6_CP] == ' ') {
					space_E6();
					E6_RP--;
				}
			}
		}
	}

	else if (!rand_mov) {
		//1st enemy
		if (!killed_E1) {
			if (ethan_hunt_CP != E1_CP &&
				ethan_hunt_CP - 7 > E1_CP &&
				map[E1_RP][E1_CP + 2] == ' ' &&
				map[E1_RP - 1][E1_CP + 2] == ' ') {
				space_E1();
				E1_CP++;
			}
			else if (E1_CP != ethan_hunt_CP + 7 &&
				ethan_hunt_CP < E1_CP &&
				map[E1_RP][E1_CP - 2] == ' ' &&
				map[E1_RP - 1][E1_CP - 2] == ' ') {
				space_E1();
				E1_CP--;
			}
			if (ethan_hunt_CP == E1_CP)
			{
				if ((ethan_hunt_RP - 7) - E1_RP >= 7 &&
					ethan_hunt_RP > E1_RP &&
					map[E1_RP + 1][E1_CP] == ' ' &&
					map[E1_RP + 1][E1_CP - 1] == ' ' &&
					map[E1_RP + 1][E1_CP + 1] == ' ')
				{
					space_E1();
					E1_RP++;
				}
				else if (E1_RP - (ethan_hunt_RP + 7) >= 7 &&
					ethan_hunt_RP < E1_RP &&
					map[E1_RP - 2][E1_CP - 1] == ' ' &&
					map[E1_RP - 2][E1_CP + 1] == ' ' &&
					map[E1_RP - 2][E1_CP] == ' ') {
					space_E1();
					E1_RP--;
				}
			}
		}
		//2nd enemy
		if (!killed_E2) {
			if (ethan_hunt_RP - 7 != E2_RP &&
				ethan_hunt_RP > E2_RP &&
				map[E2_RP + 1][E2_CP] == ' ' &&
				map[E2_RP + 1][E2_CP - 1] == ' ' &&
				map[E2_RP + 1][E2_CP + 1] == ' ')
			{
				space_E2();
				E2_RP++;
			}
			else if (E2_RP != ethan_hunt_RP + 7 &&
				ethan_hunt_RP < E2_RP &&
				map[E2_RP - 2][E2_CP - 1] == ' ' &&
				map[E2_RP - 2][E2_CP + 1] == ' ' &&
				map[E2_RP - 2][E2_CP] == ' ') {
				space_E2();
				E2_RP--;
			}
			if (ethan_hunt_RP == E2_RP)
			{
				if ((ethan_hunt_CP - 7) - E2_CP >= 7 &&
					ethan_hunt_CP > E2_CP &&
					map[E2_RP][E2_CP + 2] == ' ' &&
					map[E2_RP - 1][E2_CP + 2] == ' ') {
					space_E2();
					E2_CP++;
				}
				else if (E2_CP - (ethan_hunt_CP + 7) >= 7 &&
					ethan_hunt_CP < E2_CP &&
					map[E2_RP][E2_CP - 2] == ' ' &&
					map[E2_RP - 1][E2_CP - 2] == ' ') {
					space_E2();
					E2_CP--;
				}
			}
		}
		//3rd enemy
		if (!killed_E3) {
			if (ethan_hunt_CP != E3_CP &&
				ethan_hunt_CP - 7 > E3_CP &&
				map[E3_RP][E3_CP + 2] == ' ' &&
				map[E3_RP - 1][E3_CP + 2] == ' ') {
				space_E3();
				E3_CP++;
			}
			else if (E3_CP != ethan_hunt_CP + 7 &&
				ethan_hunt_CP < E3_CP &&
				map[E3_RP][E3_CP - 2] == ' ' &&
				map[E3_RP - 1][E3_CP - 2] == ' ') {
				space_E3();
				E3_CP--;
			}
			if (ethan_hunt_CP == E3_CP)
			{
				if ((ethan_hunt_RP - 7) - E3_RP >= 7 &&
					ethan_hunt_RP > E3_RP &&
					map[E3_RP + 1][E3_CP] == ' ' &&
					map[E3_RP + 1][E3_CP - 1] == ' ' &&
					map[E3_RP + 1][E3_CP + 1] == ' ')
				{
					space_E3();
					E3_RP++;
				}
				else if (E3_RP - (ethan_hunt_RP + 7) >= 7 &&
					ethan_hunt_RP < E3_RP &&
					map[E3_RP - 2][E3_CP - 1] == ' ' &&
					map[E3_RP - 2][E3_CP + 1] == ' ' &&
					map[E3_RP - 2][E3_CP] == ' ') {
					space_E3();
					E3_RP--;
				}
			}
		}
		//4th enemy
		if (!killed_E4) {
			if (ethan_hunt_RP - 7 != E4_RP &&
				ethan_hunt_RP > E4_RP &&
				map[E4_RP + 1][E4_CP] == ' ' &&
				map[E4_RP + 1][E4_CP - 1] == ' ' &&
				map[E4_RP + 1][E4_CP + 1] == ' ')
			{
				space_E4();
				E4_RP++;
			}
			else if (E4_RP != ethan_hunt_RP + 7 &&
				ethan_hunt_RP < E4_RP &&
				map[E4_RP - 2][E4_CP - 1] == ' ' &&
				map[E4_RP - 2][E4_CP + 1] == ' ' &&
				map[E4_RP - 2][E4_CP] == ' ') {
				space_E4();
				E4_RP--;
			}
			if (ethan_hunt_RP == E4_RP)
			{
				if ((ethan_hunt_CP - 7) - E4_CP >= 7 &&
					ethan_hunt_CP > E4_CP &&
					map[E4_RP][E4_CP + 2] == ' ' &&
					map[E4_RP - 1][E4_CP + 2] == ' ') {
					space_E4();
					E4_CP++;
				}
				else if (E4_CP - (ethan_hunt_CP + 7) >= 7 &&
					ethan_hunt_CP < E4_CP &&
					map[E4_RP][E4_CP - 2] == ' ' &&
					map[E4_RP - 1][E4_CP - 2] == ' ') {
					space_E4();
					E4_CP--;
				}
			}
		}
		//5th enemy
		if (!killed_E5) {
			if (ethan_hunt_CP != E5_CP &&
				ethan_hunt_CP - 7 > E5_CP &&
				map[E5_RP][E5_CP + 2] == ' ' &&
				map[E5_RP - 1][E5_CP + 2] == ' ') {
				space_E5();
				E5_CP++;
			}
			else if (E5_CP != ethan_hunt_CP + 7 &&
				ethan_hunt_CP < E5_CP &&
				map[E5_RP][E5_CP - 2] == ' ' &&
				map[E5_RP - 1][E5_CP - 2] == ' ') {
				space_E5();
				E5_CP--;
			}
			if (ethan_hunt_CP == E5_CP)
			{
				if ((ethan_hunt_RP - 7) - E5_RP >= 7 &&
					ethan_hunt_RP > E5_RP &&
					map[E5_RP + 1][E5_CP] == ' ' &&
					map[E5_RP + 1][E5_CP - 1] == ' ' &&
					map[E5_RP + 1][E5_CP + 1] == ' ')
				{
					space_E5();
					E5_RP++;
				}
				else if (E5_RP - (ethan_hunt_RP + 7) >= 7 &&
					ethan_hunt_RP < E5_RP &&
					map[E5_RP - 2][E5_CP - 1] == ' ' &&
					map[E5_RP - 2][E5_CP + 1] == ' ' &&
					map[E5_RP - 2][E5_CP] == ' ') {
					space_E5();
					E5_RP--;
				}
			}
		}
		//6th enemy
		if (!killed_E6) {
			if (ethan_hunt_RP - 7 != E6_RP &&
				ethan_hunt_RP > E6_RP &&
				map[E6_RP + 1][E6_CP] == ' ' &&
				map[E6_RP + 1][E6_CP - 1] == ' ' &&
				map[E6_RP + 1][E6_CP + 1] == ' ')
			{
				space_E6();
				E6_RP++;
			}
			else if (E6_RP != ethan_hunt_RP + 7 &&
				ethan_hunt_RP < E6_RP &&
				map[E6_RP - 2][E6_CP - 1] == ' ' &&
				map[E6_RP - 2][E6_CP + 1] == ' ' &&
				map[E6_RP - 2][E6_CP] == ' ') {
				space_E6();
				E6_RP--;
			}
			if (ethan_hunt_RP == E6_RP)
			{
				if ((ethan_hunt_CP - 7) - E6_CP >= 7 &&
					ethan_hunt_CP > E6_CP &&
					map[E6_RP][E6_CP + 2] == ' ' &&
					map[E6_RP - 1][E6_CP + 2] == ' ') {
					space_E6();
					E6_CP++;
				}
				else if (E6_CP - (ethan_hunt_CP + 7) >= 7 &&
					ethan_hunt_CP < E6_CP &&
					map[E6_RP][E6_CP - 2] == ' ' &&
					map[E6_RP - 1][E6_CP - 2] == ' ') {
					space_E6();
					E6_CP--;
				}
			}
		}
	}
}
void space_E1() {
	map[E1_RP][E1_CP] = ' ';
	map[E1_RP][E1_CP - 1] = ' ';
	map[E1_RP][E1_CP + 1] = ' ';
	map[E1_RP - 1][E1_CP - 1] = ' ';
	map[E1_RP - 1][E1_CP + 1] = ' ';
	map[E1_RP - 1][E1_CP] = ' ';
}
void space_E2() {
	map[E2_RP][E2_CP] = ' ';
	map[E2_RP][E2_CP - 1] = ' ';
	map[E2_RP][E2_CP + 1] = ' ';
	map[E2_RP - 1][E2_CP - 1] = ' ';
	map[E2_RP - 1][E2_CP + 1] = ' ';
	map[E2_RP - 1][E2_CP] = ' ';
}
void space_E3() {
	map[E3_RP][E3_CP] = ' ';
	map[E3_RP][E3_CP - 1] = ' ';
	map[E3_RP][E3_CP + 1] = ' ';
	map[E3_RP - 1][E3_CP - 1] = ' ';
	map[E3_RP - 1][E3_CP + 1] = ' ';
	map[E3_RP - 1][E3_CP] = ' ';
}
void space_E4() {
	map[E4_RP][E4_CP] = ' ';
	map[E4_RP][E4_CP - 1] = ' ';
	map[E4_RP][E4_CP + 1] = ' ';
	map[E4_RP - 1][E4_CP - 1] = ' ';
	map[E4_RP - 1][E4_CP + 1] = ' ';
	map[E4_RP - 1][E4_CP] = ' ';
}
void space_E5() {
	map[E5_RP][E5_CP] = ' ';
	map[E5_RP][E5_CP - 1] = ' ';
	map[E5_RP][E5_CP + 1] = ' ';
	map[E5_RP - 1][E5_CP - 1] = ' ';
	map[E5_RP - 1][E5_CP + 1] = ' ';
	map[E5_RP - 1][E5_CP] = ' ';
}
void space_E6() {
	map[E6_RP][E6_CP] = ' ';
	map[E6_RP][E6_CP - 1] = ' ';
	map[E6_RP][E6_CP + 1] = ' ';
	map[E6_RP - 1][E6_CP - 1] = ' ';
	map[E6_RP - 1][E6_CP + 1] = ' ';
	map[E6_RP - 1][E6_CP] = ' ';
}


/// ////////////////////// ETHAN HUNT FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////
void initialize_ethan() {
	space_ethan(); //w after losing a life the 1st ethan should be removed from console otherwise there will be two ethans /
	do //check if ethan is printed in space 
	{
		int start = 14, end1 = ROW - 6;
		int start2 = 2, end2 = COL - 3;
		ethan_hunt_RP = start + rand() % (end1 - start + 1);
		ethan_hunt_CP = start2 + rand() % (end2 - start2 + 1);
	} while (map[ethan_hunt_RP][ethan_hunt_CP] != ' ' ||    //check if ethan is printed in space 
		map[ethan_hunt_RP][ethan_hunt_CP - 1] != ' ' ||
		map[ethan_hunt_RP][ethan_hunt_CP + 1] != ' ' ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] != ' ' ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] != ' ' ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] != ' ');

	flag = false;         //to terminate the condition flag should be false
	ethan_hunt();
}
void ethan_hunt() {

	if (flag && life > 0 && life <= 5) //it will only proceed when the game starts and  ethan lose a life 
	{
		initialize_ethan();
	}

	if (life > 0 && life <= 5) //if life becomes 0 it will not print ethan
	{
		map[ethan_hunt_RP][ethan_hunt_CP] = '|';
		map[ethan_hunt_RP][ethan_hunt_CP - 1] = '/';
		map[ethan_hunt_RP][ethan_hunt_CP + 1] = '\\';
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] = '/';
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] = '\\';
		map[ethan_hunt_RP - 1][ethan_hunt_CP] = 'o';
	}
	else
		play = false; //to end game cause life is 
}
void movement_ethan_hunt(char input) {
	if (input == 72 && (map[ethan_hunt_RP - 2][ethan_hunt_CP]) == ' ' && (map[ethan_hunt_RP - 1][ethan_hunt_CP - 1]) == ' ' && (map[ethan_hunt_RP - 1][ethan_hunt_CP + 1]) == ' ') {
		space_ethan();
		//level_change();
		ethan_hunt_RP--;
	}
	else if (input == 80 && (map[ethan_hunt_RP + 2][ethan_hunt_CP]) == ' ' && (map[ethan_hunt_RP + 2][ethan_hunt_CP - 1]) == ' ' && (map[ethan_hunt_RP + 2][ethan_hunt_CP + 1]) == ' ') {
		space_ethan();
		ethan_hunt_RP = ethan_hunt_RP + 1;
	}
	else if (input == 77 && (map[ethan_hunt_RP][ethan_hunt_CP + 2]) == ' ' && (map[ethan_hunt_RP + 1][ethan_hunt_CP + 2]) == ' ' && (map[ethan_hunt_RP - 1][ethan_hunt_CP + 1]) == ' ') {
		space_ethan();
		//level_change();
		ethan_hunt_CP = ethan_hunt_CP + 1;
	}
	else if (input == 75 && (map[ethan_hunt_RP][ethan_hunt_CP - 2]) == ' ' && (map[ethan_hunt_RP + 1][ethan_hunt_CP - 2]) == ' ' && (map[ethan_hunt_RP - 1][ethan_hunt_CP - 1]) == ' ') {
		space_ethan();
		//level_change();
		ethan_hunt_CP = ethan_hunt_CP - 1;
	}
	else
	{
		flag = true;
		life--;
		PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
	}
}
void space_ethan()
{
	map[ethan_hunt_RP][ethan_hunt_CP] = ' ';
	map[ethan_hunt_RP][ethan_hunt_CP] = ' ';
	map[ethan_hunt_RP][ethan_hunt_CP - 1] = ' ';
	map[ethan_hunt_RP][ethan_hunt_CP + 1] = ' ';
	map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] = ' ';
	map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] = ' ';
	map[ethan_hunt_RP - 1][ethan_hunt_CP] = ' ';
}

/// ////////////////////// SOLOMAN FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////
void soloman() {
	map[soloman_RP][soloman_CP] = 'X';
	map[soloman_RP][soloman_CP + 1] = ']';
	map[soloman_RP][soloman_CP - 1] = '[';
	map[soloman_RP][soloman_CP + 2] = '[';
	map[soloman_RP][soloman_CP + 3] = ']';
	map[soloman_RP][soloman_CP - 2] = ']';
	map[soloman_RP][soloman_CP - 3] = '[';
	map[soloman_RP + 1][soloman_CP] = ' ';
	map[soloman_RP + 1][soloman_CP + 1] = ']';
	map[soloman_RP + 1][soloman_CP - 1] = '[';
	map[soloman_RP - 1][soloman_CP] = ' ';
	map[soloman_RP - 1][soloman_CP + 1] = ']';
	map[soloman_RP - 1][soloman_CP - 1] = '[';
}
void movement_soloman_lane()
{
	int direction = rand() % 4;

	// Move Up
	if (direction == 0 && soloman_RP >= 3 &&
		map[soloman_RP - 2][soloman_CP] == ' ' &&
		map[soloman_RP - 2][soloman_CP - 1] == ' ' &&
		map[soloman_RP - 2][soloman_CP + 1] == ' ' &&
		map[soloman_RP - 1][soloman_CP + 2] == ' ' &&
		map[soloman_RP - 1][soloman_CP + 3] == ' ' &&
		map[soloman_RP - 1][soloman_CP - 2] == ' ' &&
		map[soloman_RP - 1][soloman_CP - 3] == ' ')
	{
		space_soloman();
		soloman_RP--;
	}
	// Move Down
	else if (direction == 1 && soloman_RP < ROW - 3 &&
		map[soloman_RP + 2][soloman_CP] == ' ' &&
		map[soloman_RP + 2][soloman_CP - 1] == ' ' &&
		map[soloman_RP + 2][soloman_CP + 1] == ' ' &&
		map[soloman_RP + 1][soloman_CP + 2] == ' ' &&
		map[soloman_RP + 1][soloman_CP + 3] == ' ' &&
		map[soloman_RP + 1][soloman_CP - 2] == ' ' &&
		map[soloman_RP + 1][soloman_CP - 3] == ' ')
	{
		space_soloman();
		soloman_RP++;
	}
	// Move Right
	else if (direction == 2 && soloman_CP < COL - 6 &&
		map[soloman_RP][soloman_CP + 4] == ' ' &&
		map[soloman_RP + 1][soloman_CP + 2] == ' ' &&
		map[soloman_RP - 1][soloman_CP + 2] == ' ')
	{
		space_soloman();
		soloman_CP++;
	}
	// Move Left
	else if (direction == 3 && soloman_CP >= 2 &&
		map[soloman_RP][soloman_CP - 4] == ' ' &&
		map[soloman_RP + 1][soloman_CP - 2] == ' ' &&
		map[soloman_RP - 1][soloman_CP - 2] == ' ')
	{
		space_soloman();
		soloman_CP--;
	}
	soloman(); // Redraw Soloman at the new position
	if (ethan_overlaps_soloman())
	{
		life--;
		PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
		initialize_ethan();
		if (life == 0) //to end game cause life is 0
		{
			play = false;
		}
	}
}
void space_soloman() {
	map[soloman_RP][soloman_CP] = ' ';
	map[soloman_RP][soloman_CP + 1] = ' ';
	map[soloman_RP][soloman_CP - 1] = ' ';
	map[soloman_RP][soloman_CP + 2] = ' ';
	map[soloman_RP][soloman_CP + 3] = ' ';
	map[soloman_RP][soloman_CP - 2] = ' ';
	map[soloman_RP][soloman_CP - 3] = ' ';
	map[soloman_RP + 1][soloman_CP] = ' ';
	map[soloman_RP + 1][soloman_CP + 1] = ' ';
	map[soloman_RP + 1][soloman_CP - 1] = ' ';
	map[soloman_RP - 1][soloman_CP] = ' ';
	map[soloman_RP - 1][soloman_CP + 1] = ' ';
	map[soloman_RP - 1][soloman_CP - 1] = ' ';
}
bool ethan_overlaps_soloman()
{
	if (
		// right side of ethan with  map[soloman_RP][soloman_CP - 4]
		map[ethan_hunt_RP][ethan_hunt_CP + 1] == map[soloman_RP][soloman_CP - 4] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP][soloman_CP - 4] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP][soloman_CP - 4] ||
		// right side of ethan with  map[soloman_RP - 1][soloman_CP - 2]
		map[ethan_hunt_RP][ethan_hunt_CP + 1] == map[soloman_RP - 1][soloman_CP - 2] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP - 1][soloman_CP - 2] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP - 1][soloman_CP - 2] ||
		// right side of ethan with  map[soloman_RP + 1][soloman_CP -2]
		map[ethan_hunt_RP][ethan_hunt_CP + 1] == map[soloman_RP + 1][soloman_CP - 2] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP + 1][soloman_CP - 2] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 1][soloman_CP - 2] ||
		// right side of ethan with  map[soloman_RP - 1][soloman_CP - 3]
		map[ethan_hunt_RP][ethan_hunt_CP + 1] == map[soloman_RP - 1][soloman_CP - 3] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP - 1][soloman_CP - 3] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP - 1][soloman_CP - 3] ||
		// right side of ethan with  map[soloman_RP + 1][soloman_CP - 3]
		map[ethan_hunt_RP][ethan_hunt_CP + 1] == map[soloman_RP + 1][soloman_CP - 3] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP + 1][soloman_CP - 3] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 1][soloman_CP - 3] ||


		// left side of ethan with  map[soloman_RP][soloman_CP +4]
		map[ethan_hunt_RP][ethan_hunt_CP - 1] == map[soloman_RP][soloman_CP + 4] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP][soloman_CP + 4] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP][soloman_CP + 4] ||
		// left side of ethan with  map[soloman_RP + 1][soloman_CP + 2]
		map[ethan_hunt_RP][ethan_hunt_CP - 1] == map[soloman_RP + 1][soloman_CP + 2] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 1][soloman_CP + 2] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP + 1][soloman_CP + 2] ||
		// left side of ethan with  map[soloman_RP - 1][soloman_CP + 2]
		map[ethan_hunt_RP][ethan_hunt_CP - 1] == map[soloman_RP - 1][soloman_CP + 2] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP - 1][soloman_CP + 2] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP - 1][soloman_CP + 2] ||
		// left side of ethan with  map[soloman_RP + 1][soloman_CP + 3]
		map[ethan_hunt_RP][ethan_hunt_CP - 1] == map[soloman_RP + 1][soloman_CP + 3] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 1][soloman_CP + 3] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP + 1][soloman_CP + 3] ||
		// left side of ethan with  map[soloman_RP - 1][soloman_CP +3]
		map[ethan_hunt_RP][ethan_hunt_CP - 1] == map[soloman_RP - 1][soloman_CP + 3] ||
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP - 1][soloman_CP + 3] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP - 1][soloman_CP + 3] ||

		// bottom of ethan with  map[soloman_RP-2][soloman_CP -1]
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP - 2][soloman_CP - 1] ||
		// bottom of ethan with  map[soloman_RP-2][soloman_CP]
		map[ethan_hunt_RP + 1][ethan_hunt_CP + 1] == map[soloman_RP - 2][soloman_CP] ||
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP - 2][soloman_CP] ||
		// bottom of ethan with  map[soloman_RP-2][soloman_CP+1]
		map[ethan_hunt_RP + 1][ethan_hunt_CP - 1] == map[soloman_RP - 2][soloman_CP + 1] ||

		// top of ethan with  map[soloman_RP+2][soloman_CP-1]
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 2][soloman_CP - 1] ||
		// top of ethan with  map[soloman_RP+2][soloman_CP]
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 2][soloman_CP] ||
		// top of ethan with  map[soloman_RP+2][soloman_CP+1]
		map[ethan_hunt_RP - 1][ethan_hunt_CP] == map[soloman_RP + 2][soloman_CP + 1]
		)
	{
		return true;
	}
	else {
		return false;
	}


}

/// ////////////////////// BULLET FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////
void bullet_direction(char bullet_input) {

	// giving bullet the row & col position equal to row & col of ethan hunt
	if (bullet_input == 'w') {
		bullet_RP = ethan_hunt_RP - 2;
		bullet_CP = ethan_hunt_CP;
	}
	else if (bullet_input == 's') {
		bullet_RP = ethan_hunt_RP + 2;
		bullet_CP = ethan_hunt_CP;
	}
	else if (bullet_input == 'a') {
		bullet_RP = ethan_hunt_RP;
		bullet_CP = ethan_hunt_CP - 2;
	}
	else if (bullet_input == 'd') {
		bullet_RP = ethan_hunt_RP;
		bullet_CP = ethan_hunt_CP + 2;
	}
}
void bullet_movement() {
	if (bullet_input == 'w' && map[bullet_RP - 1][bullet_CP] == ' ') {
		map[bullet_RP][bullet_CP] = ' '; //to erase previous bullet
		bullet_RP--;
		kill();
	}
	else if (bullet_input == 's' && map[bullet_RP + 1][bullet_CP] == ' ') {
		map[bullet_RP][bullet_CP] = ' '; //to erase previous bullet
		bullet_RP++;
		kill();
	}
	else if (bullet_input == 'd' && map[bullet_RP][bullet_CP + 1] == ' ') {
		map[bullet_RP][bullet_CP] = ' '; //to erase previous bullet
		bullet_CP++;
		kill();
	}
	else if (bullet_input == 'a' && map[bullet_RP][bullet_CP - 1] == ' ') {
		map[bullet_RP][bullet_CP] = ' '; //to erase previous bullet
		bullet_CP--;
		kill();
	}
}
void kill() {
	if (!killed_E1) {
		// killing enemy 1
		if (bullet_RP == E1_RP + 1 && bullet_CP == E1_CP ||
			bullet_RP == E1_RP + 1 && bullet_CP == E1_CP - 1 ||
			bullet_RP == E1_RP + 1 && bullet_CP == E1_CP + 1 ||
			bullet_RP == E1_RP - 1 && bullet_CP == E1_CP + 2 ||
			bullet_RP == E1_RP && bullet_CP == E1_CP + 2 ||
			bullet_RP == E1_RP - 2 && bullet_CP == E1_CP ||
			bullet_RP == E1_RP - 2 && bullet_CP == E1_CP + 1 ||
			bullet_RP == E1_RP - 2 && bullet_CP == E1_CP - 1 ||
			bullet_RP == E1_RP - 1 && bullet_CP == E1_CP - 2 ||
			bullet_RP == E1_RP && bullet_CP == E1_CP - 2
			) {
			killed_E1 = true;
			space_E1(); // Remove enemy E1
			PlaySound(TEXT("kill"), NULL, SND_FILENAME | SND_ASYNC);
			score += 2;
			target += 2;
			fire = false; // Stop the bullet
			isnot_active = true;
		}
	}
	if (!killed_E2) {
		// killing enemy 2
		if (bullet_RP == E2_RP + 1 && bullet_CP == E2_CP ||
			bullet_RP == E2_RP + 1 && bullet_CP == E2_CP - 1 ||
			bullet_RP == E2_RP + 1 && bullet_CP == E2_CP + 1 ||
			bullet_RP == E2_RP - 1 && bullet_CP == E2_CP + 2 ||
			bullet_RP == E2_RP && bullet_CP == E2_CP + 2 ||
			bullet_RP == E2_RP - 2 && bullet_CP == E2_CP ||
			bullet_RP == E2_RP - 2 && bullet_CP == E2_CP + 1 ||
			bullet_RP == E2_RP - 2 && bullet_CP == E2_CP - 1 ||
			bullet_RP == E2_RP - 1 && bullet_CP == E2_CP - 2 ||
			bullet_RP == E2_RP && bullet_CP == E2_CP - 2
			) {
			killed_E2 = true;
			space_E2(); // Remove enemy E2
			PlaySound(TEXT("kill"), NULL, SND_FILENAME | SND_ASYNC);
			score += 2;
			target += 2;
			fire = false; // Stop the bullet
			isnot_active = true;
		}
	}

	if (!killed_E3) {
		// killing enemy 3
		if (bullet_RP == E3_RP + 1 && bullet_CP == E3_CP ||
			bullet_RP == E3_RP + 1 && bullet_CP == E3_CP - 1 ||
			bullet_RP == E3_RP + 1 && bullet_CP == E3_CP + 1 ||
			bullet_RP == E3_RP - 1 && bullet_CP == E3_CP + 2 ||
			bullet_RP == E3_RP && bullet_CP == E3_CP + 2 ||
			bullet_RP == E3_RP - 2 && bullet_CP == E3_CP ||
			bullet_RP == E3_RP - 2 && bullet_CP == E3_CP + 1 ||
			bullet_RP == E3_RP - 2 && bullet_CP == E3_CP - 1 ||
			bullet_RP == E3_RP - 1 && bullet_CP == E3_CP - 2 ||
			bullet_RP == E3_RP && bullet_CP == E3_CP - 2
			) {
			killed_E3 = true;
			space_E3(); // Remove enemy E3
			PlaySound(TEXT("kill"), NULL, SND_FILENAME | SND_ASYNC);
			score += 2;
			target += 2;
			fire = false; // Stop the bullet
			isnot_active = true;
		}
	}

	if (!killed_E4) {
		// killing enemy 4
		if (bullet_RP == E4_RP + 1 && bullet_CP == E4_CP ||
			bullet_RP == E4_RP + 1 && bullet_CP == E4_CP - 1 ||
			bullet_RP == E4_RP + 1 && bullet_CP == E4_CP + 1 ||
			bullet_RP == E4_RP - 1 && bullet_CP == E4_CP + 2 ||
			bullet_RP == E4_RP && bullet_CP == E4_CP + 2 ||
			bullet_RP == E4_RP - 2 && bullet_CP == E4_CP ||
			bullet_RP == E4_RP - 2 && bullet_CP == E4_CP + 1 ||
			bullet_RP == E4_RP - 2 && bullet_CP == E4_CP - 1 ||
			bullet_RP == E4_RP - 1 && bullet_CP == E4_CP - 2 ||
			bullet_RP == E4_RP && bullet_CP == E4_CP - 2
			) {
			killed_E4 = true;
			space_E4(); // Remove enemy E4
			PlaySound(TEXT("kill"), NULL, SND_FILENAME | SND_ASYNC);
			score += 2;
			target += 2;
			fire = false; // Stop the bullet
			isnot_active = true;
		}
	}

	if (!killed_E5) {
		// killing enemy 5
		if (bullet_RP == E5_RP + 1 && bullet_CP == E5_CP ||
			bullet_RP == E5_RP + 1 && bullet_CP == E5_CP - 1 ||
			bullet_RP == E5_RP + 1 && bullet_CP == E5_CP + 1 ||
			bullet_RP == E5_RP - 1 && bullet_CP == E5_CP + 2 ||
			bullet_RP == E5_RP && bullet_CP == E5_CP + 2 ||
			bullet_RP == E5_RP - 2 && bullet_CP == E5_CP ||
			bullet_RP == E5_RP - 2 && bullet_CP == E5_CP + 1 ||
			bullet_RP == E5_RP - 2 && bullet_CP == E5_CP - 1 ||
			bullet_RP == E5_RP - 1 && bullet_CP == E5_CP - 2 ||
			bullet_RP == E5_RP && bullet_CP == E5_CP - 2
			) {
			killed_E5 = true;
			space_E5(); // Remove enemy E5
			PlaySound(TEXT("kill"), NULL, SND_FILENAME | SND_ASYNC);
			score += 2;
			target += 2;
			fire = false; // Stop the bullet
			isnot_active = true;
		}
	}

	if (!killed_E6) {
		// killing enemy 6
		if (bullet_RP == E6_RP + 1 && bullet_CP == E6_CP ||
			bullet_RP == E6_RP + 1 && bullet_CP == E6_CP - 1 ||
			bullet_RP == E6_RP + 1 && bullet_CP == E6_CP + 1 ||
			bullet_RP == E6_RP - 1 && bullet_CP == E6_CP + 2 ||
			bullet_RP == E6_RP && bullet_CP == E6_CP + 2 ||
			bullet_RP == E6_RP - 2 && bullet_CP == E6_CP ||
			bullet_RP == E6_RP - 2 && bullet_CP == E6_CP + 1 ||
			bullet_RP == E6_RP - 2 && bullet_CP == E6_CP - 1 ||
			bullet_RP == E6_RP - 1 && bullet_CP == E6_CP - 2 ||
			bullet_RP == E6_RP && bullet_CP == E6_CP - 2
			) {
			killed_E6 = true;
			space_E6(); // Remove enemy E6
			PlaySound(TEXT("kill"), NULL, SND_FILENAME | SND_ASYNC);
			score += 2;
			target += 2;
			fire = false; // Stop the bullet
			isnot_active = true;
		}
	}

}
void print_bullet() {
	if (fire) {
		if (bullet_input == 'w') {
			map[bullet_RP][bullet_CP] = '*';
			if (map[bullet_RP - 1][bullet_CP] != ' ') {  // to remomve bullet if collide
				map[bullet_RP][bullet_CP] = ' ';
				fire = false;
				isnot_active = true;//to avoid bullet stick with collosions
			}

		}
		else if (bullet_input == 's') {
			map[bullet_RP][bullet_CP] = '*';
			if (map[bullet_RP + 1][bullet_CP] != ' ') {  // to remomve bullet if collide
				map[bullet_RP][bullet_CP] = ' ';
				fire = false;
				isnot_active = true;//to avoid bullet stick with collosions
			}
		}
		else if (bullet_input == 'a') {
			map[bullet_RP][bullet_CP] = '*';
			if (map[bullet_RP][bullet_CP - 1] != ' ') {  // to remomve bullet if collide
				map[bullet_RP][bullet_CP] = ' ';
				fire = false;
				isnot_active = true;//to avoid bullet stick with collosions
			}
		}
		else if (bullet_input == 'd') {
			map[bullet_RP][bullet_CP] = '*';
			if (map[bullet_RP][bullet_CP + 1] != ' ') {  // to remomve bullet if collide
				map[bullet_RP][bullet_CP] = ' ';
				fire = false;
				isnot_active = true;//to avoid bullet stick with collosions
			}
		}
	}
}
void missile() {
		if (initial) {
			missile_RP = ethan_hunt_RP - 2;
			missile_CP = ethan_hunt_CP;
			initial = false;
		}

	rand:
		if (random) {
			if (level == 1)
				rand_no = rand() % 2;
			else if (level == 2)
				rand_no = rand() % 3;
			else if (level == 3)
				rand_no = rand() % 4;
			else if (level == 4)
				rand_no = rand() % 5;
			else if (level == 5)
				rand_no = rand() % 6;
			random = false;
		}
		map[missile_RP][missile_CP] = '^';
		switch (rand_no) {
		case 0:
			if (!killed_E1) {
				if (map[missile_RP][missile_CP] == '^')
				{
					map[missile_RP][missile_CP] = ' ';
				}
				if (missile_RP != E1_RP || missile_CP != E1_CP) {
					if (missile_RP >= E1_RP) {
						if (missile_RP > E1_RP)
						{
							missile_RP--;
						}
						if (missile_CP > E1_CP) {
							missile_CP--;
						}
						else if (missile_CP < E1_CP) {
							missile_CP++;
						}
					}
					else if (missile_RP <= E1_RP) {
						if (missile_RP < E1_RP)
						{
							missile_RP++;
						}
						if (missile_CP > E1_CP)
							missile_CP--;
						else if (missile_CP < E1_CP) {
							missile_CP++;
						}
					}
				}
				if (map[missile_RP][missile_CP] == ' ')
				{
					map[missile_RP][missile_CP] = '^';
				}
				if (missile_RP == E1_RP && missile_CP == E1_CP) {
					space_E1();
					killed_E1 = true;
					missile_active = false;
					initial = true;
				}
			}
			else {
				random = true;
				goto rand;
			}
			break;
		case 1:
			if (!killed_E2)
			{
				if (map[missile_RP][missile_CP] == '^')
				{
					map[missile_RP][missile_CP] = ' ';
				}
				if (missile_RP != E2_RP || missile_CP != E2_CP) {
					if (missile_RP >= E1_RP) {
						if (missile_RP > E1_RP)
						{
							missile_RP--;
						}
						if (missile_CP > E2_CP) {
							missile_CP--;
						}
						else if (missile_CP < E2_CP) {
							missile_CP++;
						}
					}
					else if (missile_RP <= E2_RP) {
						if (missile_RP < E2_RP)
						{
							missile_RP++;
						}
						if (missile_CP > E2_CP)
							missile_CP--;
						else if (missile_CP < E2_CP) {
							missile_CP++;
						}
					}
				}
				if (map[missile_RP][missile_CP] == ' ')
				{
					map[missile_RP][missile_CP] = '^';
				}
				if (missile_RP == E2_RP && missile_CP == E2_CP) {
					space_E2();
					killed_E2 = true;
					missile_active = false;
					initial = true;
				}
			}
			else {
				random = true;
				goto rand;
			}
			break;
		case 2:
			if (!killed_E3) {
				if (map[missile_RP][missile_CP] == '^')
				{
					map[missile_RP][missile_CP] = ' ';
				}
				if (missile_RP != E3_RP || missile_CP != E3_CP) {
					if (missile_RP >= E3_RP) {
						if (missile_RP > E3_RP)
						{
							missile_RP--;
						}
						if (missile_CP > E3_CP) {
							missile_CP--;
						}
						else if (missile_CP < E3_CP) {
							missile_CP++;
						}
					}
					else if (missile_RP <= E3_RP) {
						if (missile_RP < E3_RP)
						{
							missile_RP++;
						}
						if (missile_CP > E3_CP)
							missile_CP--;
						else if (missile_CP < E3_CP) {
							missile_CP++;
						}
					}
				}
				if (map[missile_RP][missile_CP] == ' ')
				{
					map[missile_RP][missile_CP] = '^';
				}

				if (missile_RP == E3_RP && missile_CP == E3_CP) {
					space_E3();
					killed_E3 = true;
					missile_active = false;
					initial = true;
				}
			}
			else {
				random = true;
				goto rand;
			}
			break;
		case 3:

			if (!killed_E4) {
				if (map[missile_RP][missile_CP] == '^')
				{
					map[missile_RP][missile_CP] = ' ';
				}
				if (missile_RP != E4_RP || missile_CP != E4_CP) {
					if (missile_RP >= E4_RP) {
						if (missile_RP > E4_RP)
						{
							missile_RP--;
						}
						if (missile_CP > E4_CP) {
							missile_CP--;
						}
						else if (missile_CP < E4_CP) {
							missile_CP++;
						}
					}
					else if (missile_RP <= E4_RP) {
						if (missile_RP < E4_RP)
						{
							missile_RP++;
						}
						if (missile_CP > E4_CP)
							missile_CP--;
						else if (missile_CP < E4_CP) {
							missile_CP++;
						}
					}
				}
				if (map[missile_RP][missile_CP] == ' ')
				{
					map[missile_RP][missile_CP] = '^';
				}
				if (missile_RP == E4_RP && missile_CP == E4_CP) {
					space_E4();
					killed_E4 = true;
					missile_active = false;
					initial = true;
				}
			}
			else {
				random = true;
				goto rand;
			}
			break;
		case 4:
			if (map[missile_RP][missile_CP] == '^')
			{
				map[missile_RP][missile_CP] = ' ';
			}
			if (missile_RP != E5_RP || missile_CP != E5_CP) {
				if (missile_RP >= E5_RP) {
					if (missile_RP > E5_RP)
					{
						missile_RP--;
					}
					if (missile_CP > E5_CP) {
						missile_CP--;
					}
					else if (missile_CP < E5_CP) {
						missile_CP++;
					}
				}
				else if (missile_RP <= E5_RP) {
					if (missile_RP < E5_RP)
					{
						missile_RP++;
					}
					if (missile_CP > E5_CP)
						missile_CP--;
					else if (missile_CP < E5_CP) {
						missile_CP++;
					}
				}
			}
			if (map[missile_RP][missile_CP] == ' ')
			{
				map[missile_RP][missile_CP] = '^';
			}
			if (missile_RP == E5_RP && missile_CP == E5_CP) {
				space_E5();
				killed_E5 = true;
				missile_active = false;
				initial = true;
			}
			else {
				random = true;
				goto rand;
			}
			break;
		case 5:
			if (!killed_E6) {
				if (map[missile_RP][missile_CP] == '^')
				{
					map[missile_RP][missile_CP] = ' ';
				}
				if (missile_RP != E6_RP || missile_CP != E6_CP) {
					if (missile_RP >= E6_RP) {
						if (missile_RP > E6_RP)
						{
							missile_RP--;
						}
						if (missile_CP > E6_CP) {
							missile_CP--;
						}
						else if (missile_CP < E6_CP) {
							missile_CP++;
						}
					}
					else if (missile_RP <= E6_RP) {
						if (missile_RP < E6_RP)
						{
							missile_RP++;
						}
						if (missile_CP > E3_CP)
							missile_CP--;
						else if (missile_CP < E6_CP) {
							missile_CP++;
						}
					}
				}
				if (map[missile_RP][missile_CP] == ' ')
				{
					map[missile_RP][missile_CP] = '^';
				}
				if (missile_RP == E6_RP && missile_CP == E6_CP) {
					space_E6();
					killed_E6 = true;
					missile_active = false;
					initial = true;
				}
			}
			else {
				random = true;
				goto rand;
			}
			break;
		}
}
void enemy_bullet() {
	//Enemy 1
	if (firE1) {
		firE1 = false;
		if (E1_RP > ethan_hunt_RP &&
			E1_CP == ethan_hunt_CP) {
			E1bullet_RP = E1_RP - 2;
			E1bullet_CP = E1_CP;
			b_direction = 'n';
		}
		else if (E1_RP < ethan_hunt_RP &&
			E1_CP == ethan_hunt_CP) {
			E1bullet_RP = E1_RP + 1;
			E1bullet_CP = E1_CP;
			b_direction = 's';
		}
		if (E1_CP < ethan_hunt_CP &&
			E1_RP == ethan_hunt_RP) {
			E1bullet_CP = E1_CP + 2;
			E1bullet_RP = E1_RP;
			b_direction = 'e';
		}
		else if (E1_CP > ethan_hunt_CP &&
			E1_RP == ethan_hunt_RP) {
			E1bullet_CP = E1_CP - 2;
			E1bullet_RP = E1_RP;
			b_direction = 'w';
		}
	}

	//Enemy 2
	if (firE2) {
		firE2 = false;

		if (E2_RP > ethan_hunt_RP &&
			E2_CP == ethan_hunt_CP) {
			E2bullet_RP = E2_RP - 2;
			E2bullet_CP = E2_CP;
			b2_direction = 'n';
		}
		else if (E1_RP < ethan_hunt_RP &&
			E2_CP == ethan_hunt_CP) {
			E2bullet_RP = E2_RP + 1;
			E2bullet_CP = E2_CP;
			b2_direction = 's';
		}
		if (E2_CP < ethan_hunt_CP &&
			E2_RP == ethan_hunt_RP) {
			E2bullet_CP = E2_CP + 2;
			E2bullet_RP = E2_RP;
			b2_direction = 'e';
		}
		else if (E1_CP > ethan_hunt_CP &&
			E2_RP == ethan_hunt_RP) {
			E2bullet_CP = E2_CP - 2;
			E2bullet_RP = E2_RP;
			b2_direction = 'w';
		}
	}

	//Enemy 3
	if (firE3) {
		firE3 = false;

		if (E3_RP > ethan_hunt_RP &&
			E3_CP == ethan_hunt_CP) {
			E3bullet_RP = E3_RP - 2;
			E3bullet_CP = E3_CP;
			b3_direction = 'n';
		}
		else if (E1_RP < ethan_hunt_RP &&
			E3_CP == ethan_hunt_CP) {
			E3bullet_RP = E3_RP + 1;
			E3bullet_CP = E3_CP;
			b3_direction = 's';
		}
		if (E3_CP < ethan_hunt_CP &&
			E3_RP == ethan_hunt_RP) {
			E3bullet_CP = E3_CP + 2;
			E3bullet_RP = E3_RP;
			b3_direction = 'e';
		}
		else if (E1_CP > ethan_hunt_CP &&
			E3_RP == ethan_hunt_RP) {
			E3bullet_CP = E3_CP - 2;
			E3bullet_RP = E3_RP;
			b3_direction = 'w';
		}
	}

	//Enemy 4
	if (firE4) {
		firE4 = false;

		if (E4_RP > ethan_hunt_RP &&
			E4_CP == ethan_hunt_CP) {
			E4bullet_RP = E4_RP - 2;
			E4bullet_CP = E4_CP;
			b4_direction = 'n';
		}
		else if (E1_RP < ethan_hunt_RP &&
			E4_CP == ethan_hunt_CP) {
			E4bullet_RP = E4_RP + 1;
			E4bullet_CP = E4_CP;
			b4_direction = 's';
		}
		if (E4_CP < ethan_hunt_CP &&
			E4_RP == ethan_hunt_RP) {
			E4bullet_CP = E4_CP + 2;
			E4bullet_RP = E4_RP;
			b4_direction = 'e';
		}
		else if (E1_CP > ethan_hunt_CP &&
			E4_RP == ethan_hunt_RP) {
			E4bullet_CP = E4_CP - 2;
			E4bullet_RP = E4_RP;
			b4_direction = 'w';
		}
	}

	//Enemy 5
	if (firE5) {
		firE5 = false;

		if (E5_RP > ethan_hunt_RP &&
			E5_CP == ethan_hunt_CP) {
			E5bullet_RP = E5_RP - 2;
			E5bullet_CP = E5_CP;
			b5_direction = 'n';
		}
		else if (E1_RP < ethan_hunt_RP &&
			E5_CP == ethan_hunt_CP) {
			E5bullet_RP = E5_RP + 1;
			E5bullet_CP = E5_CP;
			b5_direction = 's';
		}
		if (E5_CP < ethan_hunt_CP &&
			E5_RP == ethan_hunt_RP) {
			E5bullet_CP = E5_CP + 2;
			E5bullet_RP = E5_RP;
			b5_direction = 'e';
		}
		else if (E1_CP > ethan_hunt_CP &&
			E5_RP == ethan_hunt_RP) {
			E5bullet_CP = E5_CP - 2;
			E5bullet_RP = E5_RP;
			b5_direction = 'w';
		}
	}

	//Enemy 6
	if (firE6) {
		firE6 = false;

		if (E6_RP > ethan_hunt_RP &&
			E6_CP == ethan_hunt_CP) {
			E6bullet_RP = E6_RP - 2;
			E6bullet_CP = E6_CP;
			b6_direction = 'n';
		}
		else if (E1_RP < ethan_hunt_RP &&
			E6_CP == ethan_hunt_CP) {
			E6bullet_RP = E6_RP + 1;
			E6bullet_CP = E6_CP;
			b6_direction = 's';
		}
		if (E6_CP < ethan_hunt_CP &&
			E6_RP == ethan_hunt_RP) {
			E6bullet_CP = E6_CP + 2;
			E6bullet_RP = E6_RP;
			b6_direction = 'e';
		}
		else if (E1_CP > ethan_hunt_CP &&
			E6_RP == ethan_hunt_RP) {
			E6bullet_CP = E6_CP - 2;
			E6bullet_RP = E6_RP;
			b6_direction = 'w';
		}
	}
}
void Ebullet_move() {
	//enemy 1
	if (b_direction == 'n' && map[E1bullet_RP - 1][E1bullet_CP] == ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = true;
		E1bullet_RP--;
	}

	else if (b_direction == 's' && map[E1bullet_RP + 1][E1bullet_CP] == ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = true;
		E1bullet_RP++;
	}
	else if (b_direction == 'w' && map[E1bullet_RP][E1bullet_CP - 1] == ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = true;
		E1bullet_CP--;
	}
	else if (b_direction == 'e' && map[E1bullet_RP][E1bullet_CP + 1] == ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = true;
		E1bullet_CP++;
	}
	//enemy 2
	if (b2_direction == 'n' && map[E2bullet_RP - 1][E2bullet_CP] == ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = true;
		E2bullet_RP--;
	}

	else if (b2_direction == 's' && map[E2bullet_RP + 1][E2bullet_CP] == ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = true;
		E2bullet_RP++;
	}
	else if (b2_direction == 'w' && map[E2bullet_RP][E2bullet_CP - 1] == ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = true;
		E2bullet_CP--;
	}
	else if (b2_direction == 'e' && map[E2bullet_RP][E2bullet_CP + 1] == ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = true;
		E2bullet_CP++;
	}
	//enemy 3
	if (b3_direction == 'n' && map[E3bullet_RP - 1][E3bullet_CP] == ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = true;
		E3bullet_RP--;
	}

	else if (b3_direction == 's' && map[E3bullet_RP + 1][E3bullet_CP] == ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = true;
		E3bullet_RP++;
	}
	else if (b3_direction == 'w' && map[E3bullet_RP][E3bullet_CP - 1] == ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = true;
		E3bullet_CP--;
	}
	else if (b3_direction == 'e' && map[E3bullet_RP][E3bullet_CP + 1] == ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = true;
		E3bullet_CP++;
	}
	//enemy 4
	if (b4_direction == 'n' && map[E4bullet_RP - 1][E4bullet_CP] == ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = true;
		E4bullet_RP--;
	}

	else if (b4_direction == 's' && map[E4bullet_RP + 1][E4bullet_CP] == ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = true;
		E4bullet_RP++;
	}
	else if (b4_direction == 'w' && map[E4bullet_RP][E4bullet_CP - 1] == ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = true;
		E4bullet_CP--;
	}
	else if (b4_direction == 'e' && map[E4bullet_RP][E4bullet_CP + 1] == ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = true;
		E4bullet_CP++;
	}
	//enemy 5
	if (b5_direction == 'n' && map[E5bullet_RP - 1][E5bullet_CP] == ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = true;
		E5bullet_RP--;
	}

	else if (b5_direction == 's' && map[E5bullet_RP + 1][E5bullet_CP] == ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = true;
		E5bullet_RP++;
	}
	else if (b5_direction == 'w' && map[E5bullet_RP][E5bullet_CP - 1] == ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = true;
		E5bullet_CP--;
	}
	else if (b5_direction == 'e' && map[E5bullet_RP][E5bullet_CP + 1] == ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = true;
		E5bullet_CP++;
	}
	//enemy 6
	if (b6_direction == 'n' && map[E6bullet_RP - 1][E6bullet_CP] == ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = true;
		E6bullet_RP--;
	}

	else if (b6_direction == 's' && map[E6bullet_RP + 1][E6bullet_CP] == ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = true;
		E6bullet_RP++;
	}
	else if (b6_direction == 'w' && map[E6bullet_RP][E6bullet_CP - 1] == ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = true;
		E6bullet_CP--;
	}
	else if (b6_direction == 'e' && map[E6bullet_RP][E6bullet_CP + 1] == ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = true;
		E6bullet_CP++;
	}
}
void Ebullet_print() {
	//enemy 1
	map[E1bullet_RP][E1bullet_CP] = '*';

	if (b_direction == 'n' && map[E1bullet_RP - 1][E1bullet_CP] != ' ') {  // to remomve bullet if collide
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = false;
		firE1 = true;
		if ((E1bullet_RP - 1) == (ethan_hunt_RP) && (E1bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b_direction == 's' && map[E1bullet_RP + 1][E1bullet_CP] != ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = false;
		firE1 = true;
		if ((E1bullet_RP + 1) == (ethan_hunt_RP - 1) && (E1bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b_direction == 'e' && map[E1bullet_RP][E1bullet_CP + 1] != ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = false;
		firE1 = true;
		if ((E1bullet_RP) == (ethan_hunt_RP) && (E1bullet_CP + 1) == (ethan_hunt_CP - 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b_direction == 'w' && map[E1bullet_RP][E1bullet_CP - 1] != ' ') {
		map[E1bullet_RP][E1bullet_CP] = ' ';
		is_active1 = false;
		firE1 = true;
		if ((E1bullet_RP) == (ethan_hunt_RP) && (E1bullet_CP - 1) == (ethan_hunt_CP + 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	if (!is_active1)
		firE1 = true;
	if (killed_E1)
		firE1 = false;

	//enemy 2
	map[E2bullet_RP][E2bullet_CP] = '*';

	if (b2_direction == 'n' && map[E2bullet_RP - 1][E2bullet_CP] != ' ') {  // to remomve bullet if collide
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = false;
		firE2 = true;
		if ((E2bullet_RP - 1) == (ethan_hunt_RP) && (E2bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b2_direction == 's' && map[E2bullet_RP + 1][E2bullet_CP] != ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = false;
		firE2 = true;
		if ((E2bullet_RP + 1) == (ethan_hunt_RP - 1) && (E2bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b2_direction == 'e' && map[E2bullet_RP][E2bullet_CP + 1] != ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = false;
		firE2 = true;
		if ((E2bullet_RP) == (ethan_hunt_RP) && (E2bullet_CP + 1) == (ethan_hunt_CP - 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b2_direction == 'w' && map[E2bullet_RP][E2bullet_CP - 1] != ' ') {
		map[E2bullet_RP][E2bullet_CP] = ' ';
		is_active2 = false;
		firE2 = true;
		if ((E2bullet_RP) == (ethan_hunt_RP) && (E2bullet_CP - 1) == (ethan_hunt_CP + 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	if (!is_active2)
		firE2 = true;
	if (killed_E2)
		firE2 = false;

	//enemy 3
	map[E3bullet_RP][E3bullet_CP] = '*';

	if (b3_direction == 'n' && map[E3bullet_RP - 1][E3bullet_CP] != ' ') {  // to remomve bullet if collide
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = false;
		firE3 = true;
		if ((E3bullet_RP - 1) == (ethan_hunt_RP) && (E3bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b3_direction == 's' && map[E3bullet_RP + 1][E3bullet_CP] != ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = false;
		firE3 = true;
		if ((E3bullet_RP + 1) == (ethan_hunt_RP - 1) && (E3bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b3_direction == 'e' && map[E3bullet_RP][E3bullet_CP + 1] != ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = false;
		firE3 = true;
		if ((E3bullet_RP) == (ethan_hunt_RP) && (E3bullet_CP + 1) == (ethan_hunt_CP - 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b3_direction == 'w' && map[E3bullet_RP][E3bullet_CP - 1] != ' ') {
		map[E3bullet_RP][E3bullet_CP] = ' ';
		is_active3 = false;
		firE3 = true;
		if ((E3bullet_RP) == (ethan_hunt_RP) && (E3bullet_CP - 1) == (ethan_hunt_CP + 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	if (!is_active3)
		firE3 = true;
	if (killed_E3)
		firE3 = false;

	//enemy 4
	map[E4bullet_RP][E4bullet_CP] = '*';

	if (b4_direction == 'n' && map[E4bullet_RP - 1][E4bullet_CP] != ' ') {  // to remomve bullet if collide
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = false;
		firE4 = true;
		if ((E4bullet_RP - 1) == (ethan_hunt_RP) && (E4bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b4_direction == 's' && map[E4bullet_RP + 1][E4bullet_CP] != ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = false;
		firE4 = true;
		if ((E4bullet_RP + 1) == (ethan_hunt_RP - 1) && (E4bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b4_direction == 'e' && map[E4bullet_RP][E4bullet_CP + 1] != ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = false;
		firE4 = true;
		if ((E4bullet_RP) == (ethan_hunt_RP) && (E4bullet_CP + 1) == (ethan_hunt_CP - 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b4_direction == 'w' && map[E4bullet_RP][E4bullet_CP - 1] != ' ') {
		map[E4bullet_RP][E4bullet_CP] = ' ';
		is_active4 = false;
		firE4 = true;
		if ((E4bullet_RP) == (ethan_hunt_RP) && (E4bullet_CP - 1) == (ethan_hunt_CP + 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	if (!is_active4)
		firE4 = true;
	if (killed_E4)
		firE4 = false;

	//enemy 5
	map[E5bullet_RP][E5bullet_CP] = '*';

	if (b5_direction == 'n' && map[E5bullet_RP - 1][E5bullet_CP] != ' ') {  // to remomve bullet if collide
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = false;
		firE5 = true;
		if ((E5bullet_RP - 1) == (ethan_hunt_RP) && (E5bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b5_direction == 's' && map[E5bullet_RP + 1][E5bullet_CP] != ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = false;
		firE5 = true;
		if ((E5bullet_RP + 1) == (ethan_hunt_RP - 1) && (E5bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b5_direction == 'e' && map[E5bullet_RP][E5bullet_CP + 1] != ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = false;
		firE5 = true;
		if ((E5bullet_RP) == (ethan_hunt_RP) && (E5bullet_CP + 1) == (ethan_hunt_CP - 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b5_direction == 'w' && map[E5bullet_RP][E5bullet_CP - 1] != ' ') {
		map[E5bullet_RP][E5bullet_CP] = ' ';
		is_active5 = false;
		firE5 = true;
		if ((E5bullet_RP) == (ethan_hunt_RP) && (E5bullet_CP - 1) == (ethan_hunt_CP + 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	if (!is_active5)
		firE5 = true;
	if (killed_E5)
		firE5 = false;

	//enemy 6
	map[E6bullet_RP][E6bullet_CP] = '*';

	if (b6_direction == 'n' && map[E6bullet_RP - 1][E6bullet_CP] != ' ') {  // to remomve bullet if collide
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = false;
		firE6 = true;
		if ((E6bullet_RP - 1) == (ethan_hunt_RP) && (E6bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b6_direction == 's' && map[E6bullet_RP + 1][E6bullet_CP] != ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = false;
		firE6 = true;
		if ((E6bullet_RP + 1) == (ethan_hunt_RP - 1) && (E6bullet_CP) == (ethan_hunt_CP)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b6_direction == 'e' && map[E6bullet_RP][E6bullet_CP + 1] != ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = false;
		firE6 = true;
		if ((E6bullet_RP) == (ethan_hunt_RP) && (E6bullet_CP + 1) == (ethan_hunt_CP - 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	else if (b6_direction == 'w' && map[E6bullet_RP][E6bullet_CP - 1] != ' ') {
		map[E6bullet_RP][E6bullet_CP] = ' ';
		is_active6 = false;
		firE6 = true;
		if ((E6bullet_RP) == (ethan_hunt_RP) && (E6bullet_CP - 1) == (ethan_hunt_CP + 1)) {
			life--;
			PlaySound(TEXT("life"), NULL, SND_FILENAME | SND_ASYNC);
			initialize_ethan();
		}
	}
	if (!is_active6)
		firE6 = true;
	if (killed_E6)
		firE6 = false;
}

/// ////////////////////// LEVEL FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////
void exit_doors() {
	int rand_no, rand_2;
	for (int i = 0; i < 2; i++) {       //randomly generating two gates
		rand_no = rand() % 3; //choosing random side except bottom
		switch (rand_no)
		{
		case 0:
			rand_2 = rand() % (COL - 10); //chosing random position for gate
			for (int j = rand_2; j < rand_2 + 10; j++)
			{
				map[0][j] = ' ';
			}
			break;
		case 1:
			rand_2 = rand() % (ROW - 5); //chosing random position for gate
			for (int j = rand_2; j < rand_2 + 5; j++)
			{
				map[j][0] = ' ';
			}
			break;
		case 2:
			rand_2 = rand() % (ROW - 5); //chosing random position for gate
			for (int j = rand_2; j < rand_2 + 5; j++)
			{
				map[j][COL - 1] = ' ';
			}
			break;
		}
	}
}
void level_check() {
	if ((ethan_hunt_CP - 1 == 0 || ethan_hunt_RP - 1 == 0 || ethan_hunt_CP + 1 == COL - 1))
	{
		level++;
		score += 5;
		target += 5;
		level_upgrade();
		level_initialization();
	}
}
void level_initialization() {
	if (play) {
		PlaySound(TEXT("buzzer_x"), NULL, SND_FILENAME | SND_ASYNC);
		life = 3;
		system("cls");
		initialize_ethan();
		map1();
		enemy_initialize();
		obstacles();
		exit_doors();
	}
}
void level_upgrade() {
	if (level == 1) {
		obs_num = 2;
		enemy_num = 2;
		speed = 150;
		killed_E1 = false;
		killed_E2 = false;
		killed_E3 = true;
		killed_E4 = true;
		killed_E5 = true;
		killed_E6 = true;
	}
	else if (level == 2) {
		obs_num = 3;
		enemy_num = 3;
		speed = 130;
		killed_E1 = false;
		killed_E2 = false;
		killed_E3 = false;
		killed_E4 = true;
		killed_E5 = true;
		killed_E6 = true;
	}
	else if (level == 3) {
		obs_num = 3;
		enemy_num = 4;
		speed = 100;
		killed_E1 = false;
		killed_E2 = false;
		killed_E3 = false;
		killed_E4 = false;
		killed_E5 = true;
		killed_E6 = true;
	}
	else if (level == 4) {
		obs_num = 4;
		enemy_num = 5;
		speed = 80;
		killed_E1 = false;
		killed_E2 = false;
		killed_E3 = false;
		killed_E4 = false;
		killed_E5 = false;
		killed_E6 = true;
	}
	else if (level == 5) {
		obs_num = 4;
		enemy_num = 6;
		speed = 70;
		killed_E1 = false;
		killed_E2 = false;
		killed_E3 = false;
		killed_E4 = false;
		killed_E5 = false;
		killed_E6 = false;
	}
	else if (level > 5)
		play = false;
}

/// ////////////////////// MENU FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////

void instructions() {
	ifstream ihandeler;
	string line;
	ihandeler.open("Instructions.txt");
	cout << endl;
	while (!ihandeler.eof()) {
		getline(ihandeler, line);
		cout << "\t\t\t\t\t\t\t" << line << endl;
	}
	cout << endl;
}
void scoreleader() {
	int count = 0;

	// Read existing scores from the file
	ifstream ihandeler(fileName);
	if (ihandeler.is_open()) {
		while (count < 10 && ihandeler >> names[count] >> scores[count]) {
			count++;
		}
		ihandeler.close();
	}



	// Find the lowest score index if 10 scores are already stored
	int minIndex = 0;
	if (count == 10) {
		for (int i = 1; i < count; ++i) {
			if (scores[i] < scores[minIndex]) {
				minIndex = i;
			}
		}
		// Replace the lowest score if the new score is higher
		if (score > scores[minIndex]) {
			names[minIndex] = newName;
			scores[minIndex] = score;
		}
	}
	else {
		// Add new score if less than 10 entries exist
		names[count] = newName;
		scores[count] = score;
		count++;
	}

	// Sort scores in descending order using bubble sort
	for (int i = 0; i < count - 1; ++i) {
		for (int j = 0; j < count - i - 1; ++j) {
			if (scores[j] < scores[j + 1]) {
				swap(scores[j], scores[j + 1]);
				swap(names[j], names[j + 1]);
			}
		}
	}

	// Write the top 10 scores back to the file
	ofstream ohandeler(fileName);
	if (ohandeler.is_open()) {
		for (int i = 0; i < count; ++i) {
			ohandeler << names[i] << " " << scores[i] << "\n";
		}
		ohandeler.close();
	}
	else {
		cerr << "\t\t\t\t\t\t\tError: Unable to open file for writing." << endl;
	}

	cout << "\t\t\t\t\t\t\tHigh scores updated!\n";
}
void display_score() {
	int count = 10;
	ifstream ihandeler(fileName);
	cout << endl << endl << endl << endl;
	cout << "\t\t\t\t\t\t\t =======================================\n";
	cout << "\t\t\t\t\t\t\t |              HIGH SCORES            |\n";
	cout << "\t\t\t\t\t\t\t =======================================\n";
	cout << "\t\t\t\t\t\t\t |    " << left << setw(15) << "NAMES" << "|" << right << setw(13) << "SCORES" << "    |" << "\n";
	cout << "\t\t\t\t\t\t\t =======================================\n";
	if (ihandeler.is_open()) {
		for (int i = 0; i < count; ++i) {
			ihandeler >> names[i] >> scores[i];
			cout << "\t\t\t\t\t\t\t |    " << left << setw(15) << names[i] << "|" << right << setw(13) << scores[i] << "    |" << "\n";
			cout << "\t\t\t\t\t\t\t ---------------------------------------\n";
		}
		ihandeler.close();
	}
	else {
		cerr << "\t\t\t\t\t\t\tError: Unable to open file for writing." << endl;
	}
}
void name() {
	// Get new player's name
	cout << "\n\n\n\n\t\t\t\t\t\t\t=====================================================" << endl;
	cout << "\t\t\t\t\t\t\t\t      Enter Your Name: "; cin >> newName; cout << endl;
	system("cls");
}
void scoreboard() {
	ostringstream buffer;
	buffer << "\t\t\t\t\t\t\t--------------------------------------------------" << endl;
	buffer << "\t\t\t\t\t\t\t|score: " << score << "            Level:" << level << "            Lives:" << life << " | " << endl;
	buffer << "\t\t\t\t\t\t\t--------------------------------------------------" << endl;
	cout << buffer.str();
}
void win_message() {
	ostringstream buffer;
	buffer << "\t\t\t\t\t\t\t--------------------------------------------------" << endl;
	if (play == false && life == 0)
	{
		buffer << "\t\t\t\t\t\t\t|                    GAME OVER                   | " << endl;
		PlaySound(TEXT("gameover"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (play == false && life > 0){
		buffer << "\t\t\t\t\t\t\t|                     YOU WIN                    |" << endl;
		PlaySound(TEXT("win"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else
		buffer << "\t\t\t\t\t\t\t|                                                 |" << endl;
	buffer << "\t\t\t\t\t\t\t--------------------------------------------------" << endl;
	cout << buffer.str();

}
void bye() {
	cout << endl << endl << endl << endl;
	PlaySound(TEXT("end"), NULL, SND_FILENAME | SND_ASYNC);
	cout << "\t\t\t\t\t\t\t===================================================" << endl;
	cout << "\t\t\t\t\t\t\t|                  Hope You Enjoyed               |" << endl;
	cout << "\t\t\t\t\t\t\t|                    See You Soon!                |" << endl;
	cout << "\t\t\t\t\t\t\t|                       BYE BYE                   |" << endl;
	cout << "\t\t\t\t\t\t\t===================================================" << endl;
}
void menu() {
	ostringstream buffer;
	buffer << "\t\t\t\t\t\t\t\033[32m==================================================" << endl;
	buffer << "\t\t\t\t\t\t\t\033[32m|               MISSION IMPOSSIBLE               |" << endl;
	buffer << "\t\t\t\t\t\t\t\033[32m==================================================" << endl;
	buffer << "\t\t\t\t\t\t\t\033[32m|                    WELCOME                     |" << endl;
	buffer << "\t\t\t\t\t\t\t\033[32m==================================================" << endl << endl;
	buffer << "\t\t\t\t\t\t\tMAXIMIZE YOUR CONSOLE! &\n\n\t\t\t\t\t\t\tPLEASE Choose one of the followings! " << endl;
	buffer << "\t\t\t\t\t\t\t1. Start Game\n\t\t\t\t\t\t\t2. How to Play\n\t\t\t\t\t\t\t3. Leader Board\n\t\t\t\t\t\t\t4. Exit" << endl;
	buffer << endl << "\t\t\t\t\t\t\tYour CHOICE: ";
	cout << "\033[H" << buffer.str();
	cin >> choice;
}
void play_againF() {
	system("cls");
	map1();
	level = 1;
	life = 3;
	target = 0;
	play = true;
	score = 0;
	level_upgrade();
	enemy_initialize();
	initialize_ethan();
}

/// ////////////////////// GENERAL FUNCTIONS //////////////////// /////////////////////////////////////////// ////////////////////////////////////////
void obstacles()
{
	//srand(time(0));
	for (int i = 1; i <= obs_num; i++)
	{
		int start = 1, end1 = ROW - 2;
		int end2 = COL - 7;
		int obs_RP = start + rand() % (end1 - start + 1);
		int obs_CP = start + rand() % (end2 - start + 1);
		if (map[obs_RP][obs_CP] == ' ' &&
			map[obs_RP][obs_CP + 1] == ' ' &&
			map[obs_RP][obs_CP + 2] == ' ' &&
			map[obs_RP][obs_CP + 3] == ' ' &&
			map[obs_RP][obs_CP + 4] == ' ' &&
			map[obs_RP][obs_CP + 5] == ' ' &&
			map[obs_RP][obs_CP + 6] == ' ')
		{
			if (level == 1 || level == 2) {
				map[obs_RP][obs_CP] = '[';
				map[obs_RP][obs_CP + 1] = '-';
				map[obs_RP][obs_CP + 2] = '-';
				map[obs_RP][obs_CP + 3] = '-';
				map[obs_RP][obs_CP + 4] = ']';
			}
			else if (level == 3 || level == 4) {
				map[obs_RP][obs_CP] = '[';
				map[obs_RP][obs_CP + 1] = '-';
				map[obs_RP][obs_CP + 2] = '-';
				map[obs_RP][obs_CP + 3] = '-';
				map[obs_RP][obs_CP + 4] = '-';
				map[obs_RP][obs_CP + 5] = ']';
			}
			else if (level == 5) {
				map[obs_RP][obs_CP] = '[';
				map[obs_RP][obs_CP + 1] = '-';
				map[obs_RP][obs_CP + 2] = '-';
				map[obs_RP][obs_CP + 3] = '-';
				map[obs_RP][obs_CP + 4] = '-';
				map[obs_RP][obs_CP + 5] = '-';
				map[obs_RP][obs_CP + 6] = ']';
			}
		}
		else
			i--;
	}
}
void map1() {
	//initializiation by space
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++)
			map[i][j] = ' ';
	}
	//boundaries
	for (int i = 0; i < ROW; i++) {
		map[i][0] = '|';
		map[i][COL - 1] = '|';
	}
	for (int i = 0; i < COL; i++) {
		map[0][i] = '-';
		map[ROW - 1][i] = '-';
	}
}
void print_map() {
	ostringstream buffer;
	moveCursorToTop();
	//printing map

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (j == 0)
				buffer << "\t\t\t\t\t\t\t";
			buffer << map[i][j];
		}
		buffer << endl;
	}
	cout << buffer.str();
	buffer.clear();
	buffer.str("");
}

void hideCursor() {    // hide the cursor
	cout << "\033[?25l" << flush;
}
void showCursor() {	  // Show the cursor
	cout << "\033[?25h\033[?25J" << flush;
}
void moveCursorToTop() { // Move the cursor to the top-left corner
	cout << "\033[H" << flush;
}