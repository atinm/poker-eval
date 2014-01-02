#include "poker_defs.h"

/* 
 * Table AStudDeck_cardMasksTable
 */

/*
AStudDeck_cardMasks[].  Maps card indices (0..32) to CardMasks.  
The output mask has only one bit set, the bit corresponding to the card
identified by the index.
 */

#include "deck_astud.h" 
AStudDeck_CardMask AStudDeck_cardMasksTable[32] = { 
      { 0x0020000000000000LL }  ,
      { 0x0040000000000000LL }  ,
      { 0x0080000000000000LL }  ,
      { 0x0100000000000000LL }  ,
      { 0x0200000000000000LL }  ,
      { 0x0400000000000000LL }  ,
      { 0x0800000000000000LL }  ,
      { 0x1000000000000000LL }  ,
      { 0x0000002000000000LL }  ,
      { 0x0000004000000000LL }  ,
      { 0x0000008000000000LL }  ,
      { 0x0000010000000000LL }  ,
      { 0x0000020000000000LL }  ,
      { 0x0000040000000000LL }  ,
      { 0x0000080000000000LL }  ,
      { 0x0000100000000000LL }  ,
      { 0x0000000000200000LL }  ,
      { 0x0000000000400000LL }  ,
      { 0x0000000000800000LL }  ,
      { 0x0000000001000000LL }  ,
      { 0x0000000002000000LL }  ,
      { 0x0000000004000000LL }  ,
      { 0x0000000008000000LL }  ,
      { 0x0000000010000000LL }  ,
      { 0x0000000000000020LL }  ,
      { 0x0000000000000040LL }  ,
      { 0x0000000000000080LL }  ,
      { 0x0000000000000100LL }  ,
      { 0x0000000000000200LL }  ,
      { 0x0000000000000400LL }  ,
      { 0x0000000000000800LL }  ,
      { 0x0000000000001000LL }  
};
