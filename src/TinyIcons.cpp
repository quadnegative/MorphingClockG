/*
 * mirel.lazar@gmail.com
 * 
 * provided 'AS IS', use it at your own risk
 */
#include "TinyIcons.h"

//void DrawIcon (PxMATRIX* d, int *ico, char xo, char yo, int cols, int rows)
void DrawIconDMA (MatrixPanel_I2S_DMA* d, int *ico, char xo, char yo, int cols, int rows) {
  int i, j;
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < cols; j++)
    {
      d->drawPixel (xo + j, yo + i, ico[i * cols + j]);
    }
  }
}
