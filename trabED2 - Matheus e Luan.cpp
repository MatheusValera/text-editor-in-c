#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "meuconio.h"

struct tpcoluna{
	struct tpcoluna *ant, *prox;
	char letra;
};
typedef struct tpcoluna tpColuna;

struct tplinha{
	struct tplinha *top, *bot;
 	tpColuna *inicioL;
 	int nro;
};typedef struct tplinha tpLinha;

struct tplistagen{
	char letra, logico;
	struct tplistagen *prim, *prox;
};typedef struct tplistagen tpListaGen;

void limpaEscrita(void)
{
	int c,l;
	for(c=4; c<26; c++)
	{
		for(l=2; l<120; l++)
		{
			gotoxy(l,c);
			printf(" ");
		}
	}
}

void DesenhoTela()
{
	for(int i=0;i<120;i++)
	{
		gotoxy(i,1);printf("%c",205);
		gotoxy(i,28);printf("%c",205);
	}
	for(int i=1;i<28;i++)
	{
		gotoxy(1,i);printf("%c",186);
		gotoxy(120,i);printf("%c",186);
	}
	gotoxy(1,1);printf("%c",201);
	gotoxy(120,1);printf("%c",187);
	gotoxy(120,28);printf("%c",188);
	gotoxy(1,28);printf("%c",200);
	gotoxy(8,2);printf("F2 - Abrir                      F3 - Salvar                      F4 - Sair                      F5 - Exibir");
	gotoxy(2,3);printf("----------------------------------------------------------------------------------------------------------------------");
	gotoxy(2,26);printf("----------------------------------------------------------------------------------------------------------------------");
}

tpColuna *NCColuna()
{
	tpColuna *aux;
	aux = (tpColuna*)malloc(sizeof(tpColuna));
	aux->prox = NULL;
	aux->ant = NULL;
	aux->letra = 0;
	return aux;
}

tpLinha *NCLinha()
{
	tpLinha *aux;
	aux = (tpLinha*)malloc(sizeof(tpLinha));
	aux->top = NULL;
	aux->bot = NULL;
	aux->nro = 0;
	aux->inicioL = NCColuna();
	return aux;
}

void DestroiLista(tpLinha **L)
{
	tpLinha *excL, *auxL;
	tpColuna *excC, *auxC;
	while(auxL != NULL)
	{
		auxC = auxL->inicioL;
		while(auxC != NULL)
		{
			excC = auxC;
			auxC = auxC->prox;
			free(excC);
		}
		excL = auxL;
		auxL = auxL->bot;
		free(excL);
	}
	free(*L);
}

void ExibeFila(tpLinha *L,tpLinha **Inicio,tpLinha **Final)
{
	tpLinha *aux=L;
	tpColuna *AuxC;
	int negrito=0;
	int x=2,y=4,qtd=0;//
	while(aux->bot!=NULL)
	{
		qtd++;
		aux = aux->bot;
	}
	aux=L;
	while(qtd>=21)
	{
		aux = aux->bot;
		qtd--;
	}
	*Inicio = aux;
	while(aux!=NULL)
	{
		textcolor(7);
		AuxC = aux->inicioL;
		while(AuxC->prox!=NULL)
		{
			if(AuxC->letra !=21)
			{
				gotoxy(x,y);printf("%c",AuxC->letra);
				x++;
			}
			else
			{
				if(negrito==0)
				{
					textcolor(3);
					negrito=1;
				}
				else
				{
					textcolor(7);
					negrito =0;
				}
			}
			AuxC = AuxC->prox;
		}
		*Final = aux;
		aux = aux->bot;
		if(y<24)
			y++;
		x=2;
	}
}

