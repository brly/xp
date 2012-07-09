
#ifndef CONSTANT_H
#define CONSTANT_H

enum {
  kCellX       = 5,
  kCellY       = 5,
  kCellBin     = 9,
  kOrientation = kCellBin,
  kBlockX      = 3,
  kBlockY      = 3,
  kResizeX     = 40,
  kResizeY     = 40,
  kCellWidth   = kResizeX / kCellX,
  kCellHeight  = kResizeY / kCellY,
  kBlockWidth  = kCellWidth - kBlockX + 1,
  kBlockHeight = kCellHeight - kBlockY + 1,
  kBlockDim    = kBlockX * kBlockY * kCellBin,
  kTotalDim    = kBlockDim * kBlockWidth * kBlockHeight,
  kGlyphSize   = 20
};

#endif // CONSTANT_H
