#pragma once

#include "file.hpp"
#include <filesystem>
#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define words_map_t                                                            \
  std::unordered_map<std::string, std::unordered_set<uint64_t>>

#define files_refs_set_t                                                       \
  std::set<std::reference_wrapper<const File>, File::RefComparer>

namespace core
{

class Index
{
  friend class IndexSerializer;

private:
  std::vector<File> files;
  words_map_t words;

  /**
   * Retorna uma referência envelopada para garantir que o *calee* não copie
   * o arquivo na hora de retornar o conjunto de referências acidentalmente.
   *
   * Isso impede que esse bug volte a acontecer (referências nulas são iguais
   * num conjunto, então diferentes arquivos nunca são inseridos pois são
   * igualmente nulos).
   */
  std::reference_wrapper<File> get_file_by_id(const uint64_t &id);

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
  files_refs_set_t
  get_files_containing_words(std::vector<std::string> word) noexcept;
};

class Indexer
{
#ifdef UNIT_TEST
public:
#else
private:
#endif
  std::filesystem::path index_data_dir;
  std::optional<Index> index;

public:
  Indexer(std::filesystem::path index_data_dir);
  /**
   * Obtém a instância corrente do indexador.
   */
  Index &get_index();
  /**
   * Carrega os dados de arquivos na memória.
   */
  void try_load();
  /**
   * Persiste os dados do indexador no disco rígido.
   */
  void flush() const;
  /**
   * Verifica se o índice pôde ser carregado e reconstruído em memória.
   */
  bool has_loaded_index() const noexcept;
};

} // namespace core
