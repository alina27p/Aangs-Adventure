
//{{BLOCK(level3bg)

//======================================================================
//
//	level3bg, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 180 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5760 + 2048 = 8320
//
//	Time-stamp: 2022-11-29, 16:33:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL3BG_H
#define GRIT_LEVEL3BG_H

#define level3bgTilesLen 5760
extern const unsigned short level3bgTiles[2880];

#define level3bgMapLen 2048
extern const unsigned short level3bgMap[1024];

#define level3bgPalLen 512
extern const unsigned short level3bgPal[256];

#endif // GRIT_LEVEL3BG_H

//}}BLOCK(level3bg)
