/**
 * \file
 * \authors Eremenko Ilya
 * \brief Sorts the strings in the text.
 *
 * This program sorts the strings in the text from file text.txt starting
 * from the beginning or ending (optional).
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <gtest/gtest.h>
#include <cassert>
#include <cerrno>

////////////////////////////////////////////////////////////////////////////////

#define asserted || Failed();

bool Failed() {
  std::abort();
  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Contains a pointer to the string in the text and its size.
 */
class Line {
public:
  char* line = nullptr;
  int size = 0;
  Line() = default;
};

/**
 * Returns the index of next letter in given string.
 * @param current {the current string}
 * @param current_index {the current index}
 * @return {the index of next letter in the string}
 */
int GetNextIndex(const Line* current, int current_index);

/**
 * Returns the index of previous letter in given string.
 * @param current {the current string}
 * @param current_index {the current index in the string}
 * @return {the index of previous letter in the string}
 */
int GetPreviousIndex(const Line* current, int current_index);

/**
 * A wrapper to the functions GetNextIndex, GetPreviousIndex.
 * @param current {the current string}
 * @param current_index {the current index in the string}
 * @param option {determines the direction in which the next letter is searched}
 * @return {the index of next or previous letter in the string depending on the
 * option value}
 */
int GetIndex(const Line* current, int current_index, bool option);

/**
 * A universal similar part of CmpPrefix and CmpSuffix comparators. The option
 * determines the way the two strings are compared.
 * @param first_value
 * @param second_value
 * @param option
 * @return
 */
int Cmp(const void* first_value, const void* second_value, bool option);

/**
 * Comparator for usage in std::qsort, comparing starts from the prefixes of
 * given strings.
 * @param first_value
 * @param second_value
 * @return
 */
int CmpPrefix(const void* first_value, const void* second_value);

/**
 * Comparator for usage in std::qsort, comparing starts from the suffixes of
 * given strings
 * @param first_value
 * @param second_value
 * @return
 */
int CmpSuffix(const void* first_value, const void* second_value);

/**
 * \brief Reading the file and creating text manager.
 *
 * This function reads the text from the file, counts the number of lines in it
 * and returns an array containing the pointer to each sentence and the size of
 * this sentence (both in one class named Line).
 * @param lines_number {A number of strings in the text}
 * @param text {A pointer to the text}
 * @return pointer to text manger array
 */
Line* FileReading(int& lines_number, char*& text, long& size_of_text);

/**
 * \brief Writing result in special file.
 * @param lines_number {A number of strings in the text}
 * @param text_manager {A pointer to the text}
 */
void ResultOutput(int& lines_number, Line* text_manager, char*& text,
    long& size_of_text);

////////////////////////////////////////////////////////////////////////////////

int main() {
  int lines_number = 0;
  long size_of_text = 0;
  char* text = nullptr;
  Line* text_manager = FileReading(lines_number, text, size_of_text);
  ResultOutput(lines_number, text_manager, text, size_of_text);
  delete[] text;
  delete[] text_manager;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

Line* FileReading(int& lines_number, char*& text, long& size_of_text) {
  int local_test_value = 0;
  FILE* file = fopen("../text.txt", "r");
  assert(file != nullptr);
  local_test_value = fseek(file, 0, SEEK_END);
  assert(local_test_value == 0);
  size_of_text = ftell(file);
  assert(errno == 0);
  text = new char[size_of_text + 1];
  local_test_value = fseek(file, 0, SEEK_SET);
  assert(local_test_value == 0);
  local_test_value = fread(text, sizeof(char), size_of_text, file);
  assert(local_test_value > 0);
  assert(local_test_value == size_of_text);
  fclose(file) != EOF asserted;
  text[size_of_text] = '\n';
  for (size_t index = 0; index < size_of_text; ++index) {
    if ((text[index] == '\n') && (index != 0) && (text[index - 1] != '\n')) {
      ++lines_number;
    }
  }
  Line* text_manager = new Line[lines_number];
  size_t line_index = 0, current_beginning = 0;
  for (size_t index = 0; index < size_of_text; ++index) {
    if (text[index] == '\n') {
      if ((index - current_beginning) != 0) {
        text_manager[line_index].size = index - current_beginning;
        text_manager[line_index].line = &text[current_beginning];
        ++line_index;
      }
      current_beginning = index + 1;
    }
  }
  return text_manager;
}

int GetNextIndex(const Line* current, int current_index) {
  ++current_index;
  while ((!isalpha(current->line[current_index])) &&
  (current_index < current->size)) {
    ++current_index;
  }
  return current_index;
}

int GetPreviousIndex(const Line* current, int current_index) {
  --current_index;
  while ((!isalpha(current->line[current_index])) && (current_index >= 0)) {
    --current_index;
  }
  return current_index;
}

int GetIndex(const Line* current, int current_index, bool option) {
  if (option) {
    return GetNextIndex(current, current_index);
  } else {
    return GetPreviousIndex(current, current_index);
  }
}

int Cmp(const void* first_value, const void* second_value, bool option) {
  const Line* first = static_cast<const Line*>(first_value);
  const Line* second = static_cast<const Line*>(second_value);
  int current_first = GetIndex(first, (option ? -1 : first->size), option),
      current_second = GetIndex(second, (option ? -1 : second->size), option),
      current_result = 0;

  assert(isalpha(first->line[current_first]));
  assert(isalpha(second->line[current_second]));
  assert(first->line[current_first] >= 0);
  assert(second->line[current_second] >= 0);

  while ((current_first < first->size) && (current_second < second->size)
  && (current_first > -1) && (current_second > -1)) {
    current_result = tolower(first->line[current_first]) -
                     tolower(second->line[current_second]);
    if (current_result != 0) {
      return current_result;
    }
    current_first = GetIndex(first, current_first, option);
    current_second = GetIndex(second, current_second, option);

    assert(isalpha(first->line[current_first]));
    assert(isalpha(second->line[current_second]));
    assert(first->line[current_first] >= 0);
    assert(second->line[current_second] >= 0);

  }

  return current_result;
}


int CmpPrefix(const void* first_value, const void* second_value) {
  return Cmp(first_value, second_value, true);
}

int CmpSuffix(const void* first_value, const void* second_value) {
  return Cmp(first_value, second_value, false);
}

void ResultOutput(int& lines_number, Line* text_manager, char*& text,
    long& size_of_text) {

  int separating_line_size = 82;
  char* separating_line = new char[separating_line_size];

  for (int index = 0; index < separating_line_size - 2; ++index) {
    separating_line[index] = '/';
  }
  separating_line[separating_line_size - 1] = '\n';
  separating_line[separating_line_size - 2] = '\n';

  FILE* output_file = fopen("../output.txt", "w");
  assert(output_file != nullptr);

  fwrite(text, sizeof(char), size_of_text + 1, output_file);

  fwrite(separating_line, sizeof(char),
         separating_line_size , output_file);

  std::qsort(text_manager, lines_number, sizeof(Line), CmpPrefix);

  for (size_t index = 0; index < lines_number; ++index) {
    fwrite(text_manager[index].line, sizeof(char),
        text_manager[index].size + 1, output_file)
        == text_manager[index].size + 1 asserted;
  }

  fwrite(separating_line, sizeof(char),
         separating_line_size , output_file);

  std::qsort(text_manager, lines_number, sizeof(Line), CmpSuffix);

  for (size_t index = 0; index < lines_number; ++index) {
    fwrite(text_manager[index].line, sizeof(char),
        text_manager[index].size + 1, output_file)
        == text_manager[index].size + 1 asserted;
  }

  fclose(output_file) != EOF asserted;
}
