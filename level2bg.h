
//{{BLOCK(level2bg)

//======================================================================
//
//	level2bg, 512x512@4, 
//	+ palette 256 entries, not compressed
//	+ 21 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 672 + 8192 = 9376
//
//	Time-stamp: 2022-11-29, 15:56:08
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL2BG_H
#define GRIT_LEVEL2BG_H

#define level2bgTilesLen 672
extern const unsigned short level2bgTiles[336];

#define level2bgMapLen 8192
extern const unsigned short level2bgMap[4096];

#define level2bgPalLen 512
extern const unsigned short level2bgPal[256];

#endif // GRIT_LEVEL2BG_H

//}}BLOCK(level2bg)
