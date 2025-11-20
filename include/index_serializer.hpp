#pragma once

#include "indexer.hpp"
#include <ios>
#include <optional>

namespace core
{

class IndexSerializer
{
private:
  std::filesystem::path data_dir;
  std::filesystem::path words_file;
  std::filesystem::path index_file;

  std::unordered_map<std::string, std::set<File>>
  map_words_map_files_ids_to_files(
      const std::set<File> &files,
      const std::unordered_map<std::string, std::set<std::string>> &words_map);

  static void
  ensure_data_has_not_reached_end(const std::istream &rstream,
                                  const std::filesystem::path &file);

  void remove_data_files() noexcept;

  static std::unordered_map<std::string, std::set<std::string>>
  deserialize_words_map(std::istream &rstream,
                        const std::filesystem::path &data_file_path);

  static std::set<File>
  deserialize_files_set(std::istream &rstream,
                        const std::filesystem::path &data_file_path);

public:
  IndexSerializer(std::filesystem::path data_dir,
                  std::filesystem::path words_file,
                  std::filesystem::path index_file);

  void serialize(Index &index);
  std::optional<Index> deserialize();
};

} // namespace core
