#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <sstream>

std::string tm_to_str(std::filesystem::file_time_type const &ftime) {
  std::time_t cftime = std::chrono::system_clock::to_time_t(
      std::chrono::file_clock::to_sys(ftime));
  const auto lt = std::localtime(&cftime);
  std::stringstream ss;
  ss << std::put_time(lt, "%Y-%m-%d %T");
  return ss.str();
}

std::size_t get_directory_size(const std::filesystem::directory_entry &entry) {
  std::filesystem::directory_iterator itr{entry};
  std::size_t answer = 0;
  std::for_each(itr, {}, [&](const std::filesystem::directory_entry &item) {
    if (item.is_regular_file()) { // calculate size of regular files only
      answer += std::filesystem::file_size(item);
    }
  });
  return answer;
}

std::pair<std::size_t, std::size_t> number_of_files_and_directories_in_path(
    const std::filesystem::directory_entry &entry) {
  std::filesystem::directory_iterator itr{entry};
  std::pair<std::size_t, std::size_t> result = std::make_pair(0, 0);
  std::for_each(itr, {},
                [&result](const std::filesystem::directory_entry &entry) {
                  if (entry.is_symlink()) {
                    return;
                  } else if (entry.is_directory()) {
                    result.second += 1;
                  } else if (entry.is_regular_file()) {
                    result.first += 1;
                  }
                });
  return result;
}

void print_directory(const std::filesystem::directory_entry &entry,
                     std::string path_prefix = "", std::size_t indent = 0) {
  const auto &[no_of_files, no_of_directories] =
      number_of_files_and_directories_in_path(entry);
  std::stringstream ss;
  for (std::size_t i = 0; i < indent; i++) {
    ss << "\t";
  }
  auto indentation = ss.str();
  std::cout << indentation << "Path: " << path_prefix
            << entry.path().filename().c_str() << "/\n"
            << indentation << "Type: Directory\n"
            << indentation << "Files: " << no_of_files << '\n'
            << indentation << "Directories: " << no_of_directories << '\n'
            << indentation << "Bytes: " << get_directory_size(entry) << '\n'
            << indentation
            << "Last Modified At: " << tm_to_str(entry.last_write_time())
            << indentation << "\n\n";
}

void print_file(const std::filesystem::directory_entry &entry,
                std::string path_prefix = "", std::size_t indent = 0) {

  std::stringstream ss;
  for (std::size_t i = 0; i < indent; i++) {
    ss << "\t";
  }
  auto indentation = ss.str();
  std::cout << indentation << "Path: " << path_prefix
            << entry.path().filename().c_str() << '\n'
            << indentation << "Type: File\n"
            << indentation << "Bytes: " << entry.file_size() << '\n'
            << indentation
            << "Last Modified At: " << tm_to_str(entry.last_write_time())
            << indentation << "\n\n";
}

void display_all_files_in_entry(const std::filesystem::directory_entry &dir,
                                std::string path_prefix = "",
                                std::size_t indent = 1) {

  std::filesystem::directory_iterator itr{dir};

  std::for_each(itr, {}, [&](const std::filesystem::directory_entry &entry) {
    if (std::filesystem::is_symlink(entry.path())) {
      return;
    } else if (std::filesystem::is_directory(entry.path())) {
      // traverse the directory and print only regular files
      // depth level = 1

      if (indent == 1) {
        print_directory(entry, path_prefix, indent);
        display_all_files_in_entry(
            entry, std::string(entry.path().filename().c_str()) + '/',
            indent + 1);
      }
      return;
    } else if (!std::filesystem::is_regular_file(entry.path())) {
      return;
    }
    print_file(entry, path_prefix, indent);
  });
}
