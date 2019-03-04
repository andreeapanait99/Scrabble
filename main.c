#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int n,nr=0;
struct jeton
{
    char litera;
    int val;
} jetoane[30];
struct patrat
{
    int coef_mult_lit;
    int coef_mult_cuv;
    struct jeton l;
    int ocupat;
} v[27][27];
int compara(const void *a,const void *b)
{
    struct jeton *pa=(struct jeton*) a;
    struct jeton *pb=(struct jeton*) b;
    if (pa->litera<pb->litera) return -1;
    if (pa->litera>pb->litera) return 1;
    if (pa->litera==pb->litera) return 0;
}
void incarcaJetoane()
{
    int i;
    FILE *f=fopen("jetoane.txt","r");
    while (!feof(f))
    {
        fscanf(f,"%c %d\n",&jetoane[nr].litera,&jetoane[nr].val);
        nr++;
    }
    fclose(f);
    qsort(jetoane,nr,sizeof(struct jeton),compara);
    for (i=0; i<nr; i++) printf("%c %d\n",jetoane[i].litera,jetoane[i].val);
}
void incarcaTablaJoc()
{
    FILE *f=fopen("tablaJoc.txt","r");
    int i,j,nr;
    char culoare[15],litera;
    fscanf(f,"%d",&n);
    fgetc(f);
    for (i=1; i<=n; i++)
        for (j=1; j<=n; j++)
        {
            v[i][j].coef_mult_lit=1;
            v[i][j].coef_mult_cuv=1;
            v[i][j].ocupat=0;
        }
    while (!feof(f))
    {
        fscanf(f,"%s",&culoare);
        fscanf(f," %c%d",&litera,&nr);
        int l=litera-'a'+1;
        if (strcmp(culoare,"bleu")==0) v[l][nr-1].coef_mult_lit=2;
        else if (strcmp(culoare,"albastru")==0) v[l][nr-1].coef_mult_lit=3;
        else if (strcmp(culoare,"roz")==0) v[l][nr-1].coef_mult_cuv=2;
        else if (strcmp(culoare,"rosu")==0) v[l][nr-1].coef_mult_cuv=3;
    }
    fclose(f);
}
void adaugaCuvant(char *cuvant,int i,int j,int directie)
{
    int k,a;
    for (k=0; k<strlen(cuvant); k++)
    {
        v[i][j].l.litera=cuvant[k];
        v[i][j].l.val=jetoane[cuvant[k]-'a'].val;
        v[i][j].ocupat=1;
        if (directie==0) j++;
        else i++;
    }
}
int calculScor(int i,int j,int lung_cuv,int directie)
{
    int scor=0,parcurs=0;
    while (parcurs<lung_cuv)
    {
         if (v[i][j].ocupat!=1) scor+=v[i][j].l.val*v[i][j].coef_mult_lit;
        parcurs++;
        if (directie==0) j++;
        else i++;
    }
    parcurs=0;
    while (parcurs<lung_cuv)
    {
         if (v[i][j].ocupat!=1) scor*=v[i][j].coef_mult_cuv;
        parcurs++;
        if (directie==0) j++;
        else i++;
    }
    if (lung_cuv>=7) scor+=50;
    return scor;
}
int main()
{
    incarcaJetoane();
    incarcaTablaJoc();
    FILE *f=fopen("joc.txt","r");
    char cuvant[100],linie[100];
    int jucator,directie,nrpct1=0,nrpct2=0,i,j;
    while (fgets(linie,100,f))
    {
        strtok(linie,"\n");
        jucator=linie[0]-'0';
        char *p=strchr(linie,' ');
        if (isalpha(p[1])) directie=0;
        else directie=1;
        if (isalpha(p[1]))
        {
            i=p[1]-'a'+1;
            j=p[2]-'0';
            if (isdigit(p[3])) j=j*10+(p[3]-'0');
        }
        else
        {
            i=p[1]-'0';
            if (isdigit(p[2]))
            {
                i=i*10+(p[2]-'0');
                j=p[3]-'a'+1;
            }
            else j=p[2]-'a'+1;
        }
        p=p+1;
        p=strchr(p,' ');
        strcpy(cuvant,p+1);
        if (directie==0) adaugaCuvant(cuvant,i,j,directie);
        else adaugaCuvant(cuvant,j,i,directie);
        if (jucator==1) nrpct1+=calculScor(i,j,strlen(cuvant),directie);
        else nrpct2+=calculScor(i,j,strlen(cuvant),directie);
    }
    for (i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++) printf("%c ",v[i][j].l.litera);
        printf("\n");
    }
    if (nrpct1>nrpct2) printf("Jucatorul 1 a castigat jocul cu %d puncte",nrpct1);
    else printf("Jucatorul 2 a castigat jocul cu %d puncte",nrpct2);
    fclose(f);
    return 0;
}
