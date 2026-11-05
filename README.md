# Simple Line Editor in C

## Project description
A small terminal-based line editor. Users can insert, delete, and display lines in a document. It also supports saving, loading, searching, and counting words.

## Team members
- Add your name
- Add other team members (if applicable)

## Features implemented
**Core features**
- Insert a line at a selected line number
- Delete a selected line
- Display the document with line numbers
- Save and load a `.txt` file

**Bonus features**
- Search for a word or phrase
- Count lines and words

## Data structure
The program uses a **dynamic array of strings**. Each array element stores one line of text.

The array starts with a capacity of 10 lines. When it becomes full, `realloc()` doubles its capacity. This avoids choosing a fixed maximum number of lines in advance.

## Compile and run
```bash
gcc line_editor.c -o line_editor
./line_editor
```
For Windows with MinGW:
```bash
gcc line_editor.c -o line_editor.exe
line_editor.exe
```

See [HELP.md](HELP.md) for command syntax and examples.
