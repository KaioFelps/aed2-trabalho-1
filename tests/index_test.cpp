#include "unit_test.hpp"

#include "indexer.hpp"
#include <catch2/catch_all.hpp>
#include <file.hpp>

using core::File;
using core::Index;
using core::Indexer;

static bool are_equivalent(std::unordered_set<uint64_t> &set,
                           std::initializer_list<uint64_t> _ids)
{
  std::vector ids(_ids);
  if (ids.size() != set.size()) return false;
  return std::all_of(ids.begin(), ids.end(),
                     [&set](const auto &id) { return set.contains(id); });
}

TEST_CASE("Deveria registrar que uma palavra foi encontrada em um arquivo",
          "[external, search, add_word_from_file]")
{
  const auto doc1 = "doc1.txt";
  const auto doc2 = "doc2.txt";

  auto index = Index();
  // lendo o arquivo doc1.txt
  index.add_word_from_file("gato", doc1);
  index.add_word_from_file("está", doc1);
  index.add_word_from_file("comendo", doc1);
  index.add_word_from_file("telhado", doc1);

  // lendo o arquivo doc2.txt
  index.add_word_from_file("cachorro", doc2);
  index.add_word_from_file("está", doc2);
  index.add_word_from_file("comendo", doc2);
  index.add_word_from_file("quintal", doc2);

  REQUIRE(are_equivalent(index.words["comendo"], {1, 2}));
  REQUIRE(are_equivalent(index.words["gato"], {1}));
  REQUIRE(are_equivalent(index.words["está"], {1, 2}));
  REQUIRE(are_equivalent(index.words["telhado"], {1}));
  REQUIRE(are_equivalent(index.words["cachorro"], {2}));
  REQUIRE(are_equivalent(index.words["quintal"], {2}));
  REQUIRE(are_equivalent(index.words["quintal"], {2}));
}

TEST_CASE("O conjunto ℝ de arquivos resultante da busca por todas as "
          "palavras de um conjunto ℙ deveria satisfazer a propriedade "
          "∀𝕒∊𝔽(ℙ⊆𝕒 ↔ 𝕒∊ℝ)",
          "[external, search, get_files_containing_words]")
{
  auto files = std::vector<File>{};
  files.push_back(File("doc1.txt")); // id = 1
  files.push_back(File("doc2.txt")); // id = 2

  auto words_map = words_map_t();
  words_map["comendo"] = {1, 2};
  words_map["gato"] = {1};
  words_map["está"] = {1, 2};
  words_map["telhado"] = {1};
  words_map["cachorro"] = {2};
  words_map["quintal"] = {2};

  REQUIRE(words_map["comendo"].size() == 2);
  REQUIRE(words_map["gato"].size() == 1);
  REQUIRE(words_map["está"].size() == 2);
  REQUIRE(words_map["quintal"].size() == 1);

  auto index = Index(files, words_map);

  SECTION("Deveria retornar todos os arquivos que contém uma única palavra")
  {
    auto files_found = index.get_files_containing_words({"comendo"});
    REQUIRE(files_found.size() == 2);
    REQUIRE(files_found.contains(files[0]));
    REQUIRE(files_found.contains(files[1]));
  }

  SECTION("Deveria retornar a interseção dos arquivos que contém todas as "
          "palavras")
  {
    auto words = std::vector<std::string>{"gato", "está", "comendo"};
    auto files_found = index.get_files_containing_words(words);
    REQUIRE(files_found.size() == 1);
    REQUIRE(files_found.contains(files[0]));
  }

  SECTION("Deveria retornar um conjunto vazio caso não exista um arquivo com "
          "todos os termos")
  {
    auto words = std::vector<std::string>{"gato", "está", "comendo", "quintal"};
    auto files_found = index.get_files_containing_words(words);
    REQUIRE(files_found.empty());
  }
}
