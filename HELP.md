# Simple Line Editor — Help

This program is a command-line text editor. It stores a document one line at a time.

## Compile and run
```bash
gcc line_editor.c -o line_editor
./line_editor
```
On Windows with MinGW:
```bash
gcc line_editor.c -o line_editor.exe
line_editor.exe
```

## Commands

| Command | Purpose | Example |
|---|---|---|
| `insert <line> <text>` | Adds text at the given line number. Existing lines move down. | `insert 1 Hello world` |
| `delete <line>` | Removes the selected line. | `delete 1` |
| `display` | Shows all lines with line numbers. | `display` |
| `save <filename>` | Saves all lines to a `.txt` file. | `save notes.txt` |
| `load <filename>` | Loads lines from a text file into the current document. | `load notes.txt` |
| `search <word or phrase>` | Finds matching lines and prints their line numbers. | `search hello` |
| `count` | Shows the number of lines and words. | `count` |
| `help` | Displays the command list. | `help` |
| `quit` | Exits the editor. | `quit` |

Line numbers start from 1. To insert at the end, use the current line count plus 1.