void InsereCarac(tpLinha **linha, tpColuna **cursor, char letra,int *c,int *l)
{
	int apaga;
	tpColuna *Aux,*exclui,*palavras,*inserir,*pos;
	int cont;
	
	if((*linha)->nro<79)
	{
		if((*cursor)->prox != NULL)
		{
			Aux = NCColuna();
			Aux->letra = letra;
			if((*linha)->inicioL == *cursor)
				(*linha)->inicioL = Aux;
			Aux->prox = *cursor;
			if((*cursor)->ant != NULL)
				(*cursor)->ant->prox = Aux;
			Aux->ant = (*cursor)->ant;
			(*cursor)->ant = Aux;
			//*cursor = Aux;
		}
		else
		{
			(*cursor)->letra=letra;
			(*cursor)->prox = NCColuna();
			(*cursor)->prox->ant = *cursor;
			*cursor = (*cursor)->prox;
			//gotoxy(25,25); printf("  %d  ",(*linha)->nro);
		}
		(*linha)->nro++;
		*c=*c+1;
	}
	else
	{	
		if(letra!=' ')
		{
			if((*linha)->bot==NULL)	// Nao tem linha para baixo
			{
				Aux=(*cursor)->ant;
				while(Aux->ant!=NULL && Aux->letra!=' ')
					Aux = Aux->ant;
				Aux = Aux->prox;
				Aux->ant->prox=NULL;
				Aux->ant=NULL;
				(*linha)->bot = NCLinha();
				(*linha)->bot->top= *linha;
				*linha = (*linha)->bot;
				*cursor = (*linha)->inicioL;
				*c = 2;
				while(Aux->prox!=NULL)
				{
					InsereCarac(&*linha,&*cursor,Aux->letra,c,l);
					exclui=Aux;
					Aux=Aux->prox;
					free(exclui);
					apaga = (*linha)->top->nro;
					gotoxy(apaga,*l);printf("  ");
					(*linha)->top->nro--;
				}
				InsereCarac(&*linha,&*cursor,letra,c,l);
				free(Aux);
				if(*l<24)
					*l=*l+1;
			}
			else	// Tem linha para baixo
			{
				if((*cursor)->ant->letra != ' ')	// Remanejamento (Quebra a linha)
				{
					cont = 2;
					palavras=(*cursor)->ant;	// Ultimo elem da palavra
					while(palavras->ant!=NULL && palavras->letra!=' ')
						palavras = palavras->ant;
					if(palavras->ant!=NULL)
						palavras = palavras->prox;	// Roda ate o primerio elem da palavra
					palavras->ant->prox=NULL;
					palavras->ant=NULL;
					inserir = (*linha)->bot->inicioL;	// Zera a linha de baixo
					*linha = (*linha)->bot;
					(*linha)->inicioL = NCColuna();
					*cursor = (*linha)->inicioL;
					if(*l<24)
						*l = *l+1;
					*c = 2;
					while(palavras->prox!=NULL)
					{
						InsereCarac(&*linha,&*cursor,palavras->letra,&*c,&*l);
						exclui = palavras;
						palavras = palavras->prox;
						free(exclui);
						(*linha)->top->nro--;
						cont++;
					}
					InsereCarac(&*linha,&*cursor,letra,&*c,&*l);
					pos = *cursor;
					while(inserir->prox!=NULL)
					{
						InsereCarac(&*linha,&*cursor,inserir->letra,&*c,&*l);
						exclui = inserir;
						inserir = inserir->prox;
						free(exclui);
					}
					InsereCarac(&*linha,&*cursor,inserir->letra,&*c,&*l);
					*cursor = pos;
					*c = cont+2;
					//*l=*l+1;
				}
				else
				{
					Aux = NCColuna();
					Aux->letra = letra;
					Aux->prox = (*linha)->bot->inicioL;
					(*linha)->bot->inicioL->ant = Aux;
					(*linha)->bot->inicioL = Aux;
					*cursor = Aux->prox;
					*linha = (*linha)->bot;
					(*linha)->nro++;
					*c = 2;
					if(*l<24)
						*l=*l+1;
				}
			}
			limpaEscrita();
		}
		else
		{
			(*linha)->bot = NCLinha();
			(*linha)->bot->top= *linha;
			*linha = (*linha)->bot;
			*cursor = (*linha)->inicioL;
			*c=2;
			if(*l<24)
				*l=*l+1;
		}
	}
}

void InsereInsert(tpLinha **linha, tpColuna **cursor, char letra,int *c,int *l)
{
	int apaga;
	tpColuna *Aux,*exclui,*palavras,*inserir,*pos;
	int cont;
	
	if((*linha)->nro<79)
	{
		if((*cursor)->prox != NULL)
			(*cursor)->letra = letra;
		else
		{
			(*cursor)->letra=letra;
			(*cursor)->prox = NCColuna();
			(*cursor)->prox->ant = *cursor;
		}
		*cursor = (*cursor)->prox;
		*c=*c+1;
	}
	else
	{	
		if(letra!=' ')
		{
			if((*linha)->bot==NULL)	// Nao tem linha para baixo
			{
				Aux=(*cursor)->ant;
				while(Aux->ant!=NULL && Aux->letra!=' ')
					Aux = Aux->ant;
				Aux = Aux->prox;
				Aux->ant->prox=NULL;
				Aux->ant=NULL;
				(*linha)->bot = NCLinha();
				(*linha)->bot->top= *linha;
				*linha = (*linha)->bot;
				*cursor = (*linha)->inicioL;
				*c = 2;
				while(Aux->prox!=NULL)
				{
					InsereCarac(&*linha,&*cursor,Aux->letra,c,l);
					exclui=Aux;
					Aux=Aux->prox;
					free(exclui);
					apaga = (*linha)->top->nro;
					gotoxy(apaga,*l);printf("  ");
					(*linha)->top->nro--;
				}
				InsereCarac(&*linha,&*cursor,letra,c,l);
				free(Aux);
				if(*l<24)
					*l=*l+1;
			}
			else	// Tem linha para baixo
			{
				if((*cursor)->ant->letra != ' ')	// Remanejamento (Quebra a linha)
				{
					cont = 2;
					palavras=(*cursor)->ant;	// Ultimo elem da palavra
					while(palavras->ant!=NULL && palavras->letra!=' ')
						palavras = palavras->ant;
					if(palavras->ant!=NULL)
						palavras = palavras->prox;	// Roda ate o primerio elem da palavra
					palavras->ant->prox=NULL;
					palavras->ant=NULL;
					inserir = (*linha)->bot->inicioL;	// Zera a linha de baixo
					*linha = (*linha)->bot;
					(*linha)->inicioL = NCColuna();
					*cursor = (*linha)->inicioL;
					if(*l<24)
						*l = *l+1;
					*c = 2;
					while(palavras->prox!=NULL)
					{
						InsereCarac(&*linha,&*cursor,palavras->letra,&*c,&*l);
						exclui = palavras;
						palavras = palavras->prox;
						free(exclui);
						(*linha)->top->nro--;
						cont++;
					}
					InsereCarac(&*linha,&*cursor,letra,&*c,&*l);
					pos = *cursor;
					while(inserir->prox!=NULL)
					{
						InsereCarac(&*linha,&*cursor,inserir->letra,&*c,&*l);
						exclui = inserir;
						inserir = inserir->prox;
						free(exclui);
					}
					InsereCarac(&*linha,&*cursor,inserir->letra,&*c,&*l);
					*cursor = pos;
					*c = cont+2;
					//*l=*l+1;
				}
				else
				{
					Aux = NCColuna();
					Aux->letra = letra;
					Aux->prox = (*linha)->bot->inicioL;
					(*linha)->bot->inicioL->ant = Aux;
					(*linha)->bot->inicioL = Aux;
					*cursor = Aux->prox;
					*linha = (*linha)->bot;
					(*linha)->nro++;
					*c = 2;
					if(*l<24)
						*l=*l+1;
				}
			}
			limpaEscrita();
		}
		else
		{
			(*linha)->bot = NCLinha();
			(*linha)->bot->top= *linha;
			*linha = (*linha)->bot;
			*cursor = (*linha)->inicioL;
			*c=2;
			if(*l<24)
				*l=*l+1;
		}
	}
}

void MoveCursorVertical(char tecla,int *c,int *l,tpColuna **cursor,tpLinha **linha)
{
	int cont=2;
	tpColuna *Aux;
	if(tecla == 72)	// Seta cima
	{
		if((*linha)->top != NULL)
		{
			Aux = (*linha)->top->inicioL;
			if((*c-2) >= (*linha)->top->nro)
			{
				while(Aux->prox != NULL)
					Aux = Aux->prox;
				*cursor = Aux;
				*c = (*linha)->top->nro+2;
			}
			else
			{
				while(cont <= *c && Aux->prox != NULL)
				{
					Aux = Aux->prox;
					cont++;
				}
				*cursor = Aux;
			}
			*linha = (*linha)->top;
			*l = *l-1;
		}
	}
	else	// Seta baixo
	{
		if((*linha)->bot != NULL)
		{
			Aux = (*linha)->bot->inicioL;
			if((*c-2) >= (*linha)->bot->nro)
			{
				while(Aux->prox != NULL)
					Aux = Aux->prox;
				*cursor = Aux;
				*c = (*linha)->bot->nro+2;
			}
			else
			{
				while(cont <= *c && Aux->prox != NULL)
				{
					Aux = Aux->prox;
					cont++;
				}
				*cursor = Aux;
			}
			*linha = (*linha)->bot;
			if(*l<24)
				*l = *l+1;
		}
	}
}

void MoveCursorHorizontal(char tecla,int *c,int *l,tpColuna **cursor,tpLinha **linha)
{
	tpColuna *Aux;
	if(tecla == 75) // Seta esquerda
	{
		if((*cursor)->ant==NULL && (*linha)->top!=NULL)	//Primeiro elem da lista
		{
			Aux = (*linha)->top->inicioL;
			while(Aux->prox!=NULL)
				Aux= Aux->prox;
			*cursor=Aux;
			*l = *l-1;
			*c = (*linha)->top->nro+2;
			*linha = (*linha)->top;
		}
		else if((*cursor)->ant!=NULL)
		{
			*cursor = (*cursor)->ant;
			*c = *c-1;
		}
	}
	else	// Seta direita
	{
		if((*cursor)->prox==NULL && (*linha)->bot!=NULL)	//Ultimo elem da lista
		{
			(*cursor)=(*linha)->bot->inicioL;
			if(*l<24)
				*l = *l+1;
			*c = 2;
			*linha = (*linha)->bot;
		}
		else if((*cursor)->prox != NULL)
		{
			*cursor=(*cursor)->prox;
			*c = *c+1;
		}
	}
}

void pageUp(tpLinha *inicio ,tpLinha **linha, tpColuna **cursor, int *c, int *l)
{
	
	*linha = inicio;
	*cursor = (*linha)->inicioL;
	*c =2;
	*l = 4;
}

void pageDown(tpLinha **linha, tpColuna **cursor, int *c, int *l)
{
	tpLinha *aux;
	aux = *linha;
	while(aux->top != NULL)
		aux = aux->top;
	while(aux->bot != NULL && *l < 23)
	{
		aux = aux->bot;
		*l=*l+1;
	}	
	*cursor = aux->inicioL;
	*c =2;
}

void BackSpace(tpLinha **linha, tpColuna **cursor, int *c, int *l)
{
	tpColuna *aux = *cursor;
	tpLinha *auxLinha = *linha;
	if(aux->ant == NULL)	// Cursor primeira letra
	{
		if((*linha)->top != NULL)	// Nao primeira LINHA
		{
			*linha = (*linha)->top;
			*cursor = (*linha)->inicioL;
			while((*cursor)->prox != NULL)
				*cursor = (*cursor)->prox;
			if(aux->prox == NULL)	// Cursor era o sentinela
			{
				(*linha)->bot = auxLinha->bot;
				if(auxLinha->bot != NULL)	// Tem linha para baixo
					auxLinha->bot->top = *linha;
				free(aux);
				free(auxLinha);
			}
			*l = *l-1;
			*c = (*linha)->nro+2;
		}
	}
	else
	{
		if(aux->ant->ant == NULL)	// Cursor segunda letra
		{
			aux = aux->ant;
			auxLinha->inicioL = aux->prox;
			aux->prox->ant = NULL;
		}
		else
		{
			aux = aux->ant;
			aux->ant->prox = aux->prox;
			aux->prox->ant = aux->ant;
			
		}
		free(aux);
		(*linha)->nro = (*linha)->nro-1;
		*c = *c-1;
	}
}

void Del(tpLinha **linha, tpColuna **cursor, int *c, int *l)
{
	tpColuna *aux = *cursor;
	tpLinha *auxLinha = *linha;
	if((*cursor)->prox != NULL)	// Cursor nao for o sentinela
	{
		*cursor = (*cursor)->prox;
		if(aux->ant == NULL)	// Cursor estava na primeira letra
		{
			(*linha)->inicioL = *cursor;
			(*cursor)->ant = NULL;
		}
		else	// Cursor nao estava na primeira letra
		{
			aux->ant->prox = aux->prox;
			aux->prox->ant = aux->ant;
		}
		free(aux);
		(*linha)->nro = (*linha)->nro-1;
	}
	else if((*cursor)->ant == NULL)	// Cursor for o sentinela && ter apenas o sentinela na linha
	{
		if((*linha)->top != NULL)	// Nao primeira linha
		{
			*linha = (*linha)->top;
			*cursor = (*linha)->inicioL;
			while((*cursor)->prox != NULL)
				*cursor = (*cursor)->prox;
			(*linha)->bot = auxLinha->bot;
			if(auxLinha->bot != NULL)	// Tem linha para baixo
				auxLinha->bot->top = *linha;
			free(aux);
			free(auxLinha);
			*l = *l-1;
			*c = (*linha)->nro+2;
		}
	}
}

void Enter(tpLinha **linha, tpColuna **cursor, int *c, int *l)
{
	tpLinha *auxL = *linha, *nl = NCLinha();
	tpColuna *auxC = *cursor, *exc = *cursor;
	nl->bot = auxL->bot;
	nl->top = auxL;
	if(auxL->bot != NULL)
		auxL->bot->top = nl;
	auxL->bot = nl;
	*linha = (*linha)->bot;
	*cursor = (*linha)->inicioL;
	*c = 2;
	if(*l<24)
		*l = *l+1;
	if(auxC->prox != NULL)
	{
		while(auxC->prox != NULL)
		{
			InsereCarac(&*linha,&*cursor,auxC->letra,&*c,&*l);
			auxC = auxC->prox;
		}
		auxC=exc;
		while(auxC->prox != NULL)
		{
			auxC->prox->ant = auxC->ant;
			auxC = auxC->prox;
			if(exc->ant == NULL)
				auxL->inicioL = auxC;	
			else
				exc->ant->prox = exc->prox;
			free(exc);
			auxL->nro = auxL->nro-1;
			exc=auxC;
		}	
	}
}

void Abrir(tpLinha **linha, tpColuna **cursor, int *c, int *l)
{
	char carac;
	FILE *ptr = fopen("arquivo.txt","r");
	if(ptr != NULL)
	{
		while(!feof(ptr))
		{
			fscanf(ptr,"%c",&carac);
			InsereCarac(&*linha, &*cursor,carac,c,l);
		}
	}
	else
		printf("\nERRO DE ABERTURA!");
	fclose(ptr);
}

void Salvar(tpLinha *L)
{
	tpLinha *aux=L;
	tpColuna *auxL;
	int i,j;
	char NomeArq[100];
	for(int i=40;i<80;i++)	//60 metade --- 20 pra ca 20 pra la
	{
		gotoxy(i,9);printf("%c",205);
		gotoxy(i,19);printf("%c",205);
	}
	for(int i=9;i<19;i++)
	{
		gotoxy(40,i);printf("%c",186);
		gotoxy(80,i);printf("%c",186);
	}
	gotoxy(40,9);printf("%c",201);
	gotoxy(80,9);printf("%c",187);
	gotoxy(80,19);printf("%c",188);
	gotoxy(40,19);printf("%c",200);
	gotoxy(48,11);printf("Digite o nome do arquivo: ");
	gotoxy(53,13);
	fflush(stdin);
	gets(NomeArq);
	strcat(NomeArq,".txt");
	FILE *salve = fopen(NomeArq,"w");
	while(aux!=NULL)
	{
		auxL = aux->inicioL;
		while(auxL->prox!=NULL)
		{
			fprintf(salve,"%c",auxL->letra);
			auxL = auxL->prox;
		}
		fprintf(salve,"\n");
		aux = aux->bot;
	}
	gotoxy(46,17);printf("Arquivo salvo com sucesso!");
	fclose(salve);
}

void exibeBonitinho(tpLinha *L)
{
    int Pri_Lin=15, Rec_Dir=7, Rec_Esq=5, TL=0, i=0, lin,col, ini=1, neg=0, pos;
    char aux[100],vetP[500][30];
    tpLinha *auxLin = L;
    tpColuna *auxCursor; 
    system("cls");
    while(auxLin!=NULL)
    {
        auxCursor = auxLin->inicioL;
        i=0;
        while(auxCursor!=NULL)
        {
            if(auxCursor->letra!=' ')
                aux[i++] = auxCursor->letra;
            else
            {
                aux[i] = '\0';
                strcpy(vetP[TL++],aux);
                i=0;
                aux[i] = '\0';
            }
            auxCursor = auxCursor->prox;
        }
        aux[i] = '\0';
        if(aux[0]!='\0')
            strcpy(vetP[TL++],aux);
        auxLin = auxLin->bot;
	}
    system("cls");
    i=0;
    lin=1;
    col=1;
    neg=0;
    textcolor(7); 
    while(i<TL)
    {
        if(ini==1)
        {
            lin++;
            ini=0;
            vetP[i][0] = toupper(vetP[i][0]);
            col = Pri_Lin+1;
            
        }
        else
        {
            if(col+strlen(vetP[i])>81-Rec_Dir)
            {
                lin++;
                col=Rec_Esq+1;
            }            
        }
        for(int j=0; j<strlen(vetP[i]); j++)
        {
            if(vetP[i][j] == 21)
            {
                if(neg==0)
                {
                    neg=1;
                    textcolor(3);
                }
                else
                {
                    neg=0;
                    textcolor(7);
                }      
            }
            else
            {
                gotoxy(col,lin);
                printf("%c ",vetP[i][j]);
                col++;
            }
        }
        col++;
        if(vetP[i][strlen(vetP[i])-1]=='.' || vetP[i][strlen(vetP[i])-1]=='!' || vetP[i][strlen(vetP[i])-1]=='?')
            ini=1;
        i++;
    }
    
    getch();
}

void BuscaPalavra(tpListaGen *Palavras,char palavra[50],int i,char Indicar[50])
{
	tpListaGen *aux = Palavras;
	if(aux!=NULL)
	{
		if(i<strlen(palavra))
		{
			while(aux!=NULL && aux->letra<palavra[i])
				aux = aux->prox;
			if(aux==NULL)
				Indicar[0] = '\0';
			else
			{
				if(aux->letra!=palavra[i])
					Indicar[0] = '\0';
				else
				{
					Indicar[i] = aux->letra;
					Indicar[i+1] = '\0';
					if(aux->logico == 'F')
						BuscaPalavra(aux->prim, palavra, i+1, Indicar);
				}
			}
		}
		else
		{
			Indicar[i] = aux->letra;
			Indicar[i+1] = '\0';
			if(aux->logico == 'F')
				BuscaPalavra(aux->prim, palavra, i+1, Indicar);
		}		
	}
}

void IndicaPalavra(tpListaGen *LG, char palavra[50],char palavraI[50])
{
	palavraI[0] = '\0';
	BuscaPalavra(LG,palavra,0,palavraI);
	gotoxy(60,27); printf("Sugestao:           %s        ",palavraI);
}

void InserePalavra(tpListaGen **Palavras, char texto[50],int i)
{
    tpListaGen *Nova, *aux, *ant;
    if(texto[i] != '\0')
    {
        Nova = (tpListaGen*)malloc(sizeof(tpListaGen));
        Nova->prim = NULL;
        Nova->prox = NULL;
        Nova->letra = texto[i];
        if(texto[i+1]!='\0')
            Nova->logico = 'F';
        else
            Nova->logico = 'T';
        if(*Palavras == NULL)
        {
            *Palavras = Nova;
            InserePalavra(&Nova->prim,texto,i+1);
        }      
        else
        {
            aux = *Palavras;
            if(texto[i] < aux->letra)
            {
                Nova->prox = aux;
                *Palavras = Nova;
                InserePalavra(&Nova->prim,texto,i+1);
            }
            else
            {
                while(aux!=NULL && texto[i]>aux->letra)
                {
                    ant = aux;
                    aux = aux->prox;
                }
                if(aux==NULL)
                {
                    ant->prox = Nova;
                    InserePalavra(&Nova->prim,texto,i+1);
                }
                else
                {
                    if(texto[i] == aux->letra)
                    {
                        if(Nova->logico == 'F')
                            InserePalavra(&aux->prim,texto,i+1);
                        else
                            aux->logico = 'T';
                    }
                        
                    else
                    {
                        Nova->prox = aux;
                        ant->prox = Nova;
                        InserePalavra(&Nova->prim, texto, i+1);
                    }
                }                
            }            
        }
    }
}

void CompletaPalavra(int *l, int*c,tpLinha **linha, tpColuna **cursor, tpListaGen *LG,char palavras[50],char palavraI[50])
{
	int i = strlen(palavras);
	while(i<strlen(palavraI))
	{
		InsereCarac(&*linha,&*cursor,palavraI[i],c,l);
		i++;
	}
}

void PegaPalavra(char palavra[50], tpColuna *cursor,int TL)
{
	palavra[0]='\0';
	tpColuna *aux = cursor->ant->ant;
	while(aux->ant!=NULL && aux->letra != ' ')
		aux = aux->ant;
	if(aux->ant != NULL)
		aux = aux->prox;
	while(aux!=cursor->ant->ant)
	{
		palavra[TL] = aux->letra;
		TL++;
		aux = aux->prox;
	}
	palavra[TL++] = aux->letra;
	palavra[TL]='\0';
}

