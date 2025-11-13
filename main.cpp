#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

struct Options
{
    fs::path src;           // Source directory to scan.
    fs::path dest;          // Destination folder for sorted/ output.
    bool dryRun = true;     // Default to preview / print-only mode.
    bool copyMode = false;  // If true, copy instead of move.
};

Options parse_args(int argc, char **argv)
{
    Options opts;

    // Default src is wherever the program is run from.
    opts.src = fs::current_path();

    // Default dest goes inside the source folder.
    opts.dest = opts.src / "sorted";

    // Parse through provided CLI args.
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        // If user provides --src <path>...
        if (arg == "--src" && i + 1 < argc)
        {
            opts.src = fs::path(argv[++i]);   // Update src.
            opts.dest = opts.src / "sorted";  // Keep dest tied to the new src.
        }
        // If user provides --dest <path>...
        else if (arg == "--dest" && i + 1 < argc)
        {
            opts.dest = fs::path(argv[++i]); // Override dest.
        }
        else if (arg == "--dry-run")
        {
            opts.dryRun = true;              // Force preview-only mode.
        }
        else if (arg == "--no-dry-run")
        {
            opts.dryRun = false;             // Allow actual moves.
        }
        else if (arg == "--copy")
        {
            opts.copyMode = true;            // Use copy instead of move.
        }
    }

    return opts;
}

// Given a file path, return lowercase extension name.
std::string extension_group(const fs::path &p)
{
    // Get file extension.
    std::string ext = p.extension().string();

    // If no extension, return placeholder.
    if (ext.empty())
    {
        return "_noext";
    }
    else
    {
        ext.erase(0, 1); // Remove leading '.'.
        // Change extension to lowercase.
        for (char &c : ext)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return ext;
    }
}

int main(int argc, char **argv)
{
    Options opts = parse_args(argc, argv);

    fs::path src = opts.src;
    fs::path destRoot = opts.dest;
    bool dryRun = opts.dryRun;
    bool copyMode = opts.copyMode;

    // Print run configuration.
    std::cout << "----------------------------------------\n";
    std::cout << "Source:      " << src << "\n";
    std::cout << "Destination: " << destRoot << "\n";
    std::cout << "Mode:        ";
    if (dryRun)
        std::cout << "DRY-RUN";
    else if (copyMode)
        std::cout << "COPY";
    else
        std::cout << "MOVE";
    std::cout << "\n";
    std::cout << "----------------------------------------\n\n";

    try
    {
        // Iterate through top-level items in the source directory.
        for (const auto &entry : fs::directory_iterator(src))
        {
            // If file...
            if (entry.is_regular_file())
            {
                // Determine extension group (jpg, txt, pdf, etc).
                std::string group = extension_group(entry.path());

                // Destination folder for this file.
                fs::path targetDir = destRoot / group;

                // Full target path.
                fs::path targetPath = targetDir / entry.path().filename();

                // Create destination folder (only in move mode).
                if (!dryRun)
                    fs::create_directories(targetDir);

                // Display planned action.
                std::cout << "[FILE] " << entry.path().filename().string() << "\n";
                std::cout << "  group : " << group << "\n";
                std::cout << "  target: " << targetPath << "\n\n";

                // Move file if not dry-run.
                if (!dryRun)
                {
                    try
                    {
                        if (copyMode)
                        {
                            // Copy file to target path. Skip if exists.
                            fs::copy_file(entry.path(), targetPath, fs::copy_options::skip_existing);
                        }
                        else
                        {
                            // Move file to target path.
                            fs::rename(entry.path(), targetPath);
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "[MOVE ERROR] " << e.what() << '\n';
                    }
                }
            }

            // If directory...
            else if (entry.is_directory())
            {

                // Skip only the sorted/ folder inside the source folder.
                if (entry.path() == src / "sorted")
                    continue;

                // Only print directory names (not organizing them).
                std::cout << "DIR:  "
                          << entry.path().filename().string()
                          << "\n";
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
