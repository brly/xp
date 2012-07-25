
#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

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

const std::string kCacheDir = "cache";
const std::string kFeatureVectorDir = kCacheDir + "/feature_vector";
const std::string kWeightVectorDir = kCacheDir + "/weight_vector";
const std::string kImageDir = "easy_test";

#endif // CONSTANT_H
