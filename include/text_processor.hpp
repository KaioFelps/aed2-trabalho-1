#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace core
{

namespace text_processor_internal
{

const auto allowed_connectors = std::vector{'-', '\'', '/'};

const auto diacritics_removal_map =
    std::unordered_map<std::string, std::string>{
        {"à", "a"}, {"á", "a"}, {"â", "a"}, {"ã", "a"}, {"ä", "a"}, {"À", "A"},
        {"Á", "A"}, {"Â", "A"}, {"Ã", "A"}, {"Ä", "A"}, {"è", "e"}, {"é", "e"},
        {"ê", "e"}, {"ë", "e"}, {"È", "E"}, {"É", "E"}, {"Ê", "E"}, {"Ë", "E"},
        {"ì", "i"}, {"í", "i"}, {"î", "i"}, {"Ì", "I"}, {"Í", "I"}, {"Î", "I"},
        {"ò", "o"}, {"ó", "o"}, {"ô", "o"}, {"õ", "o"}, {"Ò", "O"}, {"Ó", "O"},
        {"Ô", "O"}, {"Õ", "O"}, {"ù", "u"}, {"ú", "u"}, {"ü", "u"}, {"û", "u"},
        {"Ù", "U"}, {"Ú", "U"}, {"Ü", "U"}, {"Û", "U"}, {"ç", "c"}, {"Ç", "C"},
    };

} // namespace text_processor_internal

class TextProcessor
{
#ifdef UNIT_TEST
public:
#else
private:
#endif
  /**
   * É um vetor para proporcionar uma indexação mais eficiente,
   * mas deve ser garantido pelo *callee* do construtor que não há
   * elementos repetidos neste vetor para evitar desperdício de recursos.
   */
  std::unordered_set<std::string> stopwords;

  bool is_ascii(uint8_t character) const;
  /**
   * Um caractere utf-8 tem, no primeiro byte que o compõe, um padrão
   * que informa quantos bytes a serem processados pertencem a este caractere.
   *
   * Por exemplo,
   * - 110xxxx -> utf-8
   * - 1110xxxx -> utf-16
   * - 11110xxx -> utf-32
   *
   * Os bytes seguintes têm seus 2 bits mais significantes iguais a `10`. A
   * concatenação dos bits úteis formam o caractere que se quer representar de
   * fato.
   *
   * Para verificar se o presente byte marca o início de um caractere, basta
   * checar esse padrão.
   */
  bool is_utf8_initial_byte(uint8_t byte) const;
  bool is_lower_case_letter(uint8_t character) const;
  bool is_upper_case_letter(uint8_t character) const;
  bool is_alpha(uint8_t character) const;
  bool is_stopword(std::string &buffer) const;
  bool is_connected(std::string &buffer, uint8_t character) const;

  uint8_t to_lower_case(const uint8_t character) const;
  void make_lower_case(std::string &buffer) const;
  void remove_diacritics(std::string &word) const;

public:
  TextProcessor();
  TextProcessor(std::unordered_set<std::string> stopwords);
  /**
   * Transforma o conteúdo de `stream`, extraindo palavras ASCII em
   * minúsculo. Palavras definidas em `stopwords_file` são excluídas.
   */
  std::vector<std::string> process(std::istream &stream) const;
};

} // namespace core
