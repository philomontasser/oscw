#include <iostream>
#include <filesystem>
#include <map>
#include <fstream>

using namespace std;
using namespace std::filesystem;

void processDirectory(const path& dir, map<size_t, size_t>& bins) {
    for (const auto& entry : directory_iterator(dir)) {
        if (is_regular_file(entry.path())) {
            size_t fileSize = file_size(entry.path());
            size_t binIndex = fileSize / 1024;
            bins[binIndex]++;
        }
        else if (is_directory(entry.path())) {
            processDirectory(entry.path(), bins);
        }
    }
}

int main() {
    string inputPath;
    cout << "Enter directory path: ";
    cin >> inputPath;

    map<size_t, size_t> bins;

    processDirectory(inputPath, bins);

    ofstream outFile("output.csv");

    outFile << "Range Start (bytes),Range End (bytes),File Count\n";

    for (const auto& [bin, count] : bins) {
        size_t start = bin * 1024;
        size_t end = start + 1023;
        outFile << start << "," << end << "," << count << "\n";
    }

    outFile.close();

    cout << "Results saved to output.csv\n";

    return 0;
}