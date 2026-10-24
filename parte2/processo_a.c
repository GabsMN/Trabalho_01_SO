// Arquivo processo_a.c
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define QUEUE_A_B "/fila_a_b"
#define QUEUE_B_A "/fila_b_a"

// Estrutura para enviar os dois números de uma vez
typedef struct {
    int n1;
    int n2;
} MensagemRequisicao;

int main() {
    mqd_t queue_a_b, queue_b_a;
    struct mq_attr attr_req, attr_resp;
    MensagemRequisicao req;
    int soma;

    // Configura os atributos da fila de requisições (A -> B)
    attr_req.mq_maxmsg = 10;
    attr_req.mq_msgsize = sizeof(MensagemRequisicao);
    attr_req.mq_flags = 0;

    // Configura os atributos da fila de respostas (B -> A)
    attr_resp.mq_maxmsg = 10;
    attr_resp.mq_msgsize = sizeof(int);
    attr_resp.mq_flags = 0;

    // Abre/cria a fila A->B para ESCRITA e a fila B->A para LEITURA
    queue_a_b = mq_open(QUEUE_A_B, O_WRONLY | O_CREAT, 0666, &attr_req);
    queue_b_a = mq_open(QUEUE_B_A, O_RDONLY | O_CREAT, 0666, &attr_resp);

    if (queue_a_b < 0 || queue_b_a < 0) {
        perror("Erro ao abrir filas no Processo A");
        exit(1);
    }

    srand(time(NULL)); // Inicializa a semente do random

    while (1) {
        req.n1 = random() % 100;
        req.n2 = random() % 100;

        // Envia n1 e n2 para B
        if (mq_send(queue_a_b, (void*)&req, sizeof(MensagemRequisicao), 0) < 0) {
            perror("mq_send");
            exit(1);
        }

        // Aguarda e recebe a soma de B
        if (mq_receive(queue_b_a, (void*)&soma, sizeof(int), 0) < 0) {
            perror("mq_receive");
            exit(1);
        }

        // Imprime o resultado
        printf("%d + %d = %d\n", req.n1, req.n2, soma);
    }

    return 0;
}