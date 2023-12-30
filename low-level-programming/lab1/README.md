# Low Level Programming Lab 1

## Introduction

This repository contains the lab work for a Low Level Programming course, based on the tasks outlined in [this document](https://docs.yandex.ru/docs/view?url=ya-disk-public%3A%2F%2FkzHFa%2BTQft6ptBTPAuwzIQDIbD%2Ft9f1ueSb1XZmK9%2BjvFnr5Jvjen655JEbBd4XXq%2FJ6bpmRyOJonT3VoXnDag%3D%3D%3A%2Fllp-2023-tasks.pdf&name=llp-2023-tasks.pdf). The main task is to build a simple file-based database system.

The core idea of this project is to effectively read and write to a file using a combination of offsets and a linked list data structure for organizing data within tables.

## Installation

Before running the project, ensure the following prerequisites are installed:

1. **CMake (Must Have):** Essential for building the project. Visit the [CMake official website](https://cmake.org/) for installation instructions.

2. **Python, Pandas, and Matplotlib (Optional):** Required only if you want to perform performance analysis. Visit [Python's official website](https://www.python.org/), [Pandas documentation](https://pandas.pydata.org/), and [Matplotlib documentation](https://matplotlib.org/) for more information.

## Running the Tests

To run the tests, execute the following commands:

```bash
git clone https://github.com/LeTrongDat/itmo.git
cd itmo/low-level-programming
mkdir lab1/build
cd lab1/build
cmake ..
make 
./tests/test_operations && ./tests/test_operations_performance
cd ..
```

## Optional Analysis

If you wish to run the performance analysis, execute the following command:

```bash
python analysis.py
```

## Disclaimer

This project is intended for educational purposes only and should not be used in any production environment.