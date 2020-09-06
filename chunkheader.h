#ifndef CHUNKHEADER_H
#define CHUNKHEADER_H

enum ChunkType { SECTION, RELOCATION, SYMBOLS, HEADER };

struct ChunkHeader {
  ChunkType type;
  int size;
};

#endif
