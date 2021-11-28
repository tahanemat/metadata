#include <filesystem>
#include <string>
#include <vector>

std::string tm_to_str(std::filesystem::file_time_type const &ftime);

std::size_t get_directory_size(const std::filesystem::directory_entry &entry);

std::pair<std::size_t, std::size_t> number_of_files_and_directories_in_path(
    const std::filesystem::directory_entry &entry);

void print_directory(const std::filesystem::directory_entry &entry,
                     std::string path_prefix = "", std::size_t indent = 0);

void print_file(const std::filesystem::directory_entry &entry,
                std::string path_prefix = "", std::size_t indent = 0);

void display_all_files_in_entry(const std::filesystem::directory_entry &dir,
                                std::string path_prefix = "",
                                std::size_t indent = 1);
