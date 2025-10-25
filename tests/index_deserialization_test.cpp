#include "errors/malformed_data_file.hpp"
#include "file.hpp"
#include "index_serializer.hpp"
#include <catch2/catch_all.hpp>
#include <filesystem>

#include <iostream>

namespace fs = std::filesystem;

typedef struct
{
  uint32_t bytes_id;
  uint32_t bytes_caminho;
  std::string id;
  std::string caminho;
} Tuple;

TEST_CASE("Deveria conseguir deserializar uma lista de arquivos conformante "
          "com o protocolo de serialização",
          "[internal, deserialization, deserialize_files_set]")
{
  auto expectedFile1 = core::File("1", "/foo.txt");
  auto expectedFile2 = core::File("2", "/bar.txt");

  auto qtd_de_items = 2;
  auto tupla_1 = Tuple{.id = expectedFile1.get_id(),
                       .caminho = expectedFile1.get_path(),
                       .bytes_caminho = 8,
                       .bytes_id = 1};
  auto tupla_2 = Tuple{.id = expectedFile2.get_id(),
                       .caminho = expectedFile2.get_path(),
                       .bytes_caminho = 8,
                       .bytes_id = 1};

  auto stream = std::stringstream();
  stream.write(reinterpret_cast<char *>(&qtd_de_items), 4);
  stream.write(reinterpret_cast<char *>(&tupla_1.bytes_id), 4);
  stream.write(reinterpret_cast<char *>(&tupla_1.bytes_caminho), 4);
  stream.write(reinterpret_cast<char *>(tupla_1.id.data()), 1);
  stream.write(tupla_1.caminho.data(), 8);
  stream.write(reinterpret_cast<char *>(&tupla_2.bytes_id), 4);
  stream.write(reinterpret_cast<char *>(&tupla_2.bytes_caminho), 4);
  stream.write(reinterpret_cast<char *>(tupla_2.id.data()), 1);
  stream.write(tupla_2.caminho.data(), 8);

  REQUIRE(stream.good());

  auto files = core::IndexSerializer::deserialize_files_set(stream, "mock.dat");
  REQUIRE(files.size() == 2);

  SECTION(
      "Ambos os arquivos devem existir no conjunto de arquivos deserializados")
  {
    REQUIRE(files.find(expectedFile1) != files.end());
    REQUIRE(files.find(expectedFile2) != files.end());
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
