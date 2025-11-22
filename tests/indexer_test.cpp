#include "unit_test.hpp"

#include "indexer.hpp"
#include "text_processor.hpp"
#include <catch2/catch_all.hpp>
#include <filesystem>

#include <iostream>

TEST_CASE(
    "Deveria atravessar o diretório especificado para construir um índice",
    "[indexer, external, build]")
{
  auto processor = core::TextProcessor();
  auto lookup_dir = std::filesystem::path("tests/fixtures/lookup_dir");
  auto indexer = core::Indexer("build/__test__/fixtures/", processor);
  indexer.build(lookup_dir);

  REQUIRE(indexer.index.has_value());
  REQUIRE(indexer.get_index().words["bumpin'"].size() == 2);
  REQUIRE(indexer.get_files_with_words({"bumpin'", "that"}).size() == 2);
  REQUIRE(indexer.get_files_with_words({"i"}).size() == 2);
  REQUIRE(indexer.get_files_with_words({"360"}).size() == 1);
  REQUIRE(indexer.get_files_with_words({"foo"}).size() == 0);
  REQUIRE(indexer
              .get_files_with_words({
                  "i",
                  "can't",
                  "wait",
                  "to",
                  "try",
                  "him",
              })
              .size() == 1);
  REQUIRE(indexer.get_files_with_words({"Coração", "PAIXÃO"}).size() == 1);
}
