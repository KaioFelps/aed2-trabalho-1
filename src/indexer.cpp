#include "indexer.hpp"
#include "index_serializer.hpp"
#include <cassert>
#include <fstream>

namespace fs = std::filesystem;

namespace core
{

/**
 * Arquivo binário contendo o mapeamento palavras - arquivo.
 */
const fs::path WORDS_DATA_FILE = "index.dat";
/**
 * Arquivo binário contendo o mapeamento id - arquivo.
 */
const fs::path FILES_MAP_DATA_FILE = "files_map.dat";

Indexer::Indexer(fs::path index_data_dir, TextProcessor processor)
    : index_data_dir(index_data_dir), processor(processor)
{
}

files_refs_set_t Indexer::get_files_with_words(std::vector<std::string> words)
{
  auto normalized_words = std::vector<std::string>();
  for (auto &word : words)
  {
    normalized_words.push_back(TextProcessor::normalize_word(word));
  }

  return this->get_index().get_files_containing_words(normalized_words);
}

Index &Indexer::get_index() noexcept(false)
{
  if (!this->index.has_value())
  {
    throw std::runtime_error(
        "O índice não foi carregado. Tente carregá-lo com o método `try_load` "
        "e garanta que ele já tenha sido construído anteriormente.");
  }

  return this->index.value();
}

void Indexer::try_load() noexcept(false)
{
  auto deserializer = IndexSerializer(this->index_data_dir, WORDS_DATA_FILE,
                                      FILES_MAP_DATA_FILE);

  auto index = deserializer.deserialize();

  if (!index.has_value())
  {
    throw std::runtime_error("O índice não pode ser carregado. Garanta que ele "
                             "já tenha sido construído anteriormente.");
  }

  this->index = index.value();
}

void Indexer::flush() const noexcept(false)
{
  assert(this->index.has_value() && "Cannot flush a nullopt index. Ensure it "
                                    "exists before trying to flush it.");

  auto serializer = IndexSerializer(this->index_data_dir, WORDS_DATA_FILE,
                                    FILES_MAP_DATA_FILE);

  serializer.serialize(this->index.value());
}

bool Indexer::has_loaded_index() const noexcept(true)
{
  return this->index.has_value();
}

void Indexer::build(fs::path lookup_dir)
{
  this->index = Index();
  this->parse_entry(lookup_dir);
}

void Indexer::parse_entry(fs::path path)
{
  assert(this->index.has_value() && "Tried to call `Indexer::parse_entry` but "
                                    "there is no `Index` instance stored.");

  if (fs::is_regular_file(path))
  {
    auto stream = std::ifstream(path);
    auto words = this->processor.process(stream);

    for (auto &word : words)
    {
      this->index->add_word_from_file(std::move(word), path);
    }

    return;
  }
  if (fs::is_directory(path))
  {
    for (auto entry : fs::directory_iterator(path))
    {
      parse_entry(entry.path());
    }

    return;
  }

  throw std::runtime_error("Não foi possível varrer o diretório/arquivo " +
                           path.string() + ".");
}

} // namespace core
