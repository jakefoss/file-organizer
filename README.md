# File Sorter

A command-line utility that automatically organizes files into folders based on their file extensions.

## Features

- **Automatic Organization** - Sorts files by extension into separate folders
- **Dry-Run Mode** - Preview changes before executing (enabled by default)
- **Copy or Move** - Choose between copying files or moving them
- **Custom Paths** - Specify source and destination directories
- **Safe Operation** - Skips the output folder to prevent recursion

## Usage

```bash
./file-sorter [options]
```

### Default Behavior

Run without arguments to sort files in the current directory:
```bash
./file-sorter
```
- Scans current directory
- Creates `sorted/` folder with subfolders by extension
- Runs in **dry-run mode** (preview only, no changes)

### Options

| Option | Description |
|--------|-------------|
| `--src <path>` | Source directory to scan (default: current directory) |
| `--dest <path>` | Destination folder for sorted files (default: `./sorted`) |
| `--dry-run` | Preview mode - show what would happen without making changes |
| `--no-dry-run` | Execute the file operations (move/copy files) |
| `--copy` | Copy files instead of moving them |

### Examples

**Preview sorting current directory:**
```bash
./file-sorter --dry-run
```

**Sort files from a specific folder:**
```bash
./file-sorter --src /path/to/messy/folder --no-dry-run
```

**Copy files instead of moving them:**
```bash
./file-sorter --copy --no-dry-run
```

**Custom source and destination:**
```bash
./file-sorter --src ~/Downloads --dest ~/Documents/organized --no-dry-run
```

## How It Works

1. **Scans** the source directory for files
2. **Groups** files by their extension (jpg, pdf, txt, etc.)
3. **Creates** subfolders for each extension type
4. **Moves/Copies** files into their respective folders

### File Organization

```
Before:
Downloads/
├── photo.jpg
├── document.pdf
├── notes.txt
└── image.png

After (with default settings):
Downloads/
├── sorted/
│   ├── jpg/
│   │   └── photo.jpg
│   ├── pdf/
│   │   └── document.pdf
│   ├── txt/
│   │   └── notes.txt
│   └── png/
│       └── image.png
```

Files without extensions are placed in a `_noext/` folder.

## Safety Features

- **Dry-run by default** - Must explicitly use `--no-dry-run` to make changes
- **Skips output folder** - Won't move the `sorted/` folder into itself
- **Skip existing files** - In copy mode, won't overwrite existing files
- **Error handling** - Catches and reports file operation errors

## Future Enhancements

- [ ] Recursive subdirectory scanning
- [ ] Custom grouping rules (images, documents, media, etc.)
- [ ] Undo functionality
- [ ] Progress bar for large operations
- [ ] Configuration file support
- [ ] File size and date-based organization
- [ ] Duplicate file detection
- [ ] GUI version (desktop application)
