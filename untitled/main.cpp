// This program sorts the strings in the text from file text.txt starting
// from the beginning or ending (optional)
#include <iostream>
#include <cstdio>
#include <cstdlib>

using std::endl;
using std::string;

class Line { // class contains a pointer to the string in the text
  // and its size
public:
  char* line = nullptr;
  size_t size = 0;
  Line() = default;
};

int GetNextIndex(const Line* current, int current_index); // returns the index
  // of next letter in given line

int GetPreviousIndex(const Line* current, int current_index); // returns the
  // index of previous letter in given line

int cmp_prefix(const void* first_value, const void* second_value); // comparing
  // starts from the prefixes of given strings

int cmp_suffix(const void* first_value, const void* second_value); // comparing
  // starts from the suffixes of given strings

void ResultOutput(int& lines_number, Line* text_manager);

Line* FileReading(int& lines_number);

int main() {

  int lines_number = 0;
  FILE* file = fopen("../text.txt", "r");
  fseek(file, 0, SEEK_END);
  long size_of_text = ftell(file);
  char* text = new char[size_of_text + 1];
  fseek(file, 0, SEEK_SET);
  fread(text, sizeof(char), size_of_text, file);
  fclose(file);
  text[size_of_text] = '\n';
  for (size_t index = 0; index < size_of_text; ++index) {
    if (text[index] == '\n') {
      ++lines_number;
    }
  }
  Line* text_manager = new Line[lines_number]; // an array containing the
  // pointer to each sentence and the size of this sentence (both in one
  // class named Line)
  size_t line_index = 0, current_beginning = 0;
  for (size_t index = 0; index < size_of_text; ++index) {
    if (text[index] == '\n') {
      text_manager[line_index].size = index - current_beginning;
      text_manager[line_index].line = &text[current_beginning];
      ++line_index;
      current_beginning = index + 1;
    }
  }

  ResultOutput(lines_number, text_manager);
  delete[] text;
  delete[] text_manager;
}

void ResultOutput(int& lines_number, Line* text_manager) {
  FILE* output_file = fopen("../output.txt", "w");

  for (size_t index = 0; index < lines_number; ++index) {
    fwrite(text_manager[index].line, sizeof(char),text_manager[index].size + 1, output_file);
  }

  std::qsort(text_manager, lines_number, sizeof(Line), cmp_prefix);
  // sorting from the beginning of string

  for (size_t index = 0; index < lines_number; ++index) {
    fwrite(text_manager[index].line, sizeof(char),text_manager[index].size + 1, output_file);
  }

  std::qsort(text_manager, lines_number, sizeof(Line), cmp_suffix);
  // sorting from the ending of string

  for (size_t index = 0; index < lines_number; ++index) {
    fwrite(text_manager[index].line, sizeof(char),text_manager[index].size + 1, output_file);
  }

  fclose(output_file);
}

Line* FileReading(int& lines_number) {
  FILE* file = fopen("../text.txt", "r");
  fseek(file, 0, SEEK_END);
  long size_of_text = ftell(file);
  char* text = new char[size_of_text + 1];
  fseek(file, 0, SEEK_SET);
  fread(text, sizeof(char), size_of_text, file);
  fclose(file);
  text[size_of_text] = '\n';
  for (size_t index = 0; index < size_of_text; ++index) {
    if (text[index] == '\n') {
      ++lines_number;
    }
  }
  Line* text_manager = new Line[lines_number]; // an array containing the
  // pointer to each sentence and the size of this sentence (both in one
  // class named Line)
  size_t line_index = 0, current_beginning = 0;
  for (size_t index = 0; index < size_of_text; ++index) {
    if (text[index] == '\n') {
      text_manager[line_index].size = index - current_beginning;
      text_manager[line_index].line = &text[current_beginning];
      ++line_index;
      current_beginning = index + 1;
    }
  }
  delete[] text;
  return text_manager;
}

int GetNextIndex(const Line* current, int current_index) { // returns the index
  // of next letter in given line
  ++current_index;
  while ((!isalpha(current->line[current_index])) && (current_index < current->size)) {
    ++current_index;
  }
  return current_index;
}

int GetPreviousIndex(const Line* current, int current_index) { // returns the
  // index of previous letter in given line
  --current_index;
  while ((!isalpha(current->line[current_index])) && (current_index >= 0)) {
    --current_index;
  }
  return current_index;
}

int cmp_prefix(const void* first_value, const void* second_value) { // comparing
  // starts from the prefixes of given strings
  const Line* first = static_cast<const Line*>(first_value);
  const Line* second = static_cast<const Line*>(second_value);
  int current_first = GetNextIndex(first, -1),
      current_second = GetNextIndex(second, -1);
  while ((current_first < first->size) && (current_second < second->size)) {
    if (tolower(first->line[current_first]) <
        tolower(second->line[current_second])) {
      return -1; // first argument is less
    }
    if (tolower(first->line[current_first]) >
        tolower(second->line[current_second])) {
      return 1; // second argument is less
    }
    current_first = GetNextIndex(first, current_first);
    current_second = GetNextIndex(second, current_second);
  }
  // if one of two lines is ended we will choose the shorter one as the
  // least argument, if both are ended we will consider them equal
  if (current_first == first->size) {
    if (current_second == second->size) {
      return 0;
    } else {
      return -1;
    }
  } else {
    return 1;
  }
}

int cmp_suffix(const void* first_value, const void* second_value) { // comparing
  // starts from the suffixes of given strings
  const Line* first = static_cast<const Line*>(first_value);
  const Line* second = static_cast<const Line*>(second_value);
  int current_first = GetPreviousIndex(first, first->size),
      current_second = GetPreviousIndex(second, second->size);
  while ((current_first >= 0) && (current_second >= 0)) {
    if (tolower(first->line[current_first]) <
        tolower(second->line[current_second])) {
      return -1; // first argument is less
    }
    if (tolower(first->line[current_first]) >
        tolower(second->line[current_second])) {
      return 1; // second argument is less
    }
    current_first = GetPreviousIndex(first, current_first);
    current_second = GetPreviousIndex(second, current_second);
  }
  // if one of two lines is ended we will choose the shorter one as the
  // least argument, if both are ended we will consider them equal
  if (current_first == -1) {
    if (current_second == -1) {
      return 0;
    } else {
      return -1;
    }
  } else {
    return 1;
  }
}
