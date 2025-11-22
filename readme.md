# Algoritmos e Estruturas De Dados 2 :: Trabalho 1
Resolução do trabalho 1 da matéria de Algoritmos e Estruturas de Dados 2.

## Requisitos
- [ ] Ser compilável via Makefile;
- [ ] Modo Indexação:
  - `indice construir <caminho_do_diretório>`
  - varre o diretório especificado, constrói o índice invertido e o salva
    em algum arquivo padrão no diretório atual.
- [ ] Modo Busca:
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
