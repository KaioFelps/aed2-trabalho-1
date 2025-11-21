#include "indexer.hpp"
#include "index_serializer.hpp"
#include <cassert>

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

Indexer::Indexer(std::filesystem::path index_data_dir)
    : index_data_dir(index_data_dir)
{
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

} // namespace core
