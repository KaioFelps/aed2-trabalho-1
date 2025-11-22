# Algoritmos e Estruturas De Dados 2 :: Trabalho 1
Resolução do trabalho 1 da matéria de Algoritmos e Estruturas de Dados 2.

## Requisitos
- [] Ser compilável via Makefile;
- [x] Modo Indexação:
  - `indice construir <caminho_do_diretório>`
  - varre o diretório especificado, constrói o índice invertido e o salva
    em algum arquivo padrão no diretório atual.
- [x] Modo Busca:
  - `indice buscar <termo_de_busca>`
  - exige que o índice tenha sido previamente construído;
  - reconstrói o índice em memória;
  - busca os arquivos que contém <termo_de_busca> e os imprime no terminal.
- [x] Serialização e Deserialização:
  - implementação de (de)serialização está especificada no documento [Serialização].
- [x] Pré-processamento de palavras:
  - [x] remover pontuações;
  - [x] converter maiúsculas para minúsculas;
  - [x] eliminar stopwords.

[Serialização]: ./docs/serialization.md

## Rodando a aplicação
Para simplesmente rodar o executável deste programa, utilize o comando
`make build`. Essa forma de compilação só está disponível para sistemas
UNIX.

### Notação
Palavras entre angulares (<>) serão definidas após o comando.
Elementos entre colchetes ([]) são opcionais.

### Criando o índice
`<executável> indice construir <caminho>`
- executável: o caminho para o executável gerado. Por padrão, `./build/indexer`.
- caminho: o caminho (path) para um arquivo texto ou diretório com arquivos de
  textos e/ou outros diretórios. Por exemplo: `caminho/para/diretorio/de/textos/`.

Lê o arquivo de texto ou atravessa o diretório lendo todos os arquivos nele contido
para construir o índice. O caminho funciona de forma relativa a este diretório. Os
arquivos serão, posteriormente, representados por seus caminhos canônicos.

### Buscando
`<executável> indice buscar <termo1> [<termo2> <termo3> ... <termon>]`
- executável: o caminho para o executável gerado. Por padrão, `./build/indexer`.
- termo: uma palavra ASCII ou UTF-8. Termos separados por espaços são interpretados
  como uma lista de termos.

Exige que o índice tenha sido previamente criado. Exige pelo menos 1 termo de busca.
Imprime na tela os caminhos canônicos de todos os arquivos que possuem `termo`.
Se múltiplos termos forem fornecidos, é garantido que todos os arquivos impressos
possuem todos os termos.

## Testando
Para rodar em desenvolvimento e executar os testes, é necessário instalar
as dependências de desenvolvimento e a ferramenta utilizada para compilar os
testes. Siga o passo-a-passo adiante.

### Instalando dependências com o vcpkg
Siga os passos descritos no [guia de instalação do vcpkg]. Instale as
dependências necessárias para esta aplicação com o comando `vcpkg install`.

[guia de instalação do vcpkg]: https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash

### Instalando e configurando o easexx
O Easexx é a ferramenta utilizada para compilar o programa e rodar os
testes. Copie e cole o script abaixo para baixar e compilar o easexx.
Garanta que exista uma [instalação do (Rust e) Cargo] para compilar.

```
curl https://raw.githubusercontent.com/kaiofelps/easexx/main/install.sh | sh
```

Acesse o arquivo [build.json](/build.json) e altere os diretórios em
`devLibDirs` e `includeDirs` para refletir o diretório de pacotes do vcpkg
na sua máquina.

Caso utilize outro compilador que não o g++, altere o campo `compiler`.
Note que o compilador escolhido deve atender aos mesmos comandos que o g++.

[instalação do (Rust e) Cargo]: https://www.rust-lang.org/tools/install
