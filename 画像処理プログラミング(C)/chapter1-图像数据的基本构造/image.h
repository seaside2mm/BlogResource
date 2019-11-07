#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
typedef unsigned char BYTE;

//图像数据格式，以数组形式
typedef	struct STRUCT_IMAGE{
	int width,height;
	int depth;
	void* pixels;  //图像数据指针
}ImageData;

#define PIXELMAX 255
typedef struct STRUCT_PIXEL{
	int r,g,b;
}Pixel;

/* List1-2
  画像データ作成
   width :画像の横幅
   height:画像の縦幅
   depth :１画素あたりのビット数(8 or 24)
 */
ImageData* createImage(int width,int height,int depth)
{
    ImageData *newimg;
    int byte_per_pixel;

    if(width<0 || height<0) return NULL;
    if(depth!=8 && depth!=24) return NULL;	// １画素あたりのビット数(8,24以外はエラー)

    //分配堆空间
    newimg=malloc(sizeof(ImageData));

    if(newimg==NULL) return NULL;

    // 1画素格納するのに必要なバイト数を求める
    // 一个像素点需要的byte数目。8bit = 1byte
    //如灰度图1个像素1位，rgb需要3位。
    byte_per_pixel=depth/8;

    // 画像データを格納するのに必要なメモリを確保，分配图像需要空间
    newimg->pixels=malloc(sizeof(BYTE)*byte_per_pixel*width*height);
    if(newimg->pixels==NULL) {
        free(newimg);
        return NULL;
    }
    // 各プロパティ値を設定
    newimg->width=width;
    newimg->height=height;
    newimg->depth=depth;
    return newimg;
}

/* List1-3
   画像データの廃棄
 */
void disposeImage(ImageData *img)
{
    if(img->pixels!=NULL) free(img->pixels);
    free(img);
    return;
}

/* List1-4
  画像データ上の画素値を取得
  x,y 画素の座標
  pix 画素値を格納する
 */
int getPixel(ImageData *img,int x,int y,Pixel *pix)
{
    int ret=1;
    int adr;  // 画素の画像上の位置
    int dep,val;
    BYTE *pixels;

    if(img==NULL) return -1;
    if(img->pixels==NULL) return -1;

    // 画像外の座標が指定された場合の処理（最も近い画像上の画素を参照する）
    if(x<0) {
        x=0;
        ret=0;
    }
    if(x >= img->width ) {
        x=img->width -1;
        ret=0;
    }
    if(y<0) {
        y=0;
        ret=0;
    }
    if(y >= img->height ) {
        y=img->height -1;
        ret=0;
    }

    dep=img->depth;
    adr=x + y*img->width;
    pixels=img->pixels;

    if(dep==8) {  // グレースケールの場合は、RGBすべての同じ値をセットする
        val=pixels[adr];
        pix->r=val;
        pix->g=val;
        pix->b=val;
    }
    else if(dep==24) {
        pixels+=(adr*3);
        pix->r=(*pixels);
        pixels++;
        pix->g=(*pixels);
        pixels++;
        pix->b=(*pixels);
    }
    else {
        return -1;
    }
    return ret;
}

/*
  画素値の補正（範囲外の値を範囲内に収める）
*/
int correctValue(int val,int max)
{
    if(val<0) return 0;
    if(val>max) return max;
    return val;
}

/* List1-5
  画像データ上の画素値を変更する
  x,y 画素の座標
  pix セットする画素値
 */
int setPixel(ImageData *img,int x,int y,Pixel *pix)
{
    int adr;	// 画素の画像上の位置
    int dep,val;
    BYTE *pixels;

    if(img==NULL) return -1;
    if(img->pixels==NULL) return -1;
    // 画像外の座標が指定されたらなにもしない
    if(x<0 || x >= img->width || y<0 || y >= img->height ) {
        return 0;
    }
    dep=img->depth;
    adr=x + y*img->width;
    pixels=img->pixels;
    if(dep==8) {
        pixels[adr]=correctValue(pix->r,PIXELMAX);
    }
    else if(dep==24) {
        pixels+=(adr*3);
        (*pixels)=correctValue(pix->r,PIXELMAX);
        pixels++;
        (*pixels)=correctValue(pix->g,PIXELMAX);
        pixels++;
        (*pixels)=correctValue(pix->b,PIXELMAX);
    }
    else {
        return -1;
    }
    return 1;
}