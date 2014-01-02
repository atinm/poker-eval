#include "poker_defs.h"

/* 
 * Table StdDeck_maskRankTable
 */

/*
StdDeck_masksRanksTable[].  Maps card ranks (2..A) to a cardmask which
has all the bits set except the bits corresponding to the cards whose
have the input rank.  This is a quick way to mask off all the cards of
a specific rank.
 */

StdDeck_CardMask StdDeck_maskRankTable[13] = { 
      { 0xfffefffefffefffeLL }  ,
      { 0xfffdfffdfffdfffdLL }  ,
      { 0xfffbfffbfffbfffbLL }  ,
      { 0xfff7fff7fff7fff7LL }  ,
      { 0xffefffefffefffefLL }  ,
      { 0xffdfffdfffdfffdfLL }  ,
      { 0xffbfffbfffbfffbfLL }  ,
      { 0xff7fff7fff7fff7fLL }  ,
      { 0xfefffefffefffeffLL }  ,
      { 0xfdfffdfffdfffdffLL }  ,
      { 0xfbfffbfffbfffbffLL }  ,
      { 0xf7fff7fff7fff7ffLL }  ,
      { 0xefffefffefffefffLL }  
};