int main()
{
	tpListaGen *LG = NULL; 
	tpLinha *L,*linha,*pLinhaT,*inicioL,*FinalL,*aux;
	tpColuna *cursor;
	inicioL = (tpLinha*)malloc(sizeof(tpLinha));
	FinalL = (tpLinha*)malloc(sizeof(tpLinha));
	char tecla, palavra[50],palavraI[50];
	int c=2,l=4,f4=0,negrito=0,i,insert=0,TL=0,qtd=0;
	L = NCLinha();
	linha = L;
	pLinhaT = linha;
	cursor = linha->inicioL;
	DesenhoTela();
	gotoxy(35,27); printf("INSERT    ");
	gotoxy(c,l);
	while(f4!=1)
	{
		tecla = getch();
		if (tecla == -32)
		{
	    	tecla = getch();
	   		switch(tecla)
			{
				case 75:	MoveCursorHorizontal(tecla,&c,&l,&cursor,&linha);	//Seta para esquerda
		            break;
		        case 72:	MoveCursorVertical(tecla,&c,&l,&cursor,&linha);	//Seta para cima
		            break;
		        case 77: 	MoveCursorHorizontal(tecla,&c,&l,&cursor,&linha);	//Seta para direita
		            break;
		        case 80: 	MoveCursorVertical(tecla,&c,&l,&cursor,&linha);	//Seta para baixo
		            break;
				case 71:	cursor = linha->inicioL; c=2;	// HOME	
		            break;
		    	case 79:	cursor = linha->inicioL;	// END
		    				i=0;
							while(cursor->prox != NULL)
							{
								cursor = cursor->prox;
								i++;
							}
							c=i+2;
		            break;
		        case 73:	pageUp(inicioL, &linha, &cursor, &c, &l);// PAGE UP
		            break;
		        case 81:	pageDown(&linha, &cursor, &c, &l);// PAGE DOWN
		            break;
		        case 83:	Del(&linha, &cursor, &c, &l);	// DEL
							limpaEscrita();
		            break;
		        case 82:	if(insert==0)// INSERT LIGA
					        {
					        	insert=1;
					        	gotoxy(35,27); printf("INSERT ||| ");
							}
		        			else
		        			{
		        				insert=0;
		        				gotoxy(35,27); printf("INSERT    ");
							}
		            break;
	    	}
		}
		else if (tecla == NULL)
		{
		    tecla = getch();
		    switch(tecla)
			{
		        case 60: 	if(L->nro > 0)	// F2 ABRIR
								DestroiLista(&L);
							L = NCLinha();
							linha = L;
							cursor = linha->inicioL;
							c=2;
							l=4;
							Abrir(&linha, &cursor,&c,&l);
		        			
		            break;
		        case 61: 	clrscr();	// F3 SALVAR 
		        			DesenhoTela();
		        			Salvar(L);
		        			getch();
		        			clrscr(); 
		        			DesenhoTela();
		            break;
				case 62: 	f4=1;	// F4 SAIR
							clrscr();
							gotoxy(55,14);
							textcolor(1);
							printf("Saindo...");
							_sleep(3000);
							clrscr();
		            break;
		        case 63:	exibeBonitinho(L);// F5 EXIBIR
		        			getch();
		        			DesenhoTela();
		        			limpaEscrita();
		            break;
		        case 68:	cursor->letra=21;	// F10 NEGRITO
		        			cursor->prox =NCColuna();
		        			cursor->prox->ant=cursor;
		        			cursor = cursor->prox;
		            break;
		    }
		}
		else if(tecla == 8 || tecla == 13 || tecla == 9)
		{
		    switch(tecla)	// As opicoes abaixo possuem valor na tabela ASCII.
			{
		        case 8:		BackSpace(&linha, &cursor, &c, &l);	// BACKSPACE
							limpaEscrita();
		            break;
		        case 9: 	CompletaPalavra(&l, &c, &linha, &cursor,LG,palavra,palavraI);//TAB
					break;
		        case 13: 	Enter(&linha, &cursor, &c, &l);	// ENTER
		        			limpaEscrita();
		            break;
			}
		}
		else
		{
			if(insert == 0)
				InsereCarac(&linha,&cursor,tecla,&c,&l);
			else
				InsereInsert(&linha,&cursor,tecla,&c,&l);
		}
		PegaPalavra(palavra,cursor,0);	
		if(tecla == ' ' || tecla == '.' || tecla == '?' || tecla == '!' || tecla == ',')
		{
			PegaPalavra(palavra,cursor,0);
			InserePalavra(&LG,palavra,0);
			palavra[0] = '\0';
			TL = 0;
		}
		ExibeFila(L,&inicioL,&FinalL);
		IndicaPalavra(LG,palavra,palavraI);
		aux = L;
		qtd=0;
		while(aux->bot!=NULL)
		{
			qtd++;
			aux = aux->bot;
		}
		gotoxy(6,27); printf("LIN:   %d    ",qtd);
		gotoxy(18,27); printf("COL:  %d  ",linha->nro);
		gotoxy(c,l); 
	}
	
	return 1;
}
