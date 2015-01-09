/* Extra lite version of LWE (c) 2015 Tom Wright */
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include "defines.h"

c *b; i bs, g, gs, lines, cols, scrl;

i bext() { gs = bs; bs*=2; b=realloc(b, bs); if (b==NULL) return 0; return 1; }
i bput(c c_) { b[g++]=c_; gs--; if (gs==0) return bext(); return 1; }
i bread(str fn) {
   c c_; FILE *f; bs=4096; g = 0; gs = bs;
   b = malloc(bs); if (b==NULL) { err("memory"); return 0; }
   f = fopen(fn, "r"); if (f==NULL) { err("read"); return 0; }
   for (c_=fgetc(f);c_!=EOF;c_=fgetc(f)) if (!bput(c_)) return 0;
   return 1; }
i isend(c *s_) { return s_ >= (b + bs - gs); }
v draw() {
   i r_, c_, i_; c *s_; r_ = c_ = 0; i_ = scrl;
   for (s_=b; i_ > 0 && !isend(s_); s_++) if (*s_=='\n') i_--;
   p("\x1B[2J\x1B[H");
   draw: if (isend(s_)) return;
   pc(*s_++); c_++; if (*s_=='\n') c_=0; c_%=cols; r_+=c_==0?1:0;
   if (r_ <= lines) goto draw; }
v doscrl(i d_) { scrl+=d_; if (scrl < 0) scrl=0; }
v cmdloop() { draw(); }
v ed(str fn_) { if (!bread(fn_)) return; scrl=0; cmdloop(); }
v dtlines() {
   struct winsize w_; ioctl(0, TIOCGWINSZ, &w_);
   lines = w_.ws_row; cols = w_.ws_col; }
int main(i argc, c **argv) {
   dtlines();
   if (argc != 2) { err("missing file arg"); return 2; }
   else { ed(argv[1]); return 0; } }