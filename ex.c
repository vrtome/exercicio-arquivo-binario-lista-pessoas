#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tPessoa {
    char nome[100];
    char dataNascimento[11];
    char cpf[15];
} tPessoa;

void DesalocaPessoas(tPessoa **pessoas, int nPessoas) {
    for (int i = 0; i < nPessoas; i++) {
        free(pessoas[i]);
    }
    free(pessoas);
}

char *ReadString(const char *prompt, char *dest, size_t size) {
    size_t i = 0;
    int c;

    printf("%s: ", prompt);
    fflush(stdout);
    while ((c = getchar()) != EOF && c != '\n') {
        if (i + 1 < size) {
            dest[i++] = (char)c;
        }
    }
    if (size) {
        dest[i] = '\0';
    }
    printf("\n");
    // return dest unless at end of file
    return (c == EOF && i == 0) ? NULL : dest;
}

tPessoa **LePessoasInput(int *nPessoas_ptr) {
    tPessoa **pessoas = NULL;
    int nPessoas = 0;

    for (;;) {
        tPessoa pessoa = { 0 };

        printf("----------- NOVA PESSOA -----------\n");
        if (!ReadString("Nome", pessoa.nome, sizeof pessoa.nome)
        ||  !ReadString("Data de nascimento", pessoa.dataNascimento, sizeof pessoa.dataNascimento)
        ||  !ReadString("CPF", pessoa.cpf, sizeof pessoa.cpf)) {
            /* input failure */
            break;
        }
        pessoas = (tPessoa **)realloc(pessoas, sizeof(tPessoa *) * (nPessoas + 1));
        pessoas[nPessoas] = (tPessoa *)calloc(sizeof(tPessoa), 1);
        *(pessoas[nPessoas]) = pessoa;
        nPessoas += 1;

        char controle = 'n';
        for (;;) {
            printf("Deseja adicionar outra pessoa? [s/n] ");
            scanf(" %c%*c", &controle);
            printf("\n");
            if (strchr("nNsSyY", controle))
                break;
        }
        if (controle == 'n' || controle == 'N')
            break;
    }
    *nPessoas_ptr = nPessoas;
    return pessoas;
}

void EscrevePessoasBinario(tPessoa **pessoas, int nPessoas) {
    int pessoasGravadas = 0;
    FILE *arquivo = fopen("pessoas.bin", "wb");
    if (arquivo == NULL) {
        return;
    }
    for (int i = 0; i < nPessoas; i++) {
        if (fwrite(pessoas[i], sizeof(tPessoa), 1, arquivo) != 1) {
            fclose(arquivo);
            return;
        }
        pessoasGravadas++;
    }

    printf("Pessoas gravadas: %d\n", pessoasGravadas);
    fclose(arquivo);
}

void ImprimePessoasBinario(int nPessoas) {
    tPessoa pessoa = { 0 };
    FILE *arquivo = fopen("pessoas.bin", "rb");
    if (arquivo == NULL) {
        return;
    }
    int nPessoasLidas = 0;
    for (int i = 0; i < nPessoas; i++) {
        if (fread(&pessoa, sizeof(tPessoa), 1, arquivo) != 1) {
            break;
        }
        nPessoasLidas++;
        printf("-----------------------------------\n");
        printf("%s\n", pessoa.nome);
        printf("%s\n", pessoa.dataNascimento);
        printf("%s\n", pessoa.cpf);
        printf("-----------------------------------\n");
    }

    printf("Pessoas lidas: %d\n", nPessoasLidas);
    fclose(arquivo);
}

int main(void) {
    int nPessoas = 0;
    tPessoa **pessoas = LePessoasInput(&nPessoas);
    EscrevePessoasBinario(pessoas, nPessoas);
    ImprimePessoasBinario(nPessoas);
    DesalocaPessoas(pessoas, nPessoas);
    return 0;
}