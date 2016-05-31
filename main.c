
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct CELL
{
    char *anahtar;
    struct CELL *next;
};
//arama
int lookup(char *anahtar,struct CELL *l)// l liste basi
{
    if(l==NULL) return 0;
    else if(!strcmp(anahtar,l->anahtar)) return l;
    else return lookup(anahtar,l->next);
}

int insert(char *anahtar,struct CELL **l)
{
    if(*l==NULL)
    {
        *l=(struct CELL*)malloc(sizeof(struct CELL));
        (*l)->anahtar=(char*)malloc((strlen(anahtar)+1)*sizeof(char)); // istanbul 9 byte yer ayyryrç.
        strcpy((*l)->anahtar,anahtar);
        (*l)->next=NULL;
        return 1;
    }
    else if(strcmp(anahtar,(*l)->anahtar)) return insert(anahtar,&((*l)->next)); // esit degilse
    else return 0; //listede var eklemez
}

void print_list(struct CELL *l)
{
    if(l!=NULL)
    {
        printf("%s ",l->anahtar);
        print_list(l->next);
    }
    /*
    while(l!=NULL)
    {
    printf("%s",l->anahtar);
    l=l->next;
    }
    */
}

//defterdeki tabloyu tanymla

struct table_node
{
 int counter;
 struct CELL *header; //listenin baslangyc adrsi;
};

// h tablosu

struct hash_tablosu
{
    struct table_node *tablo_basi;
    int tablo_uzunlugu;
    int multiplier;
};

unsigned hash(char *anahtar,int multiplier,int table_size)
{
    int i=0;
    unsigned int value=0;
    while(anahtar[i]) //anahtar[i]!=NULL;
    {
        value=(anahtar[i]+multiplier*value)%table_size;
        i++;
    }
    return value;
}


void initialize_hash_table(struct hash_tablosu **hash_table,int multiplier,int table_size)
{
    int i;
    *hash_table=(struct hash_tablosu*)malloc(sizeof(struct hash_tablosu));
    if(*hash_table==NULL){
        printf("hash tablosu icin gerkli? yer ayrilmadi...");
        exit(1);
    }
    (*hash_table)->tablo_basi=(struct table_node*)malloc(table_size*sizeof(struct table_node));
    if((*hash_table)->tablo_basi==NULL)
    {
        printf("hash tablosu icin gerkli? yer ayrilmadi...");
        exit(1);
    }
    (*hash_table)->tablo_uzunlugu=table_size;
    (*hash_table)->multiplier=multiplier;

    for(i=0;i<table_size;i++)
    {
        (((*hash_table)->tablo_basi)+i)->counter=0;
        (((*hash_table)->tablo_basi)+i)->header=NULL;
    }
}

void insert_hash_table(struct hash_tablosu *hash_table,char *anahtar)
{
    int hash_index = hash(anahtar,hash_table->multiplier,hash_table->tablo_uzunlugu);
    if(insert(anahtar,&((hash_table->tablo_basi + hash_index)->header)))
    (hash_table->tablo_basi + hash_index)->counter++;
}

void print_hash_table(struct hash_tablosu *hash_table)
{
    if(hash_table) //nul degilse
    {
        int index;
        printf("----------HASH TABLOSU-------\n");
        for(index=0;index<hash_table->tablo_uzunlugu;index++)
        {
            printf("%5d : (%2d) ",index,(hash_table->tablo_basi+index)->counter);
            print_list((hash_table->tablo_basi+index)->header);
            printf("\n");
        }
    }
    else printf("Hash tablosu bos...\n");
}




int delete_dugum_liste(struct CELL **header,char *anahtar){
        struct CELL *simdiki,*onceki;
        simdiki=*header;
        while(simdiki && strcmp(simdiki->anahtar,anahtar)){
            onceki=simdiki;
            simdiki=simdiki->next;

        }
        if(!simdiki)return 0;
        if(simdiki==*header){
            *header=(*header)->next;
        }
        else{
            onceki->next=simdiki->next;
        }
        free(simdiki->anahtar);
        free(simdiki);
        return 1;
}

void delete_hash_table(struct hash_tablosu *table,char *anahtar){

    int hash_index=hash(anahtar,table->multiplier,table->tablo_uzunlugu);
    if(delete_dugum_liste(&((table->tablo_basi +hash_index)->header),anahtar))
    (table->tablo_basi+hash_index)->counter--;
}

void liste_yok_et(struct CELL **liste_basi){
    struct CELL *onceki;
    while(*liste_basi){
        onceki=*liste_basi;
        *liste_basi=(*liste_basi)->next;
        free(onceki->anahtar);
        free(onceki);
    }
}

void hash_table_yok_et(struct hash_tablosu **hash_table){
    int index;
    if(*hash_table){
        for(index=0;index<(*hash_table)->tablo_uzunlugu;index++)
            liste_yok_et(&((*hash_table)->tablo_basi+index)->header);

        free((*hash_table)->tablo_basi);
        free(*hash_table);
    }
    *hash_table=NULL;
}
struct hash_tablosu* hash_table_buyut(struct hash_tablosu **htable,int multiplier,int tablo_uzunlugu){
       int i;
       struct CELL *liste_basi;
       struct hash_tablosu *yeni_tablo;
       if(!*htable) return NULL;
        initialize_hash_table(&yeni_tablo,multiplier,tablo_uzunlugu);
        for(i=0;i<(*htable)->tablo_uzunlugu;i++){
           liste_basi=((*htable)->tablo_basi+i)->header;
            while(liste_basi!=NULL){
             insert_hash_table(&yeni_tablo,liste_basi->anahtar);
              liste_basi=liste_basi->next;
                 }
        }
        hash_table_yok_et(htable);
        return yeni_tablo;

       }


int main()
{

struct hash_tablosu *htable=NULL;
initialize_hash_table(&htable,7,11);
print_hash_table(htable);
insert_hash_table(htable,"kadayif");
insert_hash_table(htable,"trabzonspor");
insert_hash_table(htable,"besiktas");
insert_hash_table(htable,"baklava");
insert_hash_table(htable,"dembaba");
insert_hash_table(htable,"bilic");
print_hash_table(htable);
delete_hash_table(htable,"baklava");
print_hash_table(htable);
hash_table_yok_et(&htable);


print_hash_table(htable);
hash_table_buyut(&htable,17,19);
print_hash_table(htable);


system("PAUSE");
  return 0;

}
