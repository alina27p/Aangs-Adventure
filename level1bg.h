
//{{BLOCK(level1bg)

//======================================================================
//
//	level1bg, 1280x256@4, 
//	+ palette 256 entries, not compressed
//	+ 112 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 160x32 
//	Total size: 512 + 3584 + 10240 = 14336
//
//	Time-stamp: 2022-12-01, 20:42:29
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL1BG_H
#define GRIT_LEVEL1BG_H

#define level1bgTilesLen 3584
extern const unsigned short level1bgTiles[1792];

#define level1bgMapLen 10240
extern const unsigned short level1bgMap[5120];

#define level1bgPalLen 512
extern const unsigned short level1bgPal[256];

#endif // GRIT_LEVEL1BG_H

//}}BLOCK(level1bg)
