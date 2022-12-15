# Trabalho 2 - FSE

**Aluno:** Gabriel Davi Silva Pereira

**Matricula:** 17/0010341

Implementação do [trabalho 2 de FSE](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-2-2022-1). O trabalho consiste na implementação de um sistema embarcado de uma Air Fryer, que deve fazer o controle de um resistor e uma ventoinha de acordo com os dados recebidos dos sensores de temperatura.


## Como rodar

O programa utiliza de Makefile, e foi desenvolvido para ser compilado e executado em uma Raspiberry Pi. Para compilar o programa basta entrar na pasta uart_raspberry_pi utilizar o comando abaixo:

```sh
make all
```

A compilação gera o arquivo binário executável em `bin/bin`. Esse arquivo pode ser executado com o comando abaixo.

```sh
make run
```

## Instruções

Uma vez executado, o deve-se entrar no board correspondente a placa utilizada para execução. 
Primeiro, deve-se apertar o botão de ligar o sistema. A partir daí, uma vez que a luz acender,o usuário pode ajustar a temperatura de referência e tempo pelo board e então pressionar em "iniciar". Uma vez iniciado, no lcd o sistema deve informar o estado do sistema. Quando a temperatura interna estiver igual a referencia escolhida, as seguintes informações serão informadas: 

- TI: Temperatura Interna
- TE: Temperatura Externa
- TIMER: Tempo em segundos

Há também a possibilidade de escolher um modo pré-selecioando de execução. Para isso, basta pressionar o botão "Menu". No terminal, será apresentado modos de cozimento no terminal do programa: 

1 - Assar frango (Temperatura: 40, Tempo: 3 min)
2 - Descongelar Carne (Temperatura: 50, Tempo: 5 min)
3 - Fritar Nuggets (Temperatura: 30, Tempo: 1 min)

Uma vez selecionado o modo, o sistema irá ser iniciado. No LCD será possível verificar: 

- TI: Temperatura Interna
- TE: Temperatura Externa
- TIMER: Tempo em segundos
- M: Modo selecionado

Uma vez em execução, um arquivo de log pode ser acessado na raiz do programa com nome "log.csv".
