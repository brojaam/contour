## Setup Instructions

### Prerequisites:
#### - Ubuntu or compatible Linux distribution
#### - g++ (version ≥ 10, for C++20 support)
#### - cmake (version ≥ 3.10)
#### - make
#### - git
#### - GoogleTest (included as a Git submodule)


### Clone the repository with submodules
```bash
git clone --recurse-submodules https://github.com/brojaam/contour.git

cd contour
```


### Create build directory, configure and build
```bash
cmake -B build

cmake --build build
```

### Run the unit tests
```bash
./build/ContourTests
```