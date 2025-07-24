#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    // Open the file
    std::ifstream file("C:\\Users\\danxl\\Downloads\\lab1_canada.csv");
    
    if (!file) {
        std::cerr << "Error opening file\n";
        return 1;
    }


    
    std::string line;

    std::getline(file, line, ',');

    std::cout << line;

    std::getline(file, line, ',');

    std::cout << line;

    // while(std::getline(file, line, ',')){

    //     std::cout << line << "\n";


    // }
    
    // Read each line from the file
    // while (std::getline(file, line)) {
    //     // Create a stringstream from the line
    //     std::stringstream ss(line);
    //     std::string token;
        
    //     // Read each component separated by comma
    //     while (std::getline(ss, token, ',')) {
    //         // Print each component
    //         std::cout << token << "\n";
    //     }
    // }
    
    file.close();
    
    return 0;
}