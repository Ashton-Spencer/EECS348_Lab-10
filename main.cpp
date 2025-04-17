#include <iostream>
#include <fstream>
#include "calculator.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    ifstream input_file(argv[1]);
    string line;
    while (getline(input_file, line)) {
        if (is_valid_double(line)) {
            string result = add_string_numbers(line, "-123.456");
            cout << result << endl;
        } else {
            cout << "Invalid number: " << line << endl;
        }
    }
    return 0;
} 