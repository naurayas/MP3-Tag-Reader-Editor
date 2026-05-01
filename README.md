# MP3-Tag-Reader-Editor

## Overview
This project implements a command-line MP3 tag reader and editor in C that works with ID3v2.3 metadata. It allows users to view and modify important audio metadata such as title, artist, album, year, genre, and comments.

## Features
- View MP3 metadata (Title, Artist, Album, Year, Genre, Comment)
- Edit metadata fields
- Supports ID3v2.3
- Command-line interface for interaction

## How it Works
The program first validates whether the input MP3 file contains ID3v2.3 tags by checking the header. It then reads metadata frame by frame, extracting tag identifiers and their corresponding sizes using big-endian conversion.

For viewing, each frame is parsed and displayed in a human-readable format. For editing, the program creates a duplicate file and rewrites the frames, replacing only the selected tag while preserving all other metadata and audio content. After successful modification, the original file is replaced with the updated version.

## Concepts Demonstrated
- File handling
- ID3v2.3 frame parsing
- Endian conversion (big-endian to little endian)
- Modular programming

## Project Structure
```bash
.
├── common.c
├── common.h
├── edit.c
├── edit.h
├── main.c
├── makefile
├── view.c
└── view.h
```

## Build Instructions
### Using Makefile
```bash
make
./mp3
```

## Usage
### Sample input
Use any MP3 file with ID3c2.3 tags for testing.

### View MP3 tags
```bash
./mp3 -v sample.mp3
```
### Edit MP3 tags
```bash
./mp3 -e -t/-a/-A/-m/-y/-c "changing_text" sample.mp3
```
### Help menu
```bash
./mp3 --help
```
