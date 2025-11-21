#include "unit_test.hpp"

#include "errors/malformed_data_file.hpp"
#include "file.hpp"
#include "index_serializer.hpp"
#include <catch2/catch_all.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

typedef struct
{
  uint32_t bytes_caminho;
  std::string caminho;
} Tuple;

TEST_CASE("Deveria conseguir deserializar uma lista de arquivos conformante "
          "com o protocolo de serialização",
          "[internal, deserialization, deserialize_files_set]")
{
  auto expectedFile1 = core::File("/foo.txt");
  auto expectedFile2 = core::File("/bar.txt");

  auto qtd_de_items = 2;
  auto tupla_1 = Tuple{.caminho = expectedFile1.get_path(), .bytes_caminho = 8};
  auto tupla_2 = Tuple{.caminho = expectedFile2.get_path(), .bytes_caminho = 8};

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
