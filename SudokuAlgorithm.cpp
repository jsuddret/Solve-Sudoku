#include <algorithm>
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

void concatenate(vector<int> &v1,vector<int> &v2){
	v1.insert(v1.end(), v2.begin(), v2.end());
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

float gen(int i) {
	// return random number, limited by a number, i
	srand(time(0));
	return rand() % i;
}

void print_2d_vector(vector<vector<int>> v) {
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_rows; j++) {
			cout << v[i][j];
		}
		cout << endl;
	}
}

void print_vector(vector<int> &v) {
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << endl;
	}
}


bool prompt(bool p, vector<vector<int>> v) {
	// open file
	ofstream file_write(file, std::ofstream::out);
	if (file_write.is_open()) {
		p ? file_write << "Enter known values, save, and close." << "\n" : file_write << "One possible solution is:" << "\n";
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

// extract column by index
void get_column(vector<int> &c, vector<vector<int>> v, int i) {
	for (int j = 0; j < num_rows; j++) {
		c.push_back(v[j][i]);
	}
}
	
//extract row by index
void get_row(vector<int> &r, vector<vector<int>> v, int i) {
	for (int j = 0; j < num_rows; j++) {
		r.push_back(v[i][j]);
	}	
}

// sudoku algorithm
int sudoku(vector<vector<int>> &v) {
	vector<int> new_vector{0};
	vector<int> def{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	vector<int> col;
	vector<int> row;
	for (int i = 0; i < num_rows; i++) {
		row.clear();
		get_row(row, v, i);
		for (int j = 0; j < num_rows; j++) {
			if (v[i][j] == 0) {
				col.clear();
				get_column(col, v, j);
				concatenate(new_vector, def);
				concatenate(new_vector, col);
				concatenate(new_vector, row);
				// delete elements with more than one occurence
				print_vector(new_vector);
				return 0;
				v[i][j] = new_vector[gen(new_vector.size())];
			}
		}
	}
	return 0;
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