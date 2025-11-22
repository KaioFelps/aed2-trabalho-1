#include "text_processor.hpp"

#include <algorithm>

namespace core
{

TextProcessor::TextProcessor() : stopwords(std::unordered_set<std::string>()) {}
TextProcessor::TextProcessor(std::unordered_set<std::string> stopwords)
    : stopwords(stopwords)
{
}

std::vector<std::string> TextProcessor::process(std::istream &stream) const
{
  using namespace text_processor_internal;

  auto output = std::vector<std::string>();
  auto buffer = std::string();
  auto non_ascii_buffer = std::string();
  auto is_reading_non_ascii = false;

  std::string byte;
  byte.reserve(1);
  while (stream.get(reinterpret_cast<char &>(byte)))
  {
    uint8_t character = byte[0];

    if (!is_ascii(character))
    {
      is_reading_non_ascii = true;
      non_ascii_buffer.push_back(character);
      continue;
    }

    if (is_reading_non_ascii)
    {
      is_reading_non_ascii = false;
      remove_diacritics(non_ascii_buffer);
      make_lower_case(non_ascii_buffer);
      buffer.append(non_ascii_buffer);
      non_ascii_buffer.clear();
    }

    if (is_alphanumeric(character) || is_connected(buffer, character))
    {
      character = to_lower_case(character);
      buffer.push_back(character);
      continue;
    }

    if (!buffer.empty())
    {
      if (!is_stopword(buffer)) output.push_back(buffer);
      buffer.clear();
    }
  }

  if (!buffer.empty() && !is_stopword(buffer)) output.push_back(buffer);

  return output;
}

bool TextProcessor::is_stopword(std::string &buffer) const
{
  return this->stopwords.contains(buffer);
}

namespace text_processor_internal
{

bool is_ascii(uint8_t character) { return character <= 127; }

bool is_lower_case_letter(uint8_t character)
{
  return character >= 97 && character <= 122;
}

bool is_upper_case_letter(uint8_t character)
{
  return character >= 65 && character <= 90;
}

bool is_alphanumeric(uint8_t character)
{
  const auto is_number_char = character >= 48 && character <= 57;
  return is_upper_case_letter(character) || is_lower_case_letter(character) ||
         is_number_char;
}

bool is_connected(std::string &buffer, uint8_t character)
{
  auto is_connector = false;
  for (auto &connector : text_processor_internal::allowed_connectors)
  {
    if (connector == character)
    {
      is_connector = true;
      break;
    }
  }

  return is_connector && !buffer.empty();
}

void make_lower_case(std::string &buffer)
{
  for (size_t i = 0; i < buffer.size(); i++)
  {
    buffer[i] = to_lower_case(buffer[i]);
  }
}

uint8_t to_lower_case(const uint8_t character)
{
  if (is_upper_case_letter(character))
  {
    const uint8_t difference_between_casing = 32;
    return character + difference_between_casing;
  }

  return character;
}

void remove_diacritics(std::string &word)
{
  for (auto &diacritic : text_processor_internal::diacritics_removal_map)
  {
    auto index = word.find(diacritic.first);
    while (index != word.npos)
    {
      word.replace(index, diacritic.first.size(), diacritic.second);
      index = word.find(diacritic.first, index + diacritic.second.size());
    }
  }
}

} // namespace text_processor_internal

} // namespace core
