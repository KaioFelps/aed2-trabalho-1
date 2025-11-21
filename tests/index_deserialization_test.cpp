#include "unit_test.hpp"

#include "errors/malformed_data_file.hpp"
#include "file.hpp"
#include "index_serializer.hpp"
#include <catch2/catch_all.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

struct FileTuple
{
  uint32_t bytes_caminho;
  std::string caminho;
};

struct MapTuple
{
  uint32_t bytes_palavra;
  uint32_t qtd_de_ids;
  std::string palavra;
  std::vector<uint8_t> ids;
};

TEST_CASE("Deveria conseguir deserializar uma lista de arquivos conformante "
          "com o protocolo de serialização",
          "[internal, deserialization, deserialize_files_set]")
{

  auto expectedFile1 = core::File("/foo.txt");
  auto expectedFile2 = core::File("/bar.txt");

  auto qtd_de_items = 2;
  auto tupla_1 =
      FileTuple{.caminho = expectedFile1.get_path(), .bytes_caminho = 8};
  auto tupla_2 =
      FileTuple{.caminho = expectedFile2.get_path(), .bytes_caminho = 8};

  auto stream = std::stringstream();
  stream.write(reinterpret_cast<char *>(&qtd_de_items), 4);
  stream.write(reinterpret_cast<char *>(&tupla_1.bytes_caminho), 4);
  stream.write(tupla_1.caminho.data(), 8);
  stream.write(reinterpret_cast<char *>(&tupla_2.bytes_caminho), 4);
  stream.write(tupla_2.caminho.data(), 8);

  REQUIRE(stream.good());

  auto files = core::IndexSerializer::deserialize_files_set(stream, "mock.dat");
  REQUIRE(files.size() == 2);

  SECTION(
      "Ambos os arquivos devem existir no conjunto de arquivos deserializados")
  {
    REQUIRE(std::find(files.begin(), files.end(), expectedFile1) !=
            files.end());

    REQUIRE(std::find(files.begin(), files.end(), expectedFile2) !=
            files.end());
  }

  SECTION("O processo de deserialização deveira ler até o último bit de uma "
          "representação binária correta")
  {
    const auto cursor_current_pos = stream.tellg();
    stream.seekg(0, std::ios::end);
    const auto last_stream_byte = stream.tellg();
    REQUIRE(cursor_current_pos == last_stream_byte);
  }
}

TEST_CASE("Deveria conseguir deserializar um mapa de palavras conformante com "
          "o protocolo de serialização",
          "[internal, deserialization, deserialize_words_map]")
{

  std::vector<MapTuple> map_tuples = {
      MapTuple{
          .palavra = "baz", .bytes_palavra = 3, .qtd_de_ids = 1, .ids = {2}},
      MapTuple{.palavra = "tereré",
               .bytes_palavra = 7,
               .qtd_de_ids = 2,
               .ids = {1, 2}},
      MapTuple{.palavra = "samambaiaçu",
               .bytes_palavra = 12,
               .qtd_de_ids = 4,
               .ids = {1, 10, 53, 3}},
  };

  auto stream = std::stringstream();

  auto quantidade_de_tuplas = static_cast<uint32_t>(map_tuples.size());
  stream.write(reinterpret_cast<char *>(&quantidade_de_tuplas), 32 / 8);

  for (auto &tuple : map_tuples)
  {
    stream.write(reinterpret_cast<char *>(&tuple.bytes_palavra), 32 / 8);
    stream.write(reinterpret_cast<char *>(&tuple.qtd_de_ids), 32 / 8);
    stream.write(tuple.palavra.data(), tuple.bytes_palavra);
    for (auto &id : tuple.ids)
    {
      auto id_as_64 = static_cast<uint64_t>(id);
      stream.write(reinterpret_cast<char *>(&id_as_64), 64 / 8);
    }
  }

  REQUIRE(stream.good());
  auto words_map =
      core::IndexSerializer::deserialize_words_map(stream, "mock.dat");

  SECTION("O processo de deserialização deveira ler até o último bit de uma "
          "representação binária correta")
  {
    REQUIRE(words_map.size() == 3);
    const auto cursor_current_pos = stream.tellg();
    stream.seekg(0, std::ios::end);
    const auto last_stream_byte = stream.tellg();
    REQUIRE(cursor_current_pos == last_stream_byte);
  }

  SECTION("Todas as palavras deveriam ser mapeadas corretamente para uma lista "
          "de IDs que possuíam anteriormente a serialização")
  {
    REQUIRE(words_map.find("baz")->second.size() == 1);
    REQUIRE(words_map.find("baz")->second.contains(2));

    REQUIRE(words_map.find("tereré")->second.size() == 2);
    REQUIRE(words_map.find("tereré")->second.contains(1));
    REQUIRE(words_map.find("tereré")->second.contains(2));

    REQUIRE(words_map.find("samambaiaçu")->second.size() == 4);
    REQUIRE(words_map.find("samambaiaçu")->second.contains(1));
    REQUIRE(words_map.find("samambaiaçu")->second.contains(3));
    REQUIRE(words_map.find("samambaiaçu")->second.contains(53));
    REQUIRE(words_map.find("samambaiaçu")->second.contains(10));
  }
}
