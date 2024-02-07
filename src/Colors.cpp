#include <Arduino.h>
#include "Main.h"
#include "Colors.h"
#include "Config.h"
#include "Display.h"

int cc_time;
int cc_wind;
int cc_date;
int cc_wtext;

int color_disp = dma_display->color565(40, 40, 50);  // primary color

// some other colors
// R G B
int cc_blk = dma_display->color565(0, 0, 0);            // black          0x0000

int cc_dwht = dma_display->color565(64, 64, 64);        // dim white      0x4208
int cc_wht = dma_display->color565(128, 128, 128);      // white          0x8410
int cc_bwht = dma_display->color565(255, 255, 255);     // bright white   0xFFFF

int cc_dred = dma_display->color565(64, 0, 0);          // dim red        0x4000
int cc_red = dma_display->color565(128, 0, 0);          // red            0x8000
int cc_bred = dma_display->color565(255, 0, 0);         // bright red     0xF800

int cc_dorg = dma_display->color565(64, 32, 0);         // dim orange     0x4100
int cc_org = dma_display->color565(128, 64, 0);         // orange         0x8200
int cc_borg = dma_display->color565(255, 128, 0);       // bright orange  0xFC00

int cc_dgrn = dma_display->color565(0, 64, 0);          // dim green      0x0200
int cc_grn = dma_display->color565(0, 128, 0);          // green          0x0400
int cc_bgrn = dma_display->color565(0, 255, 0);         // bright green   0x07E0

int cc_dblu = dma_display->color565(0, 0, 64);          // dim blue       0x0008
int cc_blu = dma_display->color565(0, 0, 128);          // blue           0x0010
int cc_bblu = dma_display->color565(0, 0, 255);         // bright blue    0x001F

int cc_dylw = dma_display->color565(64, 64, 0);         // dim yellow     0x4200
int cc_ylw = dma_display->color565(128, 128, 0);        // yellow         0x8400
int cc_bylw = dma_display->color565(255, 255, 0);       // bright yellow  0xFFE0

int cc_gry = dma_display->color565(10, 10, 10);         // gray
int cc_bgry = dma_display->color565(128, 128, 128);     // bright gray
int cc_dgr = dma_display->color565(3, 3, 3);            // dark grey

int cc_dcyan = dma_display->color565(0, 64, 64);        // dim cyan       0x0208
int cc_cyan = dma_display->color565(0, 128, 128);       // cyan           0x0410
int cc_bcyan = dma_display->color565(0, 255, 255);      // bright cyan    0x07FF

int cc_dppl = dma_display->color565(64, 0, 64);         // dim purple     0x4008
int cc_ppl = dma_display->color565(128, 0, 128);        // purple         0x8010
int cc_bppl = dma_display->color565(255, 0, 255);       // bright purple  0xF81F

void select_palette() {
  int x;
  x = atoi(config["ColorPalette"]);
  debug(F("Palette:"));
  debugln(x);

  switch (x) {
    default:
    case 0:
      cc_time = cc_cyan;
      cc_wind = cc_ylw;
      cc_date = cc_grn;
      cc_wtext = cc_wht;
      break;
    case 1:
      cc_time = cc_red;
      cc_wind = cc_ylw;
      cc_date = cc_blu;
      cc_wtext = cc_grn;
      break;
    case 2:
      cc_time = cc_blu;
      cc_wind = cc_grn;
      cc_date = cc_ylw;
      cc_wtext = cc_wht;
      break;
    case 3:
      cc_time = cc_ylw;
      cc_wind = cc_cyan;
      cc_date = cc_blu;
      cc_wtext = cc_grn;
      break;
    case 4:
      cc_time = cc_bblu;
      cc_wind = cc_grn;
      cc_date = cc_ylw;
      cc_wtext = cc_grn;
      break;
    case 5:
      cc_time = cc_org;
      cc_wind = cc_red;
      cc_date = cc_grn;
      cc_wtext = cc_ylw;
      break;
    case 6:
      cc_time = cc_grn;
      cc_wind = cc_ppl;
      cc_date = cc_cyan;
      cc_wtext = cc_ylw;
      break;
    case 7:
      cc_time = dma_display->color565(config["ColorPalette7"]["time"]["r"], config["ColorPalette7"]["time"]["g"], config["ColorPalette7"]["time"]["b"]);
      cc_wind = dma_display->color565(config["ColorPalette7"]["wind"]["r"], config["ColorPalette7"]["wind"]["g"], config["ColorPalette7"]["wind"]["b"]);
      cc_date = dma_display->color565(config["ColorPalette7"]["date"]["r"], config["ColorPalette7"]["date"]["g"], config["ColorPalette7"]["date"]["b"]);
      cc_wtext = dma_display->color565(config["ColorPalette7"]["weather"]["r"], config["ColorPalette7"]["weather"]["g"], config["ColorPalette7"]["weather"]["b"]);
      break;
    case 8:
      cc_time = dma_display->color565(config["ColorPalette8"]["time"]["r"], config["ColorPalette8"]["time"]["g"], config["ColorPalette8"]["time"]["b"]);
      cc_wind = dma_display->color565(config["ColorPalette8"]["wind"]["r"], config["ColorPalette8"]["wind"]["g"], config["ColorPalette8"]["wind"]["b"]);
      cc_date = dma_display->color565(config["ColorPalette8"]["date"]["r"], config["ColorPalette8"]["date"]["g"], config["ColorPalette8"]["date"]["b"]);
      cc_wtext = dma_display->color565(config["ColorPalette8"]["weather"]["r"], config["ColorPalette8"]["weather"]["g"], config["ColorPalette8"]["weather"]["b"]);
      break;
    case 9:
      cc_time = dma_display->color565(config["ColorPalette9"]["time"]["r"], config["ColorPalette9"]["time"]["g"], config["ColorPalette9"]["time"]["b"]);
      cc_wind = dma_display->color565(config["ColorPalette9"]["wind"]["r"], config["ColorPalette9"]["wind"]["g"], config["ColorPalette9"]["wind"]["b"]);
      cc_date = dma_display->color565(config["ColorPalette9"]["date"]["r"], config["ColorPalette9"]["date"]["g"], config["ColorPalette9"]["date"]["b"]);
      cc_wtext = dma_display->color565(config["ColorPalette9"]["weather"]["r"], config["ColorPalette9"]["weather"]["g"], config["ColorPalette9"]["weather"]["b"]);
      break;
    case 10:
      cc_time = dma_display->color565(config["ColorPalette10"]["time"]["r"], config["ColorPalette10"]["time"]["g"], config["ColorPalette10"]["time"]["b"]);
      cc_wind = dma_display->color565(config["ColorPalette10"]["wind"]["r"], config["ColorPalette10"]["wind"]["g"], config["ColorPalette10"]["wind"]["b"]);
      cc_date = dma_display->color565(config["ColorPalette10"]["date"]["r"], config["ColorPalette10"]["date"]["g"], config["ColorPalette10"]["date"]["b"]);
      cc_wtext = dma_display->color565(config["ColorPalette10"]["weather"]["r"], config["Custom7"]["ColorPalette10"]["g"], config["ColorPalette10"]["weather"]["b"]);
      break;
  }
  clockStartingUp = true;
}