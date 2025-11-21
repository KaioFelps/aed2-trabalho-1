#pragma once

#include "indexer.hpp"
#include <ios>
#include <optional>

namespace core
{

class IndexSerializer
{
#ifdef UNIT_TEST
public:
#else
private:
#endif
  std::filesystem::path data_dir;
  std::filesystem::path words_file;
  std::filesystem::path index_file;

  static void
  ensure_data_has_not_reached_end(const std::istream &rstream,
                                  const std::filesystem::path &file);

  static void ensure_write_is_successful(const std::ostream &stream);

  void remove_data_files() noexcept;

  static words_map_t
  deserialize_words_map(std::istream &rstream,
                        const std::filesystem::path &data_file_path);

  static std::vector<File>
  deserialize_files_set(std::istream &rstream,
                        const std::filesystem::path &data_file_path);

  static void serialize_words_map(const words_map_t &words_map,
                                  std::ostream &wstream);

  static void serialize_files_set(const std::vector<File> &files,
                                  std::ostream &wstream);

public:
  IndexSerializer(std::filesystem::path data_dir,
                  std::filesystem::path words_file,
                  std::filesystem::path index_file);

  void serialize(const Index &index);
  std::optional<Index> deserialize();
};

} // namespace core
