# Text File Compression and Decompression

This project provides a simple implementation of text file compression and decompression using the LZW (Lempel-Ziv-Welch) algorithm in C. The program allows users to compress text files into a custom format and decompress them back to their original content.

## Features

- Compress text files using LZW algorithm
- Decompress compressed files
- Simple command-line interface

## Prerequisites

- Windows operating system
- GCC compiler for C

## How to Use

1. **Clone the repository:**
   sh
   
   git clone https://github.com/Mohimor/Text-file-compression.git
   cd Text-file-compression
Compile the code:

sh

gcc -o compress_decompress main.c
Run the program:

sh

./compress_decompress
Follow the on-screen instructions:

Option 1: Compress a text file

Option 2: Decompress a text file

Option 3: Exit the program

Example Usage:

Enter the input file path when prompted.

The compressed or decompressed file will be saved in the same directory.

Code Overview
The main functionalities are provided by the following functions:

compressTextFile(const char* inputFilename): Compresses a text file using the LZW algorithm and saves the compressed content.

decompressTextFile(const char* inputFilename): Decompresses a compressed file using the LZW algorithm and restores the original content.
