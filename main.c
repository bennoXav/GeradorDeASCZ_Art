// Autores: Igor Vicente & João Benno Weber

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct{
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct{
    int width, height;
    RGB *img;
} Img;

// Protótipos
void load(char *name, Img *pic);

// Carrega uma imagem para a struct Img
void load(char *name, Img *pic){
    int chan;
    pic->img = (unsigned char *)SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if (!pic->img){
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char **argv)
{
    Img pic;

    if (argc == 1){
        printf("loader [img]\n");
        exit(1);
    }

    load(argv[1], &pic);

    // Transforma a imagem em preto e branco
    for (int i = 0; i < pic.width * pic.height; i++){
        int p = (0.3 * pic.img[i].r + 0.59 * pic.img[i].g + 0.11 * pic.img[i].b);
        pic.img[i].r = p;
        pic.img[i].g = p;
        pic.img[i].b = p;
    }

    int reduction = 50.0;

    int mRWidht = (pic.width * (1 - (reduction / 100.0)));
    if(mRWidht%2==1){mRWidht--;}  // deixando as dimensoes pares

    int mRHeight = (pic.height * (1 - (reduction / 100.0)));
    if(mRHeight%2==1){mRHeight--;} // deixando as dimensoes pares
    
    int mReduce = (pic.width * pic.height) * (1 - (reduction / 100.0));
    int posReduction = (pic.width * pic.height) / mReduce;

    int *mR = malloc((mRWidht * pic.height)*sizeof(int));           // int mR[mRWidht * pic.height]; //matriz reduzida
    int *mRC = malloc((mRWidht * mRHeight)*sizeof(int));            // int mRC[mRWidht * mRHeight]; //matriz reduzida
    int *mR5 = malloc(((mRWidht/2) * (mRHeight/4))*sizeof(int));    // int mR5[(mRWidht/2) * (mRHeight/4)];

    int gray = 0;   // cinza medio
    int pos = 0;

    //redução de 50% do tamanho da imagem na largura
    for (int j = 0; j < (pic.width * pic.height); j += pic.width){ // indo de linha em linha de pixels da imagem
        
        for (int i = 0; i < pic.width; i+=posReduction){           // percorrendo cada linha da imagem ao passo de posReduction

            gray = 0;

            for (int o = 0; o < posReduction; o++){                // pixel atual e proximo, de acordo com o posReduction
                
                gray += pic.img[j+i+o].r;

            }

            *(mR + pos++) = gray/posReduction;                       // media do cinza

        }
    
    }

    gray = 0;
    pos = 0;

    //redução de 50% do tamanho da imagem no comprimento
    for (int j = 0; j < (mRWidht * pic.height); j += posReduction*mRWidht){

        for (int i = 0; i < mRWidht; i++){

            gray = 0;

            for (int o = 0; o < posReduction; o++){

                gray += *(mR+(j + i + o*mRWidht ));

            }
            
            *(mRC + pos++) = gray/posReduction;

        }
        
    }

    // inicio redução 4x2

    gray = 0;
    pos = 0;
    
    for (int j = 0; j < (mRWidht * mRHeight); j += 4*mRWidht){ // de 4 em 4 linhas

        for (int i = 0; i < mRWidht; i+=2){   // de 2 em 2 pixels na linha

            gray = 0;

            gray += *(mRC+(j   + i ));
            gray += *(mRC+(j+1 + i ));
            gray += *(mRC+(j+2 + i ));
            gray += *(mRC+(j+3 + i ));

            gray += *(mRC+(j   + i+1 ));
            gray += *(mRC+(j+1 + i+1 ));
            gray += *(mRC+(j+2 + i+1 ));
            gray += *(mRC+(j+3 + i+1 ));

            *(mR5 + pos++) = gray/8;  // media do cinza do bloco 4x2

        }

    }
    
    printf("\nFim !");

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, (const unsigned char *)pic.img);

    // Gravando um arquivo saida.html
    FILE *arq = fopen("saida.html", "w"); // criar o arquivo: w

    if (arq == NULL){
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    fprintf(arq, "<html>\n");
    fprintf(arq, "<head>\n");
    fprintf(arq, "</head>\n");
    fprintf(arq, "<body style='background: white; margin-left=0;margin-top=0'>\n");
    fprintf(arq, "<pre>\n");

    int troca;

    for (int i = 0; i < ((mRHeight/4) * (mRWidht/2)); i++){

        troca = mR5[i];
        // Escolha de caracteres: . : c o C O 8 @
        if (troca >= 0 && troca <= 31){fprintf(arq, "@");}
        else if (troca >=  32 && troca <=  63){fprintf(arq, "8");}
        else if (troca >=  64 && troca <=  95){fprintf(arq, "O");}
        else if (troca >=  96 && troca <= 127){fprintf(arq, "C");}
        else if (troca >= 128 && troca <= 159){fprintf(arq, "o");}
        else if (troca >= 160 && troca <= 191){fprintf(arq, "c");}
        else if (troca >= 192 && troca <= 223){fprintf(arq, ":");}
        else if (troca >= 224 && troca <= 255){fprintf(arq, ".");}

        if (i % (mRWidht/2) == 0 && i!=0){
            fprintf(arq, "<br>\n");
        }

    }

    fprintf(arq, "</pre>\n");
    fprintf(arq, "<style>\n");
    fprintf(arq, "pre{color: black;font-family: Courier;font-size: 2px;}\n");
    fprintf(arq, "</style>\n");
    fprintf(arq, "</body>\n");
    fprintf(arq, "</html>\n");
    fclose(arq);
    free(pic.img);
    free(mR);
    free(mRC);
    free(mR5);

}
