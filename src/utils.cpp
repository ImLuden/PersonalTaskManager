#include "task_manager.h"

// Helper function to trim and convert a string to lowercase
std::string toLowerAndTrim(const std::string& str)
{
    // Find the first and last non-whitespace characters
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");

    // If the string is entirely whitespace, return an empty string
    if (start == std::string::npos)
    {
        return "";
    }

    // Create a trimmed substring
    std::string trimmed = str.substr(start, end - start + 1);

    // Convert to lowercase
    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);

    return trimmed;
}