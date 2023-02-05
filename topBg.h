
//{{BLOCK(topBg)

//======================================================================
//
//	topBg, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 160 + 2048 = 2720
//
//	Time-stamp: 2022-11-29, 17:23:22
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TOPBG_H
#define GRIT_TOPBG_H

#define topBgTilesLen 160
extern const unsigned short topBgTiles[80];

#define topBgMapLen 2048
extern const unsigned short topBgMap[1024];

#define topBgPalLen 512
extern const unsigned short topBgPal[256];

#endif // GRIT_TOPBG_H

//}}BLOCK(topBg)
