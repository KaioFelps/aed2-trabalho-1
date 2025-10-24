# Serialização
Esse documento descreve os protocolos binários de serialização e deserialização
utilizado para armazenar o índice num arquivo binário.

É necessário salvar 2 arquivos para remontar o índice:
1. o arquivo de índice que armazena o conjunto de arquivos lidos, mapeando seus IDs
  para seus caminhos absolutos;
2. o arquivo do mapa de palavras, que mapeia cada palavra para um conjunto de IDs
  dos arquivos (`File`) nos quais foi citada ao menos uma vez.

## Interpretando um arquivo binário

### Sequências de tamanho
As sequências de bits prefixadas com "tamanho_" na representação em XML acima
servem para descrever o tamanho de cada parte do arquivo final a ser montado
quando finalmente deserializado.

Observe que todos eles possuem o atributo `size="32"`. Isso significa que essas
sequências são garantidas de ter **32 bits** de tamanho (não bytes), possibilitando
sua leitura e interpretação com facilidade.

Os valores de uma sequência de bits de tamanho devem ser interpretados como
**bytes**, não bits. Por exemplo, se há uma tag
`<tamanho_chave size="32" virtual-type="uint32_t" />`, o valor obtido ao interpretá-la
como o tipo C++ `uint32_t` **descreve o tamanho da chave em bytes**.

Com os tamanhos em mãos, o deserializador sabe quantos bits pular para chegar
aonde quiser dentro desse arquivo, bem como até onde deve ler para deserializar
um pedaço de uma estrutura/classe C++.

### O que significam tags fechadas e abertas
Tags que se auto-fecham (digamos `<chave />`) representam uma sequência de bits
real dentro do arquivo.

Tags que contém outras tags dentro de si (e, logo, não se fecham sozinhas,
pois precisam de uma segunda tag de fechamento) são meramente organizacionais
e não representam nada no arquivo. Permitem que o desenvolvedor
entenda que um conjunto de bits pertence a uma estrutura particular quando
lendo este documento.

Por exemplo, a tag `<item>...</item>` envelopa sequências de bits relativas
a um item específico.

### Mapa de palavras
Esse mapa, na sua versão virtual — isto é, sua representação por tipos C++ —,
é uma instância de `std::unordered_map<std::string, std::set<std::string>>`,
onde a chave é uma palavra e o valor é um conjunto de IDs de arquivos (`File`).

Sua representação binária tem o seguinte formato:
```xml
<quantidade_de_tuplas />
<tupla>
  <tamanho_chave size="32" virtual-type="uint32_t" />
  <quantidade_de_items_no_conjunto size="32" virtual-type="uint32_t" />
  <chave virtual-type="std::string" />
  <item n="1">
    <tamanho_id size="32" virtual-type="uint32_t" />
    <id virtual-type="std::string" />
  </item n="1">
    ⠇
  <item n="n">
    <tamanho_id size="32" virtual-type="uint32_t" />
    <id virtual-type="std::string" />
  </item n="n">
</tupla>
```

Podemos interpretar esse arquivo da seguinte forma:
A tag `<quantidade_de_tuplas />` representa a quantidade de tuplas a serem
deserializadas.

Para cada tupla (um registro em um mapa), devemos interpretar da seguinte forma:
- a sequência descrita por `<tamanho_chave />` descreve o tamanho (em bytes) da chave;
- a sequência descrita por `<quantidade_de_items_no_conjunto />` diz quantas ocorrências
de `<item>`s existem até o final do arquivo;
- a tag `<chave>`, de tamanho variável informado pela tag `<tamanho_chave>`, deve ser
interpretada como uma instância de `std::string` (C++).

Para cada ocorrência de `<item>` (número de ocorrências variável, porém informado
pela tag `<quantidade_de_items_no_conjunto`), deve-se interpretar da seguinte forma
[de modo a se obter uma instância do tipo `std::set<File>` (C++)]:
- a sequência descrita pela tag `<tamanho_id />` descreve o tamanho da string contendo o ID
  (`File::id`) da instância de `File` sendo deserializada;
- a tag `<id />`, de tamanho variável discriminado pelo valor dos bits de `<tamanho_id>`,
  deve ser interpretada como uma instância de `std::string` (C++) e como o campo `id`
  desta instância de `File`.

### Índice

O índice — representado, virtualmente, por `std::set<File>` — é um conjunto de
`File`s. Cada instância de `File` é uma tupla {id, caminho} de um arquivo real.
Dessa forma, podemos representá-lo, binariamente, da seguinte maneira:

```xml
<quantidade_de_items size="32" virtual-type="uint32_t" />
<tupla n="1">
  <tamanho_id size="32" virtual-type="uint32_t" />
  <tamanho_caminho size="32" virtual-type="uint32_t" />
  <id />
  <caminho />
</tupla>
  ⠇
<tupla n="n">
    <tamanho_id size="32" virtual-type="uint32_t" />
    <tamanho_caminho size="32" virtual-type="uint32_t" />
    <id />
    <caminho />
</tupla>
```

A sequência de bits representada pela tag `<quantidade_de_items>` é o tamanho do conjunto.
Seja `n` o valor que essa sequência representa, temos, então, `n` tuplas a serem
deserializadas dentro deste arquivo.

Para cada tupla (que, lembrando, é apenas uma marcação e não um valor real), devemos
interpretar:
- a sequência representada pela tag `<tamanho_id>` como o tamanho (em bytes) do `<id>`;
- a sequência representada pela tag `<tamanho_caminho>` como o tamanho  (em bytes)
  do `<caminho>`;
- a sequência representada pela tag `<id />`, de tamanho variável discriminado pelo valor
  de `<tamanho_id>`, como uma `std::string` (C++) e como o valor de `File::id`;
- a sequência representada pela tag `<caminho />`, de tamanho variável discriminado pelo
  valor de `<tamanho_caminho>`, como uma instância de `std::string` (C++) e como valor
  de `File::path`.
