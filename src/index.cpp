#include "indexer.hpp"

namespace core
{

Index::Index(std::vector<File> files, words_map_t words)
    : files(files), words(words)
{
}

Index::Index() : files(std::vector<File>()), words(words_map_t()) {};

} // namespace core
