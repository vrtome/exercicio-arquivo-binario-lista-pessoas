#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ex
{
    char nome[100];
    char dataNascimento[11];
    char cpf[15];
} tPessoa;

void DesalocaPessoas(tPessoa** pessoas, int nPessoas) {
    for (int pessoa = 0; pessoa < nPessoas; pessoa++) {
        free(pessoas[pessoa]);
    }
    free(pessoas);
}

tPessoa** LePessoasInput(int* nPessoas) {
    tPessoa **pessoas = (tPessoa**) malloc(sizeof(tPessoa *));
 
    char controle = 0;

    while (1)
    {
        (*nPessoas)++;
        pessoas = (tPessoa **) realloc(pessoas, sizeof(tPessoa*) * (*nPessoas));
        pessoas[(*nPessoas) - 1] = (tPessoa *) calloc(1, sizeof(tPessoa));
        
        printf("----------- NOVA PESSOA -----------\n");
        printf("Nome: ");
        scanf("%[^\n]%*c", pessoas[(*nPessoas)-1]->nome);
        printf("Data de nascimento: ");
        scanf("%[^\n]%*c", pessoas[(*nPessoas)-1]->dataNascimento);
        printf("CPF: ");
        scanf("%[^\n]%*c", pessoas[(*nPessoas)-1]->cpf);
        while(1){
            printf("\nDeseja adicionar outra pessoa? [s/n] ");
            scanf("%c%*c", &controle);
            printf("\n");
            if ( controle == 'n' || controle == 'N' ) break;
            else if ( controle == 's' || controle == 'S' ) break;
        }
        if ( controle == 'n' || controle == 'N' ) break;   
    }

    return pessoas;
}

void EscrevePessoasBinario(tPessoa** pessoas, int nPessoas) {
    FILE *arquivo;
    int pessoasGravadas = 0;
    arquivo = fopen("pessoas.bin", "wb");

    if (arquivo == NULL) {
        return;
    }
    
    for ( int pessoa = 0; pessoa < nPessoas; pessoa++ ) {
        if ( fwrite(pessoas[pessoa], sizeof(tPessoa), 1, arquivo) != 1 ) {
            fclose(arquivo);
            return;
        }
        pessoasGravadas++;
    }
    
    printf("Pessoas gravadas: %d\n", pessoasGravadas);
    fclose(arquivo);
}

void ImprimePessoasBinario(int nPessoas) {
    FILE *arquivo;
    tPessoa* pessoaLida = (tPessoa*) malloc(sizeof(tPessoa));

    arquivo = fopen("pessoas.bin", "rb");

    if (arquivo == NULL) {
        free(pessoaLida);
        return;
    }

    int nPessoasLidas = 0;

    for (int pessoa = 0; pessoa < nPessoas; pessoa++) {
        if (fread(pessoaLida, sizeof(tPessoa), 1, arquivo) != 1) {
            break;
        }

        nPessoasLidas++;
        printf("-----------------------------------\n");
        printf("%s\n", pessoaLida->nome);
        printf("%s\n", pessoaLida->dataNascimento);
        printf("%s\n", pessoaLida->cpf);
        printf("-----------------------------------\n");
    }

    printf("Pessoas lidas: %d\n", nPessoasLidas);

    free(pessoaLida);
    fclose(arquivo);
}

int main() {
    int nPessoas = 0;

    tPessoa** pessoas = LePessoasInput(&nPessoas);
    EscrevePessoasBinario(pessoas, nPessoas);
    ImprimePessoasBinario(nPessoas);
    DesalocaPessoas(pessoas, nPessoas);

    return 0;
}