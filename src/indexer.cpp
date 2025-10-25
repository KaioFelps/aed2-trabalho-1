#include "indexer.hpp"
#include "index_serializer.hpp"

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

Indexer::Indexer(std::filesystem::path index_data_file)
    : index_data_file(index_data_file),
      index(
          IndexSerializer(index_data_file, WORDS_DATA_FILE, FILES_MAP_DATA_FILE)
              .deserialize()
              .value_or(Index()))
{
}

} // namespace core
