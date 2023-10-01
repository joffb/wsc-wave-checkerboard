#include "sine.h"

/*
var out = []; 
for (var i = 0; i < 256; i++) { 
  out.push(Math.floor(Math.sin((i * 2* Math.PI) / 255.0) * 32) + 32);
}
JSON.stringify(out)
*/

const unsigned char sine[256] = {
    32,32,33,34,35,35,36,37,38,39,39,40,41,42,42,43,44,
    45,45,46,47,47,48,49,49,50,51,51,52,52,53,54,54,55,
    55,56,56,57,57,58,58,59,59,59,60,60,60,61,61,61,62,
    62,62,62,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
    63,63,63,63,63,63,62,62,62,62,62,61,61,61,60,60,60,
    59,59,58,58,58,57,57,56,56,55,54,54,53,53,52,52,51,
    50,50,49,48,48,47,46,46,45,44,43,43,42,41,40,40,39,
    38,37,37,36,35,34,33,33,32,31,30,30,29,28,27,26,26,
    25,24,23,23,22,21,20,20,19,18,17,17,16,15,15,14,13,
    13,12,11,11,10,10,9,9,8,7,7,6,6,5,5,5,4,4,3,3,3,2,2,
    2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,2,2,2,3,3,3,4,4,4,5,5,6,6,7,7,8,8,9,9,10,11,
    11,12,12,13,14,14,15,16,16,17,18,18,19,20,21,21,22,
    23,24,24,25,26,27,28,28,29,30,31,31
};