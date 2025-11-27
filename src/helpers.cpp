#include "helpers.hpp"
#include <algorithm>
#include <fstream>

namespace core::helpers
{

std::unordered_set<std::string>
parse_stopwords(std::filesystem::path stopwords_file)
{
  auto stopwords = std::unordered_set<std::string>();
  auto stream = std::ifstream(stopwords_file);

  if (!stream.is_open())
  {
    throw std::runtime_error(
        "Não foi possível abrir o arquivo de stopwords em " +
        stopwords_file.string() + ".");
  };

  std::string buffer;
  while (std::getline(stream, buffer))
  {

    const auto not_empty =
        std::find_if(buffer.begin(), buffer.end(), [](uint8_t character)
                     { return !std::isspace(character); }) != buffer.end();

    if (not_empty) stopwords.insert(buffer);
  }

  return stopwords;
}

} // namespace core::helpers
