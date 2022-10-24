# Archiver

This project implements archiver based on Huffman coding. The program provides zipping multiple files and archive unzipping.

## Command line interface

Command line interface realized via custom commands parser.

The program provides the following interface:
* `archiver -c archive_name file1 [file2 ...]` - encode files `file1, file2, ...` and save result in `archive_name`.
* `archiver -d archive_name` - decode files from `archive_name` and save in current directory.
* `archiver -h` - print usage manual.

## Algorithm

Compression algorithm have following realization:
1. Frequency of 8-bit file symbols is being counted. Also, frequency contains filename symbols amount and service symbols `FILENAME_END=256`, `ONE_MORE_FILE=257`, `ARCHIVE_END=258`, which provides multiple files compressing. Thereby alphabet is represented in 9 bits.
1. A binary trie is being built according to the Huffman coding algorithm.
1. Each character is represented by a binary code.
1. Each code is reduced to canonical Huffman code.
1. All file symbols are being replaced with canonical code.

The decoding algorithm is generally the reverse of the encoding algorithm.

## Realization

Realization contains following components:

- Input and output streams wrappers providing the ability to read and write arbitrary (in range 1 - 32) bits amount.
- Encoding and decoding tries with necessary functionality.
- Custom priority queue template realized via heap data structure.
- Custom command line arguments parser.
- Tests for all realized units.
