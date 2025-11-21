#include "indexer.hpp"
#include <algorithm>
#include <cassert>
#include <span>

namespace core
{

Index::Index(std::vector<File> files, words_map_t words)
    : files(files), words(words)
{
}

Index::Index() : files(std::vector<File>()), words(words_map_t()) {};

std::reference_wrapper<File> Index::get_file_by_id(const uint64_t &id)
{
  assert(id != 0 && "A File ID must never ever ever be 0");
  assert(id <= this->files.size() &&
         "A File ID must never be greater than the size of files array, and "
         "this indicates a program logical error");
  const auto index = id - 1;
  return std::ref(this->files[index]);
}

void Index::add_word_from_file(std::string word,
                               std::filesystem::path file_path) noexcept
{
  auto file_found = std::find_if(this->files.begin(), this->files.end(),
                                 [&file_path](const File &file)
                                 { return file.get_path() == file_path; });

  uint64_t id;

  if (file_found != this->files.end())
  {
    id = std::distance(this->files.begin(), file_found) + 1;
  }
  else
  {
    this->files.push_back(File(file_path.string()));
    id = this->files.size();
  }

  auto &files_containing_word = this->words[word];
  files_containing_word.insert(id);
}

files_refs_set_t
Index::get_files_containing_words(std::vector<std::string> words) noexcept
{
  auto files_set = files_refs_set_t();
  if (words.empty()) return files_set;

  const auto &first_word = words.front();
  if (words.size() == 1)
  {
    for (auto &id : this->words[first_word])
    {
      files_set.insert(this->get_file_by_id(id));
    }

    return files_set;
  }

  const auto remaining_words = std::span(std::next(words.begin()), words.end());
  for (auto &id : this->words[first_word])
  {
    auto belongs_to_result_set = true;
    for (auto &word : remaining_words)
    {
      if (this->words[word].contains(id)) continue;
      belongs_to_result_set = false;
      break;
    }

    if (belongs_to_result_set)
    {
      files_set.insert(this->get_file_by_id(id));
    }
  }

  return files_set;
}

} // namespace core
