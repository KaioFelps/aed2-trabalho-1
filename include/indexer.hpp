#pragma once

#include "file.hpp"
#include <filesystem>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define words_map_t                                                            \
  std::unordered_map<std::string, std::unordered_set<uint64_t>>

namespace core
{

class Index
{
  friend class IndexSerializer;

private:
  std::vector<File> files;
  words_map_t words;

  /**
   * Associa a palavra ao arquivo através do seu ID interno.
   */
  void add_word_from_file_by_file_id(std::string word, std::string id) noexcept;

public:
  Index();
  Index(std::vector<File> files, words_map_t words);
  /**
   * Registra a ocorrência da palavra `word` no arquivo localizado em
   * `file_path`.
   */
  void add_word_from_file(std::string word,
                          std::filesystem::path file) noexcept;
  /**
   * Obtém um conjunto de arquivos contendo a palavra `word`.
   */
  std::vector<const File> get_files_containing_word(std::string word) noexcept;
};

class Indexer
{
private:
  std::filesystem::path index_data_file;
  Index index;

public:
  Indexer(std::filesystem::path index_data_file);
  /**
   * Obtém a instância corrente do indexador.
   */
  Indexer &get_index() noexcept;
  /**
   * Carrega os dados de arquivos na memória.
   */
  void try_load() noexcept(false);
  /**
   * Persiste os dados do indexador no disco rígido.
   */
  void flush() const noexcept(false);
};

} // namespace core
