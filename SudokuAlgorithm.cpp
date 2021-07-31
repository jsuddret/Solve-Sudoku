#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

// global variables
string file = "Sudoku.txt";
int num_rows = 9;

void concatenate(vector<int> &v1,vector<int> v2){
	v1.insert(v1.end(), v2.begin(), v2.end());
}

int div_three(int x) {
	int temp;
	x % 3 == 0 ? temp = x : temp = abs(x % 3 - x);
	return temp;
}

vector<int> erase_duplicates(vector<int> &v) {
	vector<int> d;
	for (int x : v) {
		if (count(v.begin(), v.end(), x) == 1) {
			d.push_back(x);
		}
	}
	return d;
}

int gen(int i) {
	// return random number, limited by a number, i
	srand(time(0));
	return rand() % i;
}

vector<int> get_block(vector<vector<int>> v, int i, int j) {
	vector<int> b;
	for (int k = div_three(i); k < div_three(i+3); k++) {
		for (int l = div_three(j); l < div_three(j+3); l++) {
			b.push_back(v[k][l]);
		}
	}
	return b;
}

// extract column by index
vector<int> get_column(vector<vector<int>> v, int i) {
	vector<int> c;
	for (int j = 0; j < num_rows; j++) {
		c.push_back(v[j][i]);
	}
	return c;
}
	
//extract row by index
vector<int> get_row(vector<vector<int>> v, int i) {
	vector<int> r;
	for (int j = 0; j < num_rows; j++) {
		r.push_back(v[i][j]);
	}	
	return r;
}

void print_2d_vector(vector<vector<int>> v) {
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_rows; j++) {
			cout << v[i][j] << "  ";
		}
		cout << endl;
	}
}

void print_vector(vector<int> &v) {
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;
}


bool prompt(bool p, vector<vector<int>> v) {
	// open file
	ofstream file_write(file, std::ofstream::out);
	if (file_write.is_open()) {
		p ? file_write << "Replaces spaces with known values, save, and close." << "\n" : file_write << "One possible solution is:" << "\n";
		for (int i = 0; i < num_rows; i++) {
			for (int j = 0; j < num_rows; j++) {
				p ? file_write << "[ ]" : file_write << '[' << v[i][j] << ']';
			}
			file_write << "\n";
		}
		file_write.close();
		// write to notepad
		string notepad_file = "notepad \"" + file + "\"";
		system(notepad_file.c_str());
		if (!file_write.is_open()) {
			return true;
		}
	}
	return false;
}

// sudoku algorithm
int sudoku(vector<vector<int>> &v) {
	srand(unsigned(time(0)));
	vector<int> def{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	random_shuffle(def.begin(), def.end());
	vector<int> block;
	vector<int> col;
	vector<int> row;
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_rows; j++) {
			if (v[i][j] == 0) {
				vector<int> temp{0};
				block.clear();
				row.clear();
				col.clear();
				block = get_block(v, i, j);
				row = get_row(v, i);
				col = get_column(v, j);
				concatenate(temp, def);
				concatenate(temp, block);
				concatenate(temp, col);
				concatenate(temp, row);
				temp = erase_duplicates(temp);
				print_vector(temp);
				
				
				
				if (i == 5 && j == 2) {
					return 1;
				}
				
				
				i % 2 == 0 ? v[i][j] = temp[0] : v[i][j] = temp[temp.size()-1];
			}
		}
	}
	return 0;
}

bool swap(vector<int> b, vector<int> c, vector<int> &r, int j) {
	bool legal = false;
	for (int i = 0; i < j; i++) {
		retun false;
	}
	return legal;
}

void write_grid(vector<vector<int>> &v, string f) {
	string filename(file);
	vector<string> lines;
	string line;
	ifstream input_file(filename);
	getline(input_file, line);
	// read file line by line and push to vector
	while (getline(input_file, line)) {
			lines.push_back(line);
	}
	input_file.close();
	// add values into 2D vector
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_rows; j++) {
			if (lines[i][1+3*j] != ' ') {
				// convert ASCII to integer
				v[i][j] = int(lines[i][1+3*j])-48;
			}
		}
	}
}

int main() {
	// define 2D vector
	vector<vector<int>> grid(num_rows, {0, 0, 0, 0, 0, 0, 0, 0, 0});
	// prompt user
	if (prompt(1, grid)) {
		write_grid(grid, file);
	}
	// run algorithm
	sudoku(grid);
	// show solution
	prompt(0, grid);
}