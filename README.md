# Efficient Multi-File Data Merging System

## Project Overview
This project focuses on efficiently merging large-scale time-series data from multiple source files into a single, sorted output file. Each input file represents data from an independent source and is sorted by timestamp. The goal is to combine these files into one unified dataset while preserving the chronological order. If multiple entries share the same timestamp, they are further sorted based on an additional identifier (e.g., source name).

## Key Challenges Addressed
- **Large Data Volume Handling**: The total dataset spans billions of records, exceeding 100 GB in size. The solution efficiently processes this data without loading it entirely into memory.
- **File Handling Limitations**: Given system constraints on the maximum number of files that can be opened simultaneously, the implementation ensures an optimal balance between performance and resource usage.
- **Efficient Sorting & Merging**: Since individual files are pre-sorted by timestamp, an efficient multi-way merge approach is used to construct the final output while maintaining sorted order.
- **Scalability & Performance Optimization**: The solution leverages buffer-based reading and optimized data structures to minimize disk I/O and processing time.

## Implementation Highlights
- **Multi-Way Merge Algorithm**: A priority queue (min-heap) is used to track the next available record from each active file, ensuring an efficient merge process.
- **Optimized File Management**: A controlled number of files are opened concurrently to stay within system limitations. Buffered reading techniques are employed to enhance performance.
- **Threading Support (if enabled)**: Where applicable, parallel processing is used to improve efficiency, particularly in reading and managing multiple input files.
- **Configurable Execution**: The program accepts command-line arguments to specify input paths, output paths, and resource utilization parameters for adaptability to various use cases.

