/*
  _____ _______ _    _
 |_   _|__   __| |  | |
   | |    | |  | |  | |
   | |    | |  | |  | |
  _| |_   | |  | |__| |
 |_____|  |_|   \____/

@Author
Student Name: Orçun Özdemir
Student ID: 150140121
Date: 20/04/2018

This code requires ISO C++ 2011 standard.
Compile it using -std=c+11 or -std=gnu++11 parameter, otherwise you will get an error!

g++ main.cpp -o main -std=c++11
g++ main.cpp -o main -std=gnu++11

*/

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
#include <chrono>
#define SIZE 256  // Using it for file reading.

using namespace std;

class ball {
  public:
    int x;
    int y;
    int z;
};

class tools {
  public:
    void getfromfile (const string, vector<ball> &);
    float closest (const ball [], const int);
    float divideandConquer (const ball [], const ball [], const int);
    int distance_calc;
    tools() : distance_calc(0) {} // Initialize from 0 to find number of total calculations.
  private:
    float calculate (const ball, const ball);
    float midlaneClosest (const ball [], const int, const float);
};

bool compare_x (const ball, const ball);
bool compare_y (const ball, const ball);

tools t;

int main(int argc, char const *argv[]) {

  if (argc == 2) {
    string arg_s = argv[1];

    if (strcmp(argv[1], "-h") == 0)
      cout << "Example Usage: ./program data.txt";

    else if (arg_s.find_last_of(".txt")){ // Only accept txt files as input.
      vector<ball> v;
      t.getfromfile(argv[1], v);  // Read values and write into a vector.

      vector<ball> vx(v.begin(), v.end());  // Copy v into vx.
      vector<ball> vy(v.begin(), v.end());  // Copy v into vy.

      sort(vx.begin(), vx.end(), compare_x);  // Sort vx in terms of x coordinates. O(nlogn) sorting complexity.
      sort(vy.begin(), vy.end(), compare_y);  // Sort vy in terms of y coordinates. O(nlogn) sorting complexity.

      ball va[v.size()];  // Initialize "va" array to copy vector and use it in recursive calls.
      copy(v.begin(), v.end(), va);

      ball vax[vx.size()];  // Initialize "vax" array to copy vector and use it in recursive calls.
      copy(vx.begin(), vx.end(), vax);

      ball vay[vy.size()];  // Initialize "vay" array to copy vector and use it in recursive calls.
      copy(vy.begin(), vy.end(), vay);

      auto start0_time = chrono::high_resolution_clock::now();
      cout << endl << "The distance is (Bruteforce): " << t.closest(va, v.size()) << endl;
      auto end0_time = chrono::high_resolution_clock::now();

      cout << "Number of total distance calculations is (Bruteforce): ";
      cout << t.distance_calc << endl;
      cout << "Elapsed time (Bruteforce): " << double(chrono::duration_cast<chrono::microseconds>(end0_time - start0_time).count()) << " microseconds." << endl << endl;

      t.distance_calc = 0;

      auto start_time = chrono::high_resolution_clock::now();
      cout << "The distance is (Divide & Conquer): " << t.divideandConquer(vax, vay, v.size()) << endl;
      auto end_time = chrono::high_resolution_clock::now();
      cout << "Number of total distance calculations is (Divide & Conquer): " << t.distance_calc << endl;
      cout << "Elapsed time (Divide & Conquer): " << double(chrono::duration_cast<chrono::microseconds>(end_time - start_time).count()) << " microseconds." << endl << endl;
    }

    else
    	cout << "Wrong command!" << endl << "Use -h parameter to see help.";
	}

	else {
		cout << "Wrong command!" << endl << "Use -h parameter to see help.";
	}

    return 0;
}

bool compare_x (const ball fst, const ball snd) {
  return fst.x <= snd.x;
}

bool compare_y (const ball fst, const ball snd) {
  return fst.y <= snd.y;
}

void tools::getfromfile (const string fname, vector<ball> &v) {
  ball *ptr;
  ptr = new ball;
  ifstream input;
	input.open(fname.c_str());

		if (input.fail()) {
				cout << "File couldn't be opened!\n";
        exit(0);
		}

    input.ignore(SIZE,'\n');

    while (!(input.eof())) {
  		input >> ptr->x;
      input >> ptr->y;
      input >> ptr->z;
  		input.ignore(SIZE,'\n');
      v.push_back(*ptr);
  	}

    v.pop_back(); // To get rid of the blank lines at the end of the text files.
    delete ptr;
  	input.close();
}

float tools::calculate (const ball a, const ball b) {
  distance_calc++;  // Calculating total calculation times.
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

float tools::closest (const ball v[], const int n) {
  float closestDist = INFINITY; // Give a very big number to find minimum distances.

  for (int x = 0; x < n; x++) {
    for (int y = x + 1; y < n; y++) {
      if (calculate(v[x], v[y]) < closestDist)
        closestDist = calculate(v[x], v[y]);
    }
  }

  return closestDist;
}

float tools::midlaneClosest (const ball v[], const int length, const float currentMin)
{
    float newMin = currentMin;

    for (int a = 0; a < length; ++a)
        for (int b = a + 1; (b < length) && ((v[b].y - v[a].y) < newMin); ++b)
            if (calculate(v[a],v[b]) < newMin)
                newMin = calculate(v[a], v[b]);

    return newMin;
}

float tools::divideandConquer (const ball vx[], const ball vy[], const int size)
{

    if (size <= 3)
        return closest(vx, size);

    int intermediate = size / 2;
    ball midPoint = vx[intermediate];

    ball leftpart[intermediate + 1];
    ball rightpart[size - intermediate - 1];

    int left = 0, right = 0;

    for (int a = 0; a < size; a++)
    {
      if (vy[a].x <= midPoint.x)
         leftpart[left++] = vy[a];
      else
         rightpart[right++] = vy[a];
    }

    float lengthl = divideandConquer(vx, leftpart, intermediate);
    float lengthr = divideandConquer(vx + intermediate, rightpart, size - intermediate);

    float smallest = min(lengthl, lengthr);

    ball midlane[size];
    int midlength = 0;
    for (int a = 0; a < size; a++)
        if (abs(vy[a].x - midPoint.x) < smallest){
            midlane[midlength] = vy[a];
            midlength++;
        }

    return min(smallest, midlaneClosest(midlane, midlength, smallest));
}
