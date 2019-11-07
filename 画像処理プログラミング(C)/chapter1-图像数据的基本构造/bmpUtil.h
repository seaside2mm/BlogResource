//
// Created by kubota on 11/7/19.
//

#ifndef CHAPTER1_BMPUTIL_H
#define CHAPTER1_BMPUTIL_H
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned short      WORD;
typedef unsigned long       LONG;

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

// BMPヘッダ部のデータ構造定義
typedef struct tagBITMAPFILEHEADER {
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPCOREHEADER {
    DWORD   bcSize;
    WORD    bcWidth;
    WORD    bcHeight;
    WORD    bcPlanes;
    WORD    bcBitCount;
} BITMAPCOREHEADER, *PBITMAPCOREHEADER;

typedef struct tagBITMAPINFOHEADER{
    DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#define MAXCOLORS 256

// ファイルより２バイト整数を書き込む（リトルエンディアン）
int fwriteWORD(WORD val,FILE *fp)
{
    int i,c;

    c=val;
    for(i=0;i<2;i++) {
        fputc(c%256,fp);
        c/=256;
    }
    return TRUE;
}

// ファイルより４バイト整数を書き込む（リトルエンディアン）
int fwriteDWORD(DWORD val,FILE *fp)
{
    int i,c;

    c=val;
    for(i=0;i<4;i++) {
        fputc(c%256,fp);
        c/=256;
    }
    return TRUE;
}

// ファイルより２バイト整数を読み込む（リトルエンディアン）
int freadWORD(WORD *res,FILE *fp)
{
    int i,c;
    int val[2];

    for(i=0;i<2;i++) {
        c=fgetc(fp);
        if(c==EOF) return FALSE;
        val[i]=c;
    }
    *res=val[1]*256+val[0];
    return TRUE;
}

// ファイルより４バイト整数を読み込む（リトルエンディアン）
int  freadDWORD(DWORD *res,FILE *fp)
{
    int i,c;
    int val[4];
    DWORD tmp=0;

    for(i=0;i<4;i++) {
        c=fgetc(fp);
        if(c==EOF) return FALSE;
        val[i]=c;
    }
    tmp=0;
    for(i=3;i>=0;i--) {
        tmp*=256;
        tmp+=val[i];
    }
    *res=tmp;
    return TRUE;
}

// BMPの種類を判別
// 戻り値：FALSE　OS/2形式
//           TRUE   WIndows形式
static BOOL	IsWinDIB(BITMAPINFOHEADER* pBIH)
{
    if (((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER)) {
        return FALSE;
    }
    return TRUE;
}

// パレットのサイズを取得
// iBitCount １画素あたりのビット数
int countOfDIBColorEntries(int iBitCount)
{
    int	iColors;

    switch (iBitCount) {
        case 1:
            iColors	= 2;
            break;
        case 4:
            iColors	= 16;
            break;
        case 8:
            iColors	= 256;
            break;
        default:
            iColors	= 0;
            break;
    }

    return iColors;
}

// パディング要素を考慮して１列分のバイト数を求める
int getDIBxmax(int mx,int dep)
{
    switch(dep) {
        case 32:
            return mx*4;
        case 24:
            //return mx;
            return ((mx*3)+3)/4*4;
            break;
        case 16:
            return (mx+1)/2*2;
            break;
        case 8:
            return (mx+3)/4*4;
            break;
        case 4:
            return (((mx+1)/2)+3)/4*4;
            break;
        case 1:
            return (((mx+7)/8)+3)/4*4;
    }
    return mx;
}

#endif //CHAPTER1_BMPUTIL_H
