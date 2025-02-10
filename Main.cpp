#include "Merger.h"

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string folderPath = "D:\\Prepration\\Fresh Start\\BestEx_Research\\SmallData";  // Change this to your folder path
    std::string outputFilePath = "D:\\Prepration\\Fresh Start\\BestEx_Research\\Output\\Result.txt"; // Change this to your output folder
    Merger merger(folderPath, std::move(outputFilePath));

    merger.mergeAndSort();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken: " << elapsed.count() << " seconds\n";

    return 0;
}