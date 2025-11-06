#include <fstream> // For file stream operations (ifstream)
#include <iostream> // For input/output operations (cout, cin)
#include <string>   // For string manipulation
#include <sstream>  // For string stream operations (istringstream)

int main() {
    std::ifstream inputFile("input"); // Replace "your_file.txt" with your file name

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1; // Indicate an error
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Process each line here
        std::string token;
        std::istringstream lineStream(line); // Create a string stream from the line

        while (std::getline(lineStream, token, ',')) { // Use ',' as a delimiter
            // Process each token here
            std::cout << "Token: " << token << std::endl;
        }
    }

}
