//imageManip.h
//Devin Ramsden; dramsde3@gmail.com

#ifndef IMANGEMANIP_H
#define IMANGEMANIP_H

void adjust_contrast(double factor, Pixel* p);
void free_all(Image *im);
double sq(double val);
double g_num(double sigma, int dx, int dy);
void gaussian_dis(double sigma,int n,double filter[][n]);
void swap(Image * im);
void grayscale(Image * im);
void contrast(Image * im, double factor);
Image * zoom_in(Image * im);
Image * zoom_out(Image * im);
Image * occlude(Image * im, int top_left_r, int top_left_c, int bottom_right_r, int bottom_right_c);
Image * blur(Image * im, double sigma);

#endif
