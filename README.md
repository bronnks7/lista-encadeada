# Lista encadeada

Projeto didático com o programa original em C e uma demonstração interativa para navegador. A versão web permite incluir registros no início, no fim ou em uma posição, consultar por código, alterar nomes, remover registros e visualizar a ordem da lista.

## Executar o programa em C

Com GCC instalado, compile e execute:

```sh
gcc -Wall -Wextra -o lista lista_encadeadaJoaoVictor.c
./lista
```

No Windows, execute `lista.exe` após a compilação.

## Abrir a versão web localmente

Abra `index.html` em um navegador. A página não precisa de dependências ou servidor. Os dados da demonstração ficam apenas na sessão atual do navegador e são descartados ao recarregar a página.

## Publicação

O GitHub Actions publica automaticamente os arquivos da página no GitHub Pages a cada atualização da branch `main`.

Endereço esperado: <https://bronnks7.github.io/lista-encadeada/>
