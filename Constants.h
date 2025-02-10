#pragma once
namespace Constants {
    inline constexpr size_t MAX_FILES_PER_THREAD = 10; 
    inline constexpr size_t NUM_THREADS = 8;
    inline constexpr int CHUNK_LINES = 100'000'000; // 10^8 lines per file
    inline constexpr size_t MAX_OPEN_FILES = NUM_THREADS * MAX_FILES_PER_THREAD;
}