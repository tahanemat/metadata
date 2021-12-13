#include "directory.h"
#include <fstream>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>

using Entries = std::vector<std::unordered_map<std::string, std::string>>;

class FilesystemTest : public ::testing::Test {
protected:
  FilesystemTest() {}
  void SetUp() override {
    std::filesystem::remove_all(
        "./" + test_dir_name); // incase of previous failed tests
    std::filesystem::create_directory("./" + test_dir_name + "/");
  }

  void TearDown() override {
    std::filesystem::remove_all("./" + test_dir_name + "/");
  }
  ~FilesystemTest() {}

  Entries entries;
  std::string test_dir_name = "testdir";
};

std::vector<std::string> split(std::string s, std::string delimiter) {
  std::size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

Entries parseCout(std::string input) {
  Entries entries(0);
  const auto nodes = split(input, "\n\n");
  for (const auto node : nodes) {
    if (!node.empty()) {
      std::vector<std::string> lines;
      testing::internal::SplitString(node, '\n', &lines);
      entries.emplace_back();
      for (const auto line : lines) {
        const auto keyval = split(line, ": ");
        auto key = keyval.front();
        key.erase(remove_if(key.begin(), key.end(), isspace), key.end());
        entries.back().insert(std::make_pair(key, keyval.back()));
      }
    }
  }
  return entries;
}

void createFile(std::filesystem::path p, std::string text_to_insert = "abc\n") {
  std::ofstream ofs(p);
  ofs << text_to_insert;
  ofs.close();
}

TEST_F(FilesystemTest, Test_Directory_Print) {
  ::testing::internal::CaptureStdout();

  std::filesystem::directory_entry e{"./" + test_dir_name + "/"};
  print_directory(e);

  std::string output = testing::internal::GetCapturedStdout();

  const auto nodes = parseCout(output);

  EXPECT_EQ(nodes.size(), 1);

  const auto &entry = nodes.back();

  EXPECT_EQ(entry.at("Path"), "/");
  EXPECT_EQ(entry.at("Type"), "Directory");
  EXPECT_EQ(entry.at("Files"), "0");
  EXPECT_EQ(entry.at("Directories"), "0");
  EXPECT_EQ(entry.at("Bytes"), "0");
}

TEST_F(FilesystemTest, Test_File_Print) {
  ::testing::internal::CaptureStdout();

  std::filesystem::directory_entry e{"./" + test_dir_name + "/"};
  std::filesystem::path path{"./" + test_dir_name + "/testfile.txt"};

  std::ofstream ofs(path);
  ofs << "this is some text in the new file\n";
  ofs.close();

  std::filesystem::directory_iterator itr{e};
  for (auto i : itr) {
    print_file(i);
  }

  std::string output = testing::internal::GetCapturedStdout();

  const auto nodes = parseCout(output);

  EXPECT_EQ(nodes.size(), 1);

  const auto &entry = nodes.back();

  EXPECT_EQ(entry.at("Path"), "testfile.txt");
  EXPECT_EQ(entry.at("Type"), "File");
  EXPECT_GT(entry.at("Bytes"), "0");
}

TEST_F(FilesystemTest, Test_Symlink_Ignore) {
  ::testing::internal::CaptureStdout();

  std::filesystem::directory_entry e{"./" + test_dir_name + "/"};

  std::filesystem::create_symlink("./" + test_dir_name + "/",
                                  "./" + test_dir_name + "/symlink");

  print_directory(e);

  std::string output = testing::internal::GetCapturedStdout();

  const auto nodes = parseCout(output);

  EXPECT_EQ(nodes.size(), 1);

  const auto &entry = nodes.back();

  EXPECT_EQ(entry.at("Path"), "/");
  EXPECT_EQ(entry.at("Type"), "Directory");
  EXPECT_EQ(entry.at("Files"), "0");
  EXPECT_EQ(entry.at("Directories"), "0");
  EXPECT_EQ(entry.at("Bytes"), "0");
}

TEST_F(FilesystemTest, Test_SubDirectories_With_Files) {

  ::testing::internal::CaptureStdout();

  // GENERATE FILES AND SUBDIRECTORIES====================================
  std::filesystem::directory_entry e{"./" + test_dir_name + "/"};

  createFile("./" + test_dir_name + "/testfile1.txt");
  createFile("./" + test_dir_name + "/testfile2.txt");
  std::filesystem::create_directories("./" + test_dir_name + "/subdir1/");
  std::filesystem::create_directories("./" + test_dir_name + "/subdir2/");
  createFile("./" + test_dir_name + "/subdir1/testfile3.txt");
  createFile("./" + test_dir_name + "/subdir2/testfile4.txt");
  //======================================================================

  print_directory(e);
  display_all_files_in_entry(e);

  const std::string output = testing::internal::GetCapturedStdout();

  const auto nodes = parseCout(output);

  EXPECT_EQ(nodes.size(), 7);

  for (const auto node : nodes) {
    const auto node_path = node.at("Path");

    if (node_path == "/") {
      EXPECT_EQ(node.at("Type"), "Directory");
      EXPECT_EQ(node.at("Files"), "2");
      EXPECT_EQ(node.at("Directories"), "2");
      EXPECT_GT(node.at("Bytes"), "0");
    } else if (node_path == "testfile1.txt" || node_path == "testfile2.txt" ||
               node_path == "testfile3.txt" || node_path == "testfile4.txt") {
      EXPECT_EQ(node.at("Type"), "File");
      EXPECT_GT(node.at("Bytes"), "0");
    } else if (node_path == "subdir1/" || node_path == "subdir2/") {
      EXPECT_EQ(node.at("Type"), "Directory");
      EXPECT_EQ(node.at("Files"), "1");
      EXPECT_EQ(node.at("Directories"), "0");
      EXPECT_GT(node.at("Bytes"), "0");
    }
  }
}
