# Directory Tree Viewer

A **Directory Tree Viewer** implemented in C. This program recursively lists the contents of a directory in a tree-like format, with options to display hidden files, limit depth, and show file sizes.

---

## Features
- **Tree-like structure**: Visualize directories and files with indentation and branch characters.
- **Hidden files**: Option to include hidden files in the output.
- **Depth control**: Limit the recursion depth.
- **File sizes**: Display file sizes in a human-readable format.

---

## Usage

### Command Syntax
```bash
./dtree [options] [directory]
```

### Options
| Option    | Description                                   |
|-----------|-----------------------------------------------|
| `-a`      | Show hidden files.                           |
| `-d N`    | Maximum depth (N levels). Default: unlimited.|
| `-s`      | Show file sizes.                             |
| `-h`      | Display the help message.                    |

### Example Commands
1. View the current directory as a tree:
   ```bash
   ./dtree
   ```

2. Show hidden files and limit depth to 2 levels:
   ```bash
   ./dtree -a -d 2
   ```

3. Display file sizes in the `/home/user` directory:
   ```bash
   ./dtree -s /home/user
   ```

---

## Installation

### Prerequisites
- GCC (GNU Compiler Collection)
- Linux/Unix-based environment

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/directory-tree-viewer.git
   cd directory-tree-viewer
   ```

2. Build the executable:
   ```bash
   make
   ```

3. Install the program system-wide (optional):
   ```bash
   sudo make install
   ```

4. Run the program:
   ```bash
   dtree [options] [directory]
   ```

---

## Uninstallation
To remove the installed program:
```bash
sudo make uninstall
```

---

## File Structure
```
.
├── Makefile        # Build and install instructions
├── dtree.c         # Main source code
├── README.md       # Project documentation
```

---

## Makefile Targets
| Target       | Description                           |
|--------------|---------------------------------------|
| `make`       | Compile the program.                  |
| `make clean` | Remove compiled files.                |
| `make install`| Install the program to `/usr/local/bin`. |
| `make uninstall`| Remove the installed program.        |

---


## Acknowledgments
- Inspired by the Unix `tree` command.
