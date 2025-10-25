#include "indexer.hpp"

namespace core
{

Index::Index(std::set<File> files,
             std::unordered_map<std::string, std::set<File>> words)
    : files(files), words(words)
{
}

Index::Index()
    : files(std::set<File>()),
      words(std::unordered_map<std::string, std::set<File>>()) {};

} // namespace core
