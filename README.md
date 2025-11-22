# Project Overview

**AIZOprojekt** is a C++ application designed for sorting algorithm analysis and benchmarking.  
It supports multiple sorting algorithms, various data types, and different data distributions.  
The project demonstrates efficient sorting implementations, performance measurement, and includes a unique "drunk" sorting mode that introduces controlled randomness to analyze algorithm behavior under imperfect conditions.

You can view the full project source code here:  
[https://github.com/szymoo7/AIZOprojekt](https://github.com/szymoo7/AIZOprojekt)

---

## How to Get the Code

To download the project from GitHub, open a terminal and run:

```bash
git clone https://github.com/szymoo7/AIZOprojekt.git
cd AIZOprojekt
```

---

## How to Build the Application

### Prerequisites
- CMake (version 3.26 or higher)
- C++17 compatible compiler (GCC, Clang, or MSVC)

### Building from Source

1. **Create a build directory**
   ```bash
   mkdir build
   cd build
   ```

2. **Generate build files with CMake**
   ```bash
   cmake ..
   ```

3. **Compile the project**
   ```bash
   cmake --build .
   ```

4. **The executable will be created in the build directory**
   ```bash
   ./AIZOprojekt --help
   ```

---

## Supported Sorting Algorithms

The application implements the following sorting algorithms:

| Algorithm ID | Name | Complexity (Avg) | Complexity (Worst) |
|--------------|------|------------------|-------------------|
| 0 | Bubble Sort | O(n²) | O(n²) |
| 1 | Merge Sort | O(n log n) | O(n log n) |
| 2 | Insertion Sort | O(n²) | O(n²) |
| 3 | Binary Insertion Sort | O(n²) | O(n²) |
| 4 | Quick Sort | O(n log n) | O(n²) |
| 5 | Heap Sort | O(n log n) | O(n log n) |
| 6 | Shell Sort | O(n log²n) | O(n²) |

---

## Supported Data Types

| Type ID | Data Type | Range |
|---------|-----------|-------|
| 0 | Integer | INT_MIN to INT_MAX |
| 1 | Float | FLOAT_MIN to FLOAT_MAX |
| 2 | Character | 0 to 255 (ASCII) |

---

## Data Distributions

| Distribution ID | Description |
|-----------------|-------------|
| 0 | Random (default) |
| 1 | Descending order |
| 2 | Ascending order |
| 3 | 33% sorted |
| 4 | 66% sorted |

---

## Usage Modes

### 1. FILE TEST MODE
Test sorting algorithms on data loaded from a file.

```bash
./AIZOprojekt --file <algorithm> <type> <inputFile> [outputFile]
```

**Parameters:**
- `<algorithm>` - Algorithm ID (0-6)
- `<type>` - Data type ID (0-2)
- `<inputFile>` - Path to input file containing data
- `[outputFile]` - (Optional) Path to save sorted results

**Example:**
```bash
./AIZOprojekt --file 4 0 random_numbers.txt sorted_output.txt
```

**Input File Format:**
```
100
66
36
88
...
```
The first line contains the number of elements, followed by the elements themselves.

---

### 2. BENCHMARK MODE
Generate random data and benchmark sorting performance with 100 iterations.

```bash
./AIZOprojekt --test <algorithm> <type> <size> <outputFile> [distribution]
```

**Parameters:**
- `<algorithm>` - Algorithm ID (0-6)
- `<type>` - Data type ID (0-2)
- `<size>` - Number of elements to generate
- `<outputFile>` - CSV file to save benchmark results
- `[distribution]` - (Optional) Distribution type (0-4, default: 0)

**Example:**
```bash
./AIZOprojekt --test 4 0 10000 results.csv 0
```

---

### 3. DRUNK MODE
A unique mode that introduces controlled randomness into sorting algorithms to analyze their behavior under imperfect conditions. Currently supports Bubble Sort and Quick Sort.

```bash
./AIZOprojekt --drunk <algorithm> <type> <size> <drunkLevel> <outputFile> [distribution]
```

**Parameters:**
- `<algorithm>` - Algorithm ID (0 for Bubble, 1 for Quick)
- `<type>` - Data type ID (0-2)
- `<size>` - Number of elements to generate
- `<drunkLevel>` - Level of randomness (0 = sober, higher = more random)
- `<outputFile>` - CSV file to save results
- `[distribution]` - (Optional) Distribution type (0-4, default: 0)

**Example:**
```bash
./AIZOprojekt --drunk 0 0 10000 5 drunk_results.csv 0
```

**How Drunk Mode Works:**
- Higher drunk levels introduce more random errors during sorting
- Algorithms may skip comparisons or make incorrect swaps
- After the "drunk" phase, a final "sober" pass ensures correctness
- Useful for analyzing algorithm robustness and recovery behavior

---

### 4. RUN TESTS MODE
Execute comprehensive benchmarks across all algorithms, data types, sizes, and distributions.

```bash
./AIZOprojekt --run_tests <option> <outputFile>
```

**Parameters:**
- `<option>` - Test suite option (currently only 0 is supported)
  - `0` - Run every algorithm with every data type, size, and distribution
- `<outputFile>` - CSV file to save all benchmark results

**Test Parameters:**
- Algorithms: All 6 algorithms (0-5)
- Data Types: int, float, char
- Sizes: 10,000, 20,000, 30,000, 40,000, 50,000 elements
- Distributions: Random, Descending, Ascending, 33% sorted, 66% sorted

**Example:**
```bash
./AIZOprojekt --run_tests 0 comprehensive_results.csv
```

---

### 5. HELP MODE
Display usage information and command syntax.

```bash
./AIZOprojekt --help
```

---

## Output Format

### Benchmark CSV Output
The benchmark results are saved in CSV format with the following columns:

| Column | Description |
|--------|-------------|
| RecordNumber | Sequential record identifier |
| MinTime | Minimum execution time (ms) |
| MaxTime | Maximum execution time (ms) |
| AvgTime | Average execution time (ms) |
| MedianTime | Median execution time (ms) |
| StdDev | Standard deviation |
| Algorithm | Algorithm name |
| Distribution | Data distribution type |
| Size | Number of elements |
| DataType | Data type used |
| DrunkLevel | Drunk level (0 for normal mode) |

---

## Project Structure

```
AIZOprojekt/
├── main.cpp                 # Entry point
├── ArgumentParser.h/.cpp    # Command line argument parsing
├── Sorter.h/.cpp           # Sorting algorithm implementations
├── DynamicArray.h/.cpp     # Custom dynamic array template class
├── Helper.h/.cpp           # Utility functions and test runners
├── Timer.h/.cpp            # High-resolution timer for benchmarking
├── CMakeLists.txt          # CMake build configuration
└── random_numbers.txt      # Sample input file
```

---

## Key Features

### Custom Dynamic Array
The project implements a custom `DynamicArray` template class with:
- Automatic resizing (doubles capacity when full)
- Memory-efficient shrinking (halves capacity when 1/4 full)
- Index-based access and modification
- Insert and delete operations at any position

### High-Precision Timing
Uses C++ `<chrono>` library for millisecond-precision timing of sorting operations.

### Multiple Data Distributions
Supports various data distributions to analyze algorithm performance under different conditions:
- **Random**: Completely unsorted data
- **Ascending**: Already sorted (best case for some algorithms)
- **Descending**: Reverse sorted (worst case for some algorithms)
- **Partial sorting**: 33% and 66% sorted to simulate real-world scenarios

### Statistical Analysis
Each benchmark runs 100 iterations and calculates:
- Minimum, maximum, and average execution times
- Median execution time
- Standard deviation for consistency analysis

---

## Example Workflows

### Quick Test with File Input
```bash
# Sort integers from file using Quick Sort
./AIZOprojekt --file 4 0 input.txt output.txt
```

### Performance Benchmark
```bash
# Benchmark Merge Sort on 50,000 random integers
./AIZOprojekt --test 1 0 50000 merge_benchmark.csv 0
```

### Compare Algorithms
```bash
# Run comprehensive tests to compare all algorithms
./AIZOprojekt --run_tests 0 comparison_results.csv
```

### Drunk Mode Experiment
```bash
# Test robustness of Quick Sort with drunk level 10
./AIZOprojekt --drunk 1 0 20000 10 drunk_quick.csv 0
```

---

## Implementation Highlights

### Optimized Quick Sort
The Quick Sort implementation uses tail recursion optimization to prevent stack overflow on large datasets:
```cpp
static void quickSort(DynamicArray<T>* data, int low, int high) {
    while (low < high) {
        int pi = partition(data, low, high);
        // Recur on smaller partition first to optimize stack usage
        if (pi - low < high - pi) {
            quickSort(data, low, pi - 1);
            low = pi + 1;
        } else {
            quickSort(data, pi + 1, high);
            high = pi - 1;
        }
    }
}
```

### Drunk Algorithm Mode
The drunk mode introduces controlled randomness to study algorithm behavior:
- Random pivot selection based on drunk level
- Probabilistic comparison skipping
- Occasional incorrect swaps
- Final sober pass ensures correctness

---

## Testing

The application includes built-in correctness verification:
- After each sort, the array is validated to ensure proper ordering
- Results are logged to confirm sorting accuracy
- The `isCorrect()` method verifies that each element is ≤ its successor

---

## Performance Considerations

- **Memory**: Dynamic arrays resize automatically, with ~2x overhead during growth
- **Thread Safety**: Single-threaded implementation (no concurrent sorting)
- **Large Datasets**: Quick Sort uses tail recursion to handle datasets up to system memory limits
- **Precision**: Timer resolution is in milliseconds

---

## Limitations

1. **File Input Format**: Input files must follow the specific format (size on first line, then elements)
2. **Drunk Mode**: Only available for Bubble Sort and Quick Sort
3. **Single-threaded**: No parallel sorting implementations
4. **Memory**: All data must fit in RAM (no external sorting)

---

## Future Improvements

- Add more sorting algorithms (Radix Sort, Counting Sort, Tim Sort)
- Implement parallel sorting variants
- Add visualization output for sorting process
- Support for reading various file formats (CSV, JSON)
- Add memory usage profiling
- Implement external sorting for very large datasets

---

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

---

## License

This project is available for educational and research purposes.
