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
  auto output = std::vector<std::string>();
  auto buffer = std::string();
  auto non_ascii_buffer = std::string();
  auto is_reading_non_ascii = false;

  const auto flush_non_ascii_buffer = [&non_ascii_buffer, &buffer, this]()
  {
    this->remove_diacritics(non_ascii_buffer);
    this->make_lower_case(non_ascii_buffer);
    buffer.append(non_ascii_buffer);
    non_ascii_buffer.clear();
  };

  std::string byte;
  byte.reserve(1);
  while (stream.get(reinterpret_cast<char &>(byte)))
  {
    uint8_t character = byte[0];

    if (!this->is_ascii(character))
    {
      is_reading_non_ascii = true;
      non_ascii_buffer.push_back(character);
      continue;
    }

    if (is_reading_non_ascii)
    {
      is_reading_non_ascii = false;
      flush_non_ascii_buffer();
    }

    if (this->is_alpha(character) || this->is_connected(buffer, character))
    {
      character = this->to_lower_case(character);
      buffer.push_back(character);
      continue;
    }

    if (!buffer.empty())
    {
      if (!this->is_stopword(buffer)) output.push_back(buffer);
      buffer.clear();
    }
  }

  if (!buffer.empty() && !this->is_stopword(buffer)) output.push_back(buffer);

  return output;
}

bool TextProcessor::is_ascii(uint8_t character) const
{
  return character <= 127;
}

bool TextProcessor::is_utf8_initial_byte(uint8_t byte) const
{
  return (byte & 0b11000000) == 0b11000000;
}

bool TextProcessor::is_lower_case_letter(uint8_t character) const
{
  return character >= 97 && character <= 122;
}

bool TextProcessor::is_upper_case_letter(uint8_t character) const
{
  return character >= 65 && character <= 90;
}

bool TextProcessor::is_alpha(uint8_t character) const
{
  return this->is_upper_case_letter(character) ||
         this->is_lower_case_letter(character);
}

bool TextProcessor::is_stopword(std::string &buffer) const
{
  return this->stopwords.contains(buffer);
}

bool TextProcessor::is_connected(std::string &buffer, uint8_t character) const
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

void TextProcessor::make_lower_case(std::string &buffer) const
{
  for (size_t i = 0; i < buffer.size(); i++)
  {
    buffer[i] = this->to_lower_case(buffer[i]);
  }
}

uint8_t TextProcessor::to_lower_case(const uint8_t character) const
{
  if (this->is_upper_case_letter(character))
  {
    const uint8_t difference_between_casing = 32;
    return character + difference_between_casing;
  }

  return character;
}

void TextProcessor::remove_diacritics(std::string &word) const
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

} // namespace core
