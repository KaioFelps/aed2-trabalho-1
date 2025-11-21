#include "unit_test.hpp"

#include "index_serializer.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("Deveria conseguir serializar um mapa de palavras de modo que ele "
          "possa ser deserializado posteriormente corretamente",
          "[internal, serialization, serialize_words_map]")
{
  auto map = words_map_t();
  map["tereré"] = {2, 1};
  map["samambaiaçu"] = {1, 53, 10, 3};
  map["baz"] = {2};

  auto stream = std::stringstream();
  core::IndexSerializer::serialize_words_map(map, stream);

  stream.seekg(0, std::ios::beg);
  auto deserialized_map =
      core::IndexSerializer::deserialize_words_map(stream, "mock.dat");

  SECTION("O mapa deveria ter sido reconstruído de forma equivalente a sua "
          "versão original")
  {
    REQUIRE(deserialized_map.find("baz")->second.size() == 1);
    REQUIRE(deserialized_map.find("baz")->second.contains(2));

    REQUIRE(deserialized_map.find("tereré")->second.size() == 2);
    REQUIRE(deserialized_map.find("tereré")->second.contains(1));
    REQUIRE(deserialized_map.find("tereré")->second.contains(2));

    REQUIRE(deserialized_map.find("samambaiaçu")->second.size() == 4);
    REQUIRE(deserialized_map.find("samambaiaçu")->second.contains(1));
    REQUIRE(deserialized_map.find("samambaiaçu")->second.contains(3));
    REQUIRE(deserialized_map.find("samambaiaçu")->second.contains(53));
    REQUIRE(deserialized_map.find("samambaiaçu")->second.contains(10));
  }
}

TEST_CASE("Deveria conseguir serializar o vetor de arquivos conforme o "
          "protocolo binário, de modo que ele possa ser deserializado "
          "posteriormente corretamente",
          "[internal, serialization, serialize_files_set]")
{
  std::vector<core::File> files = {
      core::File("/foo.txt"),
      core::File("/bar.txt"),
  };

  auto stream = std::stringstream();
  core::IndexSerializer::serialize_files_set(files, stream);

  stream.seekg(0, std::ios::beg);
  auto deserialized_files =
      core::IndexSerializer::deserialize_files_set(stream, "mock.dat");

  SECTION("Deveria retornar um vetor com tamanho exato ao original")
  {
    REQUIRE(files.size() == deserialized_files.size());
  }

  SECTION("Deveria recriar o vetor de forma idêntica ao original")
  {
    for (size_t i = 0; i < files.size(); i++)
    {
      REQUIRE(files[i] == deserialized_files[i]);
    }
  }
}
