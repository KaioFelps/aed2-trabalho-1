# Algoritmos e Estruturas De Dados 2 :: Trabalho 1
Resolução do trabalho 1 da matéria de Algoritmos e Estruturas de Dados 2.

## Rodando a aplicação
Siga os passos abaixo para entender como compilar e rodar o executável deste
programa.

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

## Serialização e deserialização
A implementação de (de)serialização está especificada no documento [Serialização].

[Serialização]: ./docs/serialization.md
