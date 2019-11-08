#include "chapter1/bmp.h"
#include "chapter2/effect.h"
#include "chapter1/image.h"
int main(int ac,char *av[])
{
    ImageData *img,*outimg;
    int res;
    int x,y,mx,my;

    if(ac<4) {
        printf("パラメータが足りません");
        return 0 ;
    }

    

    // ファイルより画像データの読み込み
    res=readBMPfile(av[1],&img);
    if(res<0) {
        printf("画像が読めません");
        return 0;
    }

    // 結果格納用画像データ作成
    outimg=createImage(img->width,img->height,24);

    effect1(img,outimg,atoi(av[3]),atoi(av[4]));

    writeBMPfile(av[2],outimg);
    disposeImage(img);
    disposeImage(outimg);
}