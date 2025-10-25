/**
 * @file index-serializer.hpp
 * @brief Implementação de (de)serialização do `Index`.
 *
 * A implementação da serialização e deserialização está especificada no
 * documento [Serialização](docs/serialization.md).
 */

#include "index_serializer.hpp"
#include "errors/malformed_data_file.hpp"
#include <algorithm>
#include <format>
#include <fstream>

namespace core
{

IndexSerializer::IndexSerializer(std::filesystem::path data_dir,
                                 std::filesystem::path words_file,
                                 std::filesystem::path index_file)
    : data_dir(data_dir), words_file(words_file), index_file(index_file)
{
}

std::optional<Index> IndexSerializer::deserialize()
{
  const auto words_data_file_path = this->data_dir / this->words_file;
  auto words_map_stream =
      std::ifstream(words_data_file_path, std::ios::binary | std::ios::in);

  const auto index_data_file = this->data_dir / this->index_file;
  auto files_set_stream =
      std::ifstream(index_data_file, std::ios::binary | std::ios::in);

  if (!words_map_stream || !files_set_stream)
  {
    words_map_stream.close();
    files_set_stream.close();

    this->remove_data_files();
    return std::nullopt;
  }

  auto _words_map =
      this->deserialize_words_map(words_map_stream, words_data_file_path);

  auto files = this->deserialize_files_set(files_set_stream, index_data_file);

  words_map_stream.close();
  files_set_stream.close();

  auto words_map = this->map_words_map_files_ids_to_files(files, _words_map);
  return Index(files, words_map);
}

void IndexSerializer::ensure_data_has_not_reached_end(
    const std::istream &rstream,
    const std::filesystem::path &file) noexcept(false)
{
  if (rstream.good()) return;
  throw errors::MalformedDataFileException(
      file, "O arquivo finalizou sem completar a serialização.");
}

std::unordered_map<std::string, std::set<std::string>>
IndexSerializer::deserialize_words_map(
    std::istream &binary_data,
    const std::filesystem::path &words_data_file_path)
{
  auto words_map = std::unordered_map<std::string, std::set<std::string>>();

  uint32_t tuples_amount;
  binary_data.read(reinterpret_cast<char *>(&tuples_amount), 32 / 8);

  ensure_data_has_not_reached_end(binary_data, words_data_file_path);

  for (size_t tuple_cursor = 0; tuple_cursor < tuples_amount; tuple_cursor++)
  {
    uint32_t word_size;
    uint32_t set_length;
    binary_data.read(reinterpret_cast<char *>(&word_size), 32 / 8);
    binary_data.read(reinterpret_cast<char *>(&set_length), 32 / 8);
    ensure_data_has_not_reached_end(binary_data, words_data_file_path);

    auto word = std::string(word_size, 0);
    binary_data.read(word.data(), word_size);

    auto &set = words_map[word];

    for (size_t _item_cursor = 0; _item_cursor < set_length; _item_cursor++)
    {
      uint32_t id_size;
      binary_data.read(reinterpret_cast<char *>(&id_size), 32 / 8);

      auto id = std::string();
      id.reserve(id_size);
      binary_data.read(reinterpret_cast<char *>(&id_size), 32 / 8);

      if (id.size() == id_size) set.insert(id);
    }

    if (set.size() != set_length)
    {
      throw errors::MalformedDataFileException(
          words_data_file_path,
          std::format("Encontrado tamanho inesperado de IDs. Esperava-se {}, "
                      "mas foi recebido {} até o final do arquivo.",
                      set_length, set.size()));
    }

    if (tuple_cursor < tuples_amount)
    {
      ensure_data_has_not_reached_end(binary_data, words_data_file_path);
    }
  }

  return words_map;
}

std::set<File> IndexSerializer::deserialize_files_set(
    std::istream &stream, const std::filesystem::path &index_data_file)
{
  auto files_set = std::set<File>();

  uint32_t set_length;
  stream.read(reinterpret_cast<char *>(&set_length), 32 / 8);
  ensure_data_has_not_reached_end(stream, index_data_file);

  for (size_t tuples_cursor = 0; tuples_cursor < set_length; tuples_cursor++)
  {
    uint32_t id_size;
    uint32_t path_size;
    stream.read(reinterpret_cast<char *>(&id_size), 32 / 8);
    stream.read(reinterpret_cast<char *>(&path_size), 32 / 8);

    auto id = std::string(id_size, 0);
    auto path = std::string(path_size, 0);

    stream.read(reinterpret_cast<char *>(id.data()), id_size);
    stream.read(reinterpret_cast<char *>(path.data()), path_size);

    if (tuples_cursor < set_length)
    {
      ensure_data_has_not_reached_end(stream, index_data_file);
    }

    files_set.insert(File(std::move(id), std::move(path)));
  }

  if (files_set.size() != set_length)
  {
    throw errors::MalformedDataFileException(
        index_data_file,
        std::format(
            "Encontrado tamanho inesperado de arquivos. Esperava-se {}, "
            "instâncias de `File`, mas foram recebidas {} até o final do "
            "arquivo.",
            set_length, files_set.size()));
  }

  return files_set;
}

std::unordered_map<std::string, std::set<File>>
IndexSerializer::map_words_map_files_ids_to_files(
    const std::set<File> &files,
    const std::unordered_map<std::string, std::set<std::string>> &words_map)
{
  auto words_final_map = std::unordered_map<std::string, std::set<File>>();

  for (auto &pair : words_map)
  {
    auto &files_set = words_final_map[pair.first];
    auto &files_ids = pair.second;

    std::for_each(files_ids.begin(), files_ids.end(),
                  [&files, &files_set](const std::string &file_id)
                  {
                    auto file = std::find_if(
                        files.begin(), files.end(), [&file_id](const File &file)
                        { return file.get_id() == file_id; });

                    if (file == files.end())
                    {
                      throw errors::MalformedDataFileException(std::format(
                          "Não foi encontrado nenhum arquivo registrado com o "
                          "ID {}, embora esperava-se que houvesse um.",
                          file_id));
                    }

                    files_set.insert(*file);
                  });
  }

  return words_final_map;
}

void IndexSerializer::remove_data_files() noexcept
{
  using std::filesystem::remove;
  remove(this->data_dir / this->index_file);
  remove(this->data_dir / this->words_file);
}
} // namespace core
