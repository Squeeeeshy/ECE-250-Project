#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

class TimeSeries {
private:
    string series_name;
    string series_code;
    int* years;
    double* data;
    int size;
    int capacity;

    void resize(int new_capacity) {
        if (new_capacity < 2) new_capacity = 2;

        int* new_years = new int[new_capacity];
        double* new_data = new double[new_capacity];

        for (int i = 0; i < size; ++i) {
            new_years[i] = years[i];
            new_data[i] = data[i];
        }

        delete[] years;
        delete[] data;

        years = new_years;
        data = new_data;
        capacity = new_capacity;
    }

public:
    TimeSeries() : years(new int[2]), data(new double[2]), size(0), capacity(2) {}

    ~TimeSeries() {
        delete[] years;
        delete[] data;
    }

    bool load(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return false;

        string line;
        getline(file, line); // Skip the header line

        while (getline(file, line)) {
            stringstream ss(line);
            string country_name, country_code, series_name_field, series_code_field, value;
            getline(ss, country_name, ',');
            getline(ss, country_code, ',');
            getline(ss, series_name_field, ',');
            getline(ss, series_code_field, ',');

            series_name = series_name_field;
            series_code = series_code_field;

            for (int year = 1960; year <= 2023; ++year) {
                getline(ss, value, ',');
                if (!value.empty()) {
                    add(year, stod(value));
                }
            }
        }
        return true;
    }

    bool add(int year, double value) {
        for (int i = 0; i < size; ++i) {
            if (years[i] == year) return false;
        }
        if (size == capacity) resize(capacity * 2);
        years[size] = year;
        data[size] = value;
        ++size;
        return true;
    }

    bool update(int year, double value) {
        for (int i = 0; i < size; ++i) {
            if (years[i] == year) {
                data[i] = value;
                return true;
            }
        }
        return false;
    }

    void print() {
        if (size == 0) {
            cout << "failure" << endl;
            return;
        }

        for (int i = 0; i < size; ++i) {
            cout << "(" << years[i] << "," << data[i] << ") ";
        }
        cout << endl;
    }

    double mean() {
        if (size == 0) return 0.0;

        double sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += data[i];
        }
        return sum / size;
    }

    bool is_monotonic() {
        if (size == 0) return false;

        bool increasing = true, decreasing = true;
        for (int i = 1; i < size; ++i) {
            if (data[i] < data[i - 1]) increasing = false;
            if (data[i] > data[i - 1]) decreasing = false;
        }
        return increasing || decreasing;
    }

    void best_fit(double& m, double& b) {
        if (size == 0) {
            m = b = 0;
            return;
        }

        double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
        for (int i = 0; i < size; ++i) {
            sum_x += years[i];
            sum_y += data[i];
            sum_xy += years[i] * data[i];
            sum_x2 += years[i] * years[i];
        }

        m = (size * sum_xy - sum_x * sum_y) / (size * sum_x2 - sum_x * sum_x);
        b = (sum_y - m * sum_x) / size;
    }
};

int main() {
    TimeSeries ts;
    string command;

    while (cin >> command) {
        if (command == "LOAD") {
            string filename;
            cin >> filename;
            cout << (ts.load(filename) ? "success" : "failure") << endl;
        } else if (command == "PRINT") {
            ts.print();
        } else if (command == "ADD") {
            int year;
            double value;
            cin >> year >> value;
            cout << (ts.add(year, value) ? "success" : "failure") << endl;
        } else if (command == "UPDATE") {
            int year;
            double value;
            cin >> year >> value;
            cout << (ts.update(year, value) ? "success" : "failure") << endl;
        } else if (command == "MEAN") {
            double mean = ts.mean();
            if (mean == 0.0) {
                cout << "failure" << endl;
            } else {
                cout << "mean is " << mean << endl;
            }
        } else if (command == "MONOTONIC") {
            if (ts.is_monotonic()) {
                cout << "series is monotonic" << endl;
            } else {
                cout << "series is not monotonic" << endl;
            }
        } else if (command == "FIT") {
            double m, b;
            ts.best_fit(m, b);
            if (m == 0 && b == 0) {
                cout << "failure" << endl;
            } else {
                cout << "slope is " << m << " intercept is " << b << endl;
            }
        } else if (command == "EXIT") {
            break;
        }
    }

    return 0;
}