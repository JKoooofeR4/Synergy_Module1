#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int field[100][100];	// -1 --> bomb ; 0-8 --> cell
int field_p[100][100];	// -1 --> flag ; 0-8 --> cell is open ; -2 --> cell is close

const int LINES = 9, COLUMNS = 9, BOMBS = 10;

void print_field() {
	system("cls");
	cout << "  ";

	for (int i = 0; i < COLUMNS; ++i) {
		cout << " " << i + 1;

		if (i + 1 < 10)
			cout << ' ';
	}
	cout << endl;

	for (int i = 0; i < LINES; i++) {
		cout << i + 1 << " ";

		if (i + 1 < 10)
			cout << ' ';

		for (int j = 0; j < COLUMNS; j++) {
			if (field_p[i][j] == -2) cout << "_  ";
			else if (field_p[i][j] == -1) cout << "f  ";
			else if (field_p[i][j] == -3) cout << "*  ";
			else cout << field_p[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}



void dfs(int x, int y) {
	if (x < 0 || x >= LINES || y < 0 || y >= COLUMNS || field_p[x][y] > -2) return;
	field_p[x][y] = field[x][y];
	if (field[x][y] > 0) return;
	for (int i2 = x - 1; i2 <= x + 1; ++i2) {
		for (int j2 = y - 1; j2 <= y + 1; ++j2) {
			if (i2 != x || j2 != y) dfs(i2, j2);
		}
	}
}



bool open_cell(int x, int y) {
	if (field[x][y] == -1)
		return false;

	if (field[x][y] > 0) {
		field_p[x][y] = field[x][y];

		return true;
	}
	dfs(x, y);
}



bool is_win() {
	int opened = 0;
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (field_p[i][j] >= 0) opened++; //0 1 2 3
		}
	}
	return (LINES * COLUMNS - BOMBS == opened); //9*9-10
}



void end_game(bool is_win = false) {
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (field_p[i][j] == -1)
				field_p[i][j] = -2;

			if (field[i][j] == -1)
				field_p[i][j] = -3;
		}
	}
	print_field();

	cout << "You " << (is_win ? "win" : "lost") << ", wanna start new game?:\n";
	string s;
	cin >> s;
}



int main() {
	while (true) {
		srand(time(nullptr));

		for (int i = 0; i < LINES; i++) {
			for (int j = 0; j < COLUMNS; j++) {
				field[i][j] = 0;
				field_p[i][j] = -2;
			}
		}

		for (int i = 0; i < BOMBS; ++i) {
			while (true) {
				int x = rand() % LINES;
				int y = rand() % COLUMNS;

				if (field[x][y] != -1) {
					field[x][y] = -1;

					break;
				}
			}
		}

		for (int i = 0; i < LINES; i++) {
			for (int j = 0; j < COLUMNS; j++) {
				if (field[i][j] != -1) {
					field[i][j] = 0;
					for (int i2 = i - 1; i2 <= i + 1; ++i2) {
						for (int j2 = j - 1; j2 <= j + 1; ++j2) {
							if (i2 >= 0 && i2 < LINES && j2 >= 0 && j2 <= COLUMNS &&
								field[i2][j2] == -1) ++field[i][j];
						}
					}
				}
			}
		}


		while (true) {
			print_field();
			cout << "Enter command: ";

			string command;
			cin >> command;

			if (command == "\\o") {
				int x, y;
				cin >> x >> y;
				--x; --y;

				if (x < 0 || x >= LINES || y < 0 || y >= COLUMNS || field_p[x][y] >= 0)
					continue;

				if (!open_cell(x, y)) {
					end_game();

					break;
				}

				if (is_win()) {
					end_game(true);
					break;
				}
			}
			else if (command == "\\f") {
				int x, y;
				cin >> x >> y;
				--x; --y;

				if (x < 0 || x >= LINES || y < 0 || y >= COLUMNS || field_p[x][y] >= 0)
					continue;

				if (field_p[x][y] == -1)
					field_p[x][y] = -2;
				else
					field_p[x][y] = -1;
			}
			else if (command == "\\n") {
				break;
			}
			else if (command == "\\q") {
				return 0;
			}
		}
	}

	return 0;
}