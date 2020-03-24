//
// Created by Jason Zheng on 2020/3/17.
//

#ifndef SANDRA_SGR_H
#define SANDRA_SGR_H

#include <stdio.h>
#include <string.h>

/* ======= ANSI Escape Code (SGR parameters) ======= */
#define CSI "\x1b["
#define SGR(n) CSI #n "m"

#define SGR_RESET SGR(0)         // all attributes off
#define SGR_BOLD SGR(1)
#define SGR_FAINT SGR(2)
#define SGR_ITALIC SGR(3)        // Not widely supported. Sometimes treated as inverse.
#define SGR_UNDERLINE SGR(4)
#define SGR_SLOW_BLINK SGR(5)    // less than 150 per minute
#define SGR_RAPID_BLINK SGR(6)   // MS-DOS ANSI.SYS; 150+ per minute; not widely supported
#define SGR_REVERSE_VIDEO SGR(7) // swap foreground and background colors
#define SGR_CONCEAL SGR(8)       // Not widely supported.
#define SGR_CROSSED_OUT SGR(9)   // Characters legible, but marked for deletion.
#define SGR_PRIMARY_FONT SGR(10)
//   11–19,  Alternative font,  Select alternative font n − 10
#define SGR_FRAKTUR SGR(20)      // Rarely supported
#define SGR_DOUBLY_UNDERLINE_OR_BOLD_OFF SGR(21) // Double-underline per ECMA-48.
#define SGR_NORMAL_COLOR_OR_INTENSITY SGR(22)    // Neither bold nor faint
#define SGR_NOT_ITALIC_NOT_FRAKTUR SGR(23)
#define SGR_UNDERLINE_OFF SGR(24)
#define SGR_BLINK_OFF SGR(25)
#define SGR_INVERSE_OFF SGR(27)
#define SGR_REVEAL SGR(28)
#define SGR_NOT_CROSSED_OUT SGR(29)
//   30–37,  Set foreground color,  See color table below
//      38,  Set foreground color,  Next arguments are 5;n or 2;r;g;b, see below
#define SGR_DEFAULT_FOREGROUND_COLOR SGR(39)     // implementation defined (according to standard)
//   40–47,  Set background color,  See color table below
//      48,  Set background color,  Next arguments are 5;n or 2;r;g;b, see below
#define SGR_DEFAULT_BACKGROUND_COLOR SGR(49)
#define SGR_FRAMED SGR(51)
#define SGR_ENCIRCLED SGR(52)
#define SGR_OVERLINED SGR(53)
#define SGR_NOT_FRAMED_OR_ENCIRCLED SGR(54)
#define SGR_NOT_OVERLINED SGR(55)
#define SGR_IDEOGRAM_UNDERLINE_OR_RIGHT_SIDE_LINE SGR(60)   // Rarely supported
#define SGR_IDEOGRAM_DOUBLE_UNDERLINE_OR_DOUBLE_LINE_ON_THE_RIGHT_SIDE SGR(61)  // Rarely supported
#define SGR_IDEOGRAM_OVERLINE_OR_LEFT_SIDE_LINE SGR(62)     // Rarely supported
#define SGR_IDEOGRAM_DOUBLE_OVERLINE_OR_DOUBLE_LINE_ON_THE_LEFT_SIDE SGR(63)    // Rarely supported
#define SGR_IDEOGRAM_STRESS_MARKING SGR(64)
#define SGR_IDEOGRAM_ATTRIBUTES_OFF SGR(65)      // reset the effects of all of 60–64
//   90–97,  Set bright foreground color,  aixterm (not in standard)
// 100–107,  Set bright background color,  aixterm (not in standard)

/* === Colors === */
// 3/4 bit
#define SGR_FG_BLACK SGR(30)
#define SGR_FG_RED SGR(31)
#define SGR_FG_GREEN SGR(32)
#define SGR_FG_YELLOW SGR(33)
#define SGR_FG_BLUE SGR(34)
#define SGR_FG_MAGENTA SGR(35)
#define SGR_FG_CYAN SGR(36)
#define SGR_FG_WHITE SGR(37)

#define SGR_FG_BRIGHT_BLACK SGR(90)
#define SGR_FG_BRIGHT_RED SGR(91)
#define SGR_FG_BRIGHT_GREEN SGR(92)
#define SGR_FG_BRIGHT_YELLOW SGR(93)
#define SGR_FG_BRIGHT_BLUE SGR(94)
#define SGR_FG_BRIGHT_MAGENTA SGR(95)
#define SGR_FG_BRIGHT_CYAN SGR(96)
#define SGR_FG_BRIGHT_WHITE SGR(96)

#define SGR_BG_BLACK SGR(40)
#define SGR_BG_RED SGR(41)
#define SGR_BG_GREEN SGR(42)
#define SGR_BG_YELLOW SGR(43)
#define SGR_BG_BLUE SGR(44)
#define SGR_BG_MAGENTA SGR(45)
#define SGR_BG_CYAN SGR(46)
#define SGR_BG_WHITE SGR(47)

#define SGR_BG_BRIGHT_BLACK SGR(100)
#define SGR_BG_BRIGHT_RED SGR(101)
#define SGR_BG_BRIGHT_GREEN SGR(102)
#define SGR_BG_BRIGHT_YELLOW SGR(103)
#define SGR_BG_BRIGHT_BLUE SGR(104)
#define SGR_BG_BRIGHT_MAGENTA SGR(105)
#define SGR_BG_BRIGHT_CYAN SGR(106)
#define SGR_BG_BRIGHT_WHITE SGR(106)

//  ESC[ 38;5;⟨n⟩ m Select foreground color
//  ESC[ 48;5;⟨n⟩ m Select background color
//    0-  7:  standard colors (as in ESC [ 30–37 m)
//    8- 15:  high intensity colors (as in ESC [ 90–97 m)
//   16-231:  6 × 6 × 6 cube (216 colors): 16 + 36 × r + 6 × g + b (0 ≤ r, g, b ≤ 5)
//  232-255:  grayscale from black to white in 24 steps
#define SGR_FG(n) SGR(38;5;n)
#define SGR_BG(n) SGR(48;5;n)

/**
 * 0 ≤ r, g, b ≤ 5
 * @param buf  The buffer to store a string of SGR 8bit-color, at least 16 in length
 */
static inline char *sgr_fg_color(char *buf, int r, int g, int b) {
    int val = 16 + (36 * r) + (6 * g) + b;
    char *fmt = SGR_FG(%d);
    sprintf(buf, fmt, val);
    return buf;
}

/**
 * 0 ≤ r, g, b ≤ 5
 * @param buf  The buffer to store a string of SGR 8bit-color, at least 16 in length
 */
static inline char *sgr_bg_color(char *buf, int r, int g, int b) {
    int val = 16 + (36 * r) + (6 * g) + b;
    char *fmt = SGR_BG(%d);
    sprintf(buf, fmt, val);
    return buf;
}

#define SGR_24_FG_COLOR(R, G, B) SGR(38;2;R;G;B)
#define SGR_24_BG_COLOR(R, G, B) SGR(48;2;R;G;B)

#endif //SANDRA_SGR_H
