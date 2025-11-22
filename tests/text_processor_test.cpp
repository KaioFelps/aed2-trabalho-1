#include "unit_test.hpp"

#include "text_processor.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("Espaços em branco devem ser stopwords implícitos",
          "[internal, process, stopwords]")
{
  const auto processor = core::TextProcessor();

  const auto input = "o rato roeu a roupa do rei de roma";
  auto input_stream = std::istringstream(input);

  auto words = processor.process(input_stream);
  const auto expected_words = {"o",  "rato", "roeu", "a",   "roupa",
                               "do", "rei",  "de",   "roma"};

  REQUIRE(words.size() == 9);
  REQUIRE(std::ranges::equal(words, expected_words));
}

TEST_CASE("Deveria remover todas as stopwords explícitas",
          "[internal, process, stopwords]")
{
  const auto stopwords = std::unordered_set<std::string>{
      "o", "a", "e", "do", "da", "de",
  };
  const auto processor = core::TextProcessor(stopwords);

  const auto input = "o rato, roeu a roupa do rei de roma.";
  auto input_stream = std::istringstream(input);

  auto words = processor.process(input_stream);
  const auto expected_words = {"rato", "roeu", "roupa", "rei", "roma"};

  REQUIRE(words.size() == 5);
  REQUIRE(std::ranges::is_permutation(words, expected_words));
}

TEST_CASE("Deveria transformar maiúsculas em minúsculas e não alterar outros "
          "caracteres",
          "[internal, make_lower_case]")
{
  const auto processor = core::TextProcessor();

  SECTION("deveria transformar maiúsculas em minúsculas")
  {
    for (char letter = 65; letter <= 90; letter++)
    {
      REQUIRE(processor.to_lower_case(letter) == letter + 32);
    }
  }

  SECTION("não deveria alterar caracteres que não são letras")
  {
    for (char non_letter = 0; non_letter < 65; non_letter++)
    {
      REQUIRE(processor.to_lower_case(non_letter) == non_letter);
    }
  }

  SECTION("não deveria alterar letras que já estão em minúsculo")
  {
    for (char already_lower_case = 97; already_lower_case <= 122;
         already_lower_case++)
    {
      REQUIRE(processor.to_lower_case(already_lower_case) ==
              already_lower_case);
    }
  }
}

TEST_CASE("Deveria remover os acentos configurados",
          "[internal, remove_diacritics]")
{
  const auto processor = core::TextProcessor();
  std::string input = "coração não pão É Ô ä ü";
  const auto expected = "coracao nao pao E O a u";
  processor.remove_diacritics(input);
  REQUIRE(input == expected);
}

TEST_CASE("Deveria transformar um texto de input em um vetor de palavras ASCII "
          "em minúsculo e sem stopwords",
          "[internal, process, remove_diacritics, make_lower_case]")
{
  const auto stopwords = std::unordered_set<std::string>{
      "o", "a", "e", "do", "da", "de",
  };
  const auto processor = core::TextProcessor(stopwords);
  const auto input =
      "O cálculo de quão eficaz a fUnção de Indexação é, passou por ALTERAÇÕES "
      "no último mês. O AVIÃO pousou em São JoÃo e fez o açúcar subir.";
  auto input_stream = std::istringstream(input);

  const auto words = processor.process(input_stream);
  const auto expected_words = std::vector<std::string>{
      "calculo",    "quao", "eficaz", "funcao", "indexacao", "passou", "por",
      "alteracoes", "no",   "ultimo", "mes",    "aviao",     "pousou", "em",
      "sao",        "joao", "fez",    "acucar", "subir",
  };

  REQUIRE(std::ranges::is_permutation(words, expected_words));
}

TEST_CASE("Não deveria separar palavras unidas por conectores",
          "[internal, process, allowed_connectors, is_connected]")
{
  const auto processor = core::TextProcessor();
  const auto input = "É um não-não!";
  auto input_stream = std::istringstream(input);

  const auto words = processor.process(input_stream);
  const auto expected_words = std::vector<std::string>{"e", "um", "nao-nao"};

  REQUIRE(std::ranges::is_permutation(words, expected_words));
}
