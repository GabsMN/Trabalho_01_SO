// Arquivo processo_b.c
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_A_B "/fila_a_b"
#define QUEUE_B_A "/fila_b_a"

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

    // Abre/cria a fila A->B para LEITURA e a fila B->A para ESCRITA
    queue_a_b = mq_open(QUEUE_A_B, O_RDONLY | O_CREAT, 0666, &attr_req);
    queue_b_a = mq_open(QUEUE_B_A, O_WRONLY | O_CREAT, 0666, &attr_resp);

    if (queue_a_b < 0 || queue_b_a < 0) {
        perror("Erro ao abrir filas no Processo B");
        exit(1);
    }

    while (1) {
        // Recebe n1 e n2 de A
        if (mq_receive(queue_a_b, (void*)&req, sizeof(MensagemRequisicao), 0) < 0) {
            perror("mq_receive");
            exit(1);
        }

        // Realiza a soma
        soma = req.n1 + req.n2;

        // Aguarda 1 segundo conforme especificação
        sleep(1);

        // Envia a soma de volta para A
        if (mq_send(queue_b_a, (void*)&soma, sizeof(int), 0) < 0) {
            perror("mq_send");
            exit(1);
        }
    }

    return 0;
}