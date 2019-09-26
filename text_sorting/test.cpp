#include <gtest/gtest.h>


class TestText : public ::testing::Test {
protected:
  void SetUp() {
    FILE *file = fopen("../test.txt", "w");
    ASSERT_NE(file, nullptr);
  }
};

TEST(, file_reading) {
  FILE* test_file = fopen("../test.txt", "r");
  fseek(file, 0, SEEK_END);
  long size_of_text = ftell(file);
  text = new char[size_of_text];
  fseek(file, 0, SEEK_SET);
  fread(text, sizeof(char), size_of_text, file);

}



