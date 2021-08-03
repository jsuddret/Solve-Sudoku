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

void fill_grid(vector<vector<int>> &v, vector<vector<int>> s) {
	srand(unsigned(time(0)));
	vector<int> row;
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_rows; j++) {
			if (!s[i][j]) {
				vector<int> def{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
				random_shuffle(def.begin(), def.end());
				vector<int> temp{0};
				row.clear();
				row = get_row(v, i);
				concatenate(temp, def);
				concatenate(temp, row);
				temp = erase_duplicates(temp);
				i % 2 == 0 ? v[i][j] = temp[temp.size()-1] : v[i][j] = temp[0];
			}
		}
	}
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

void swap_elements(vector<int> &v, int x1, int x2) {
	vector<int>::iterator ix1 = find(v.begin(), v.end(), x1);
	int i1 = distance(v.begin(), ix1);
	vector<int>::iterator ix2 = find(v.begin(), v.end(), x2);
	int i2 = distance(v.begin(), ix2);
	v[i1] = x2;
	v[i2] = x1;
}

int swap(vector<vector<int>> &v, vector<int> b, vector<int> c, int i, int x) {
	vector<int> new_row;
	vector<int> b_prime;
	vector<int> c_prime;
	for (int j = 0; j < v[i].size(); j++) {
		b_prime = get_block(v, i, j);
		c_prime = get_column(v, j);
		if (count(b.begin(), b.end(), v[i][j]) == 0 && count(c.begin(), c.end(), v[i][j] == 0)) {
			if (count(b_prime.begin(), b_prime.end(), x) == 0 && count(c_prime.begin(), c_prime.end(), x) == 0) {
				swap_elements(v[i], x, v[i][j]);
				return 1;
			}
		}
	}
	return 0;
}

// sudoku algorithm
int sudoku(vector<vector<int>> &v, vector<vector<int>> s) {
	fill_grid(v, s);
	for (int i = 0; i < num_rows; i++) {
		vector<int> temp_row = get_row(v, i);
		for (int j = 0; j < num_rows; j++) {
			vector<int> temp_block = get_block(v, i, j);
			vector<int> temp_col = get_column(v, j);
			if (count(temp_block.begin(), temp_block.end(), v[i][j]) > 1 || count(temp_col.begin(), temp_col.end(), v[i][j]) > 1) {
				swap(v, temp_block, temp_col, i, v[i][j]);
			}
		}
	}
	return 0;
}

void write_grid(vector<vector<int>> &v, vector<vector<int>> &s, string f) {
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
				s[i][j] = 1;
			}
		}
	}
}

int main() {
	// define 2D vectors
	vector<vector<int>> grid(num_rows, {0, 0, 0, 0, 0, 0, 0, 0, 0});
	vector<vector<int>> secured(num_rows, {0, 0, 0, 0, 0, 0, 0, 0, 0});
	// prompt user
	if (prompt(1, grid)) {
		write_grid(grid, secured, file);
	}
	// run algorithm
	sudoku(grid, secured);
	// show solution
	prompt(0, grid);
}