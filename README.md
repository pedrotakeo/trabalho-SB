# Trabalho Final de Software Básico
Camila Shibata (GRR20245211) | Pedro Shima (GRR20240627)

## Detalhes de Implementação
Para utilizar as funções implementadas, não deve haver mistura com funções da *libc*.  
É necessário chamar a função `printf()` no início do programa para que a *libc* realize suas alocações internas antes da manipulação direta da heap. Dessa forma, evitamos desalocar memória fora da nossa responsabilidade.
Como o enunciado não menciona a necessidade de fazer *merge* de blocos livres adjacentes, decidimos fazer uma implementação mais simples e iterativa.

## Estrutura de Bloco
Cada bloco de memória é organizado da seguinte forma:
 
[ USO (1B) | TAMANHO (8B) | DADOS (variável) ]

### `setup_brk()` e `dismiss_brk()`
A função `setup_brk()` obtém o valor inicial do *break* do processo via syscall `sys_brk(0)` e o armazena em `ini_brk` e `cur_brk`.  
Já `dismiss_brk()` restaura o *break* inicial utilizando `ini_brk`.

### `memory_alloc()`
A função `memory_alloc(bytes)` utiliza o algoritmo **Worst-Fit**, que busca o maior bloco livre capaz de acomodar o tamanho solicitado.  
Caso um bloco adequado seja encontrado, ele é marcado como usado.  
Se houver espaço restante maior que 10 bytes, o bloco é dividido e o espaço excedente é tratado como um novo bloco livre.  
Se nenhum bloco disponível for suficiente, a heap é expandida via *brk*.

### `memory_free(pointer)`
A função `memory_free(pointer)` marca o bloco apontado como livre.  
Retorna:
- `0` em caso de sucesso;  
- `-1` se o ponteiro for inválido (`NULL`, menor que `ini_brk` ou maior que `cur_brk`).

---
