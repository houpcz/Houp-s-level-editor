/*
 * Windows Bitmap File Loader
 * Version 1.2.2 (20100827)
 *
 * Supported Formats: 1, 4, 8, 16, 24, 32 Bit Images
 * Alpha Bitmaps are also supported.
 * Supported compression types: RLE 8, BITFIELDS
 *
 * Created by: Benjamin Kalytta, 2006 - 2010
 *
 * Licence: Free to use, URL to my source and my name is required in your source code.
 *
 * Source can be found at http://www.kalytta.com/bitmap.h
 */

#include <cstring>

#define BITMAP_SIGNATURE 'MB'

typedef struct {
	unsigned short int Signature;
	unsigned int Size;
	unsigned int Reserved;
	unsigned int BitsOffset;
} BITMAP_FILEHEADER;

#define BITMAP_FILEHEADER_SIZE 14

typedef struct {
	unsigned int HeaderSize;
	int Width;
	int Height;
	unsigned short int Planes;
	unsigned short int BitCount;
	unsigned int Compression;
	unsigned int SizeImage;
	int PelsPerMeterX;
	int PelsPerMeterY;
	unsigned int ClrUsed;
	unsigned int ClrImportant;
	unsigned int RedMask;
	unsigned int GreenMask;
	unsigned int BlueMask;
	unsigned int AlphaMask;
	unsigned int CsType;
	unsigned int Endpoints[9]; // see http://msdn2.microsoft.com/en-us/library/ms536569.aspx
	unsigned int GammaRed;
	unsigned int GammaGreen;
	unsigned int GammaBlue;
} BITMAP_HEADER;

typedef struct {
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
	unsigned char Alpha;
} RGBA;

typedef struct {
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Alpha;
} BGRA;

class CBitmap {
private:
	BITMAP_FILEHEADER m_BitmapFileHeader;
	BITMAP_HEADER m_BitmapHeader;
	RGBA *m_BitmapData;
	unsigned int m_BitmapSize;
	BGRA *m_ColorTable;
	unsigned int m_ColorTableSize;

	// Masks and bit counts shouldn't exceed 32 Bits

	class CColor {
public:
		static inline unsigned int BitCountByMask(unsigned int Mask) {
			unsigned int BitCount = 0;
			while (Mask) {
				Mask &= Mask - 1;
				BitCount++;
			}
			return BitCount;
		}

		static inline unsigned int BitPositionByMask(unsigned int Mask) {
			return BitCountByMask((Mask & -Mask) - 1);
		}

		static inline unsigned int ComponentByMask(unsigned int Color, unsigned int Mask) {
			unsigned int Component = Color & Mask;
			return Component >> BitPositionByMask(Mask);
		}

		static inline unsigned int BitCountToMask(unsigned int BitCount) {
			return (BitCount == 32) ? 0xFFFFFFFF : (1 << BitCount) - 1;
		}

		static unsigned int Convert(unsigned int Color, unsigned int FromBitCount, unsigned int ToBitCount) {
			if (ToBitCount < FromBitCount) {
				Color >>= (FromBitCount - ToBitCount);
			} else {
				Color <<= (ToBitCount - FromBitCount);
				if (Color > 0) {
					Color |= BitCountToMask(ToBitCount - FromBitCount);
				}
			}
			return Color;
		}
	};

public:

	CBitmap() : m_BitmapData(0), m_BitmapSize(0), m_ColorTableSize(0), m_ColorTable(0) {
		Dispose();
	}

	CBitmap(char* Filename) : m_BitmapData(0), m_BitmapSize(0), m_ColorTableSize(0), m_ColorTable(0) {
		Load(Filename);
	}

	~CBitmap() {
		Dispose();
	}

	void Dispose() {
		if (m_BitmapData) delete[] m_BitmapData;
		if (m_ColorTable) delete[] m_ColorTable;
		m_ColorTableSize = 0;
		m_BitmapData = 0;
		m_ColorTable = 0;
		memset(&m_BitmapFileHeader, 0, sizeof(m_BitmapFileHeader));
		memset(&m_BitmapHeader, 0, sizeof(m_BitmapHeader));
	}

	/* Load specified Bitmap and stores it as RGBA in an internal buffer */

	bool Load(const char *Filename) {
		FILE *file = fopen(Filename, "rb");

		Dispose();

		if (file == 0) return false;

		fread(&m_BitmapFileHeader, BITMAP_FILEHEADER_SIZE, 1, file);
		if (m_BitmapFileHeader.Signature != BITMAP_SIGNATURE) {
			return false;
		}

		fread(&m_BitmapHeader, sizeof(BITMAP_HEADER), 1, file);

		/* Load Color Table */

		fseek(file, BITMAP_FILEHEADER_SIZE + m_BitmapHeader.HeaderSize, SEEK_SET);

		if (m_BitmapHeader.BitCount == 1) {
			m_ColorTableSize = 2;
		} else if (m_BitmapHeader.BitCount == 4) {
			m_ColorTableSize = 16;
		} else if (m_BitmapHeader.BitCount == 8) {
			m_ColorTableSize = 256;
		}

		m_ColorTable = new BGRA[m_ColorTableSize];

		fread(m_ColorTable, sizeof(BGRA), m_ColorTableSize, file);

		/* ... Color Table for 16 or 32 Bit Images are not supported yet */

		m_BitmapSize = GetWidth() * GetHeight();
		m_BitmapData = new RGBA[m_BitmapSize];

		unsigned int LineWidth = ((GetWidth() * GetBitCount() / 8) + 3) & ~3;
		unsigned char *Line = new unsigned char[LineWidth];

		fseek(file, m_BitmapFileHeader.BitsOffset, SEEK_SET);

		int Index = 0;

		if (m_BitmapHeader.Compression == 0) {
			for (int i = 0; i < GetHeight(); i++) {
				fread(Line, LineWidth, 1, file);

				unsigned char *LinePtr = Line;

				for (int j = 0; j < GetWidth(); j++) {
					if (m_BitmapHeader.BitCount == 1) {
						unsigned int Color = *((unsigned char*) LinePtr);
						for (int k = 0; k < 8; k++) {
							m_BitmapData[Index].Red = m_ColorTable[Color & 0x80 ? 1 : 0].Red;
							m_BitmapData[Index].Green = m_ColorTable[Color & 0x80 ? 1 : 0].Green;
							m_BitmapData[Index].Blue = m_ColorTable[Color & 0x80 ? 1 : 0].Blue;
							m_BitmapData[Index].Alpha = m_ColorTable[Color & 0x80 ? 1 : 0].Alpha;
							Index++;
							Color <<= 1;
						}
						LinePtr++;
						j += 7;
					} else if (m_BitmapHeader.BitCount == 4) {
						unsigned int Color = *((unsigned char*) LinePtr);
						m_BitmapData[Index].Red = m_ColorTable[(Color >> 4) & 0x0f].Red;
						m_BitmapData[Index].Green = m_ColorTable[(Color >> 4) & 0x0f].Green;
						m_BitmapData[Index].Blue = m_ColorTable[(Color >> 4) & 0x0f].Blue;
						m_BitmapData[Index].Alpha = m_ColorTable[(Color >> 4) & 0x0f].Alpha;
						Index++;
						m_BitmapData[Index].Red = m_ColorTable[Color & 0x0f].Red;
						m_BitmapData[Index].Green = m_ColorTable[Color & 0x0f].Green;
						m_BitmapData[Index].Blue = m_ColorTable[Color & 0x0f].Blue;
						m_BitmapData[Index].Alpha = m_ColorTable[Color & 0x0f].Alpha;
						Index++;
						LinePtr++;
						j++;
					} else if (m_BitmapHeader.BitCount == 8) {
						unsigned int Color = *((unsigned char*) LinePtr);
						m_BitmapData[Index].Red = m_ColorTable[Color].Red;
						m_BitmapData[Index].Green = m_ColorTable[Color].Green;
						m_BitmapData[Index].Blue = m_ColorTable[Color].Blue;
						m_BitmapData[Index].Alpha = m_ColorTable[Color].Alpha;
						Index++;
						LinePtr++;
					} else if (m_BitmapHeader.BitCount == 16) {
						unsigned int Color = *((unsigned short int*) LinePtr);
						m_BitmapData[Index].Red = ((Color >> 10) & 0x1f) << 3;
						m_BitmapData[Index].Green = ((Color >> 5) & 0x1f) << 3;
						m_BitmapData[Index].Blue = (Color & 0x1f) << 3;
						m_BitmapData[Index].Alpha = 255;
						Index++;
						LinePtr += 2;
					} else if (m_BitmapHeader.BitCount == 24) {
						unsigned int Color = *((unsigned int*) LinePtr);
						m_BitmapData[Index].Blue = Color & 0xff;
						m_BitmapData[Index].Green = (Color >> 8) & 0xff;
						m_BitmapData[Index].Red = (Color >> 16) & 0xff;
						m_BitmapData[Index].Alpha = 255;
						Index++;
						LinePtr += 3;
					} else if (m_BitmapHeader.BitCount == 32) {
						unsigned int Color = *((unsigned int*) LinePtr);
						m_BitmapData[Index].Blue = Color & 0xff;
						m_BitmapData[Index].Green = (Color >> 8) & 0xff;
						m_BitmapData[Index].Red = (Color >> 16) & 0xff;
						m_BitmapData[Index].Alpha = Color >> 24;
						Index++;
						LinePtr += 4;
					}
				}
			}
		} else if (m_BitmapHeader.Compression == 1) { // RLE 8
			unsigned char Count = 0;
			unsigned char ColorIndex = 0;
			int x = 0, y = 0;

			while (!feof(file)) {
				fread(&Count, 1, 1, file);
				fread(&ColorIndex, 1, 1, file);

				if (Count > 0) {
					Index = x + y * GetWidth();
					for (int k = 0; k < Count; k++) {
						m_BitmapData[Index + k].Red = m_ColorTable[ColorIndex].Red;
						m_BitmapData[Index + k].Green = m_ColorTable[ColorIndex].Green;
						m_BitmapData[Index + k].Blue = m_ColorTable[ColorIndex].Blue;
						m_BitmapData[Index + k].Alpha = m_ColorTable[ColorIndex].Alpha;
					}
					x += Count;
				} else if (Count == 0) {
					int Flag = ColorIndex;
					if (Flag == 0) {
						x = 0;
						y++;
					} else if (Flag == 1) {
						break;
					} else if (Flag == 2) {
						char rx = 0;
						char ry = 0;
						fread(&rx, 1, 1, file);
						fread(&ry, 1, 1, file);
						x += rx;
						y += ry;
					} else {
						Count = Flag;
						Index = x + y * GetWidth();
						for (int k = 0; k < Count; k++) {
							fread(&ColorIndex, 1, 1, file);
							m_BitmapData[Index + k].Red = m_ColorTable[ColorIndex].Red;
							m_BitmapData[Index + k].Green = m_ColorTable[ColorIndex].Green;
							m_BitmapData[Index + k].Blue = m_ColorTable[ColorIndex].Blue;
							m_BitmapData[Index + k].Alpha = m_ColorTable[ColorIndex].Alpha;
						}
						x += Count;
						if (ftell(file) & 1) fseek(file, 1, SEEK_CUR);
					}
				}
			}
		} else if (m_BitmapHeader.Compression == 2) { // RLE 4
			/* RLE 4 is not supported */
		} else if (m_BitmapHeader.Compression == 3) { // BITFIELDS

			/* We assumes that mask of each color component can be in any order */

			unsigned int BitCountRed = CColor::BitCountByMask(m_BitmapHeader.RedMask);
			unsigned int BitCountGreen = CColor::BitCountByMask(m_BitmapHeader.GreenMask);
			unsigned int BitCountBlue = CColor::BitCountByMask(m_BitmapHeader.BlueMask);
			unsigned int BitCountAlpha = CColor::BitCountByMask(m_BitmapHeader.AlphaMask);

			for (int i = 0; i < GetHeight(); i++) {
				fread(Line, LineWidth, 1, file);

				unsigned char *LinePtr = Line;

				for (int j = 0; j < GetWidth(); j++) {

					unsigned int Color = 0;

					if (m_BitmapHeader.BitCount == 16) {
						Color = *((unsigned short int*) LinePtr);
						LinePtr += 2;
					} else if (m_BitmapHeader.BitCount == 32) {
						Color = *((unsigned int*) LinePtr);
						LinePtr += 4;
					} else {
						// Other formats are not valid
					}
					m_BitmapData[Index].Red = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.RedMask), BitCountRed, 8);
					m_BitmapData[Index].Green = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.GreenMask), BitCountGreen, 8);
					m_BitmapData[Index].Blue = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.BlueMask), BitCountBlue, 8);
					m_BitmapData[Index].Alpha = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.AlphaMask), BitCountAlpha, 8);

					Index++;
				}
			}
		}

		fclose(file);
		return true;
	}

	bool Save(char* Filename, unsigned int BitCount = 32) {
		bool Result = false;

		FILE *file = fopen(Filename, "wb");

		if (file == 0) return false;

		BITMAP_FILEHEADER bfh = {0};
		BITMAP_HEADER bh = {0};

		bfh.Signature = BITMAP_SIGNATURE;
		bfh.BitsOffset = sizeof(BITMAP_HEADER) + sizeof(BITMAP_FILEHEADER);
		bfh.Size = (GetWidth() * GetHeight() * BitCount) / 8 + bfh.BitsOffset;

		bh.HeaderSize = sizeof(BITMAP_HEADER);
		bh.BitCount = BitCount;

		if (BitCount == 32) {
			bh.Compression = 3; // BITFIELD
			bh.AlphaMask = 0xff000000;
			bh.BlueMask = 0x00ff0000;
			bh.GreenMask = 0x0000ff00;
			bh.RedMask = 0x000000ff;
		} else if (BitCount == 16) {
			bh.Compression = 3; // BITFIELD
			bh.AlphaMask = 0x00000000;
			bh.BlueMask = 0x0000001f;
			bh.GreenMask = 0x000007E0;
			bh.RedMask = 0x0000F800;
		} else {
			bh.Compression = 0; // RGB
		}

		unsigned int LineWidth = (GetWidth() + 3) & ~3;

		bh.Planes = 1;
		bh.Height = GetHeight();
		bh.Width = GetWidth();
		bh.SizeImage = (LineWidth * BitCount * GetHeight()) / 8;
		bh.PelsPerMeterX = 3780;
		bh.PelsPerMeterY = 3780;

		if (BitCount == 32) {
			fwrite(&bfh, sizeof(BITMAP_FILEHEADER), 1, file);
			fwrite(&bh, sizeof(BITMAP_HEADER), 1, file);
			fwrite(m_BitmapData, bh.SizeImage, 1, file);
		} else if (BitCount < 16) {
			unsigned char* Bitmap = new unsigned char[bh.SizeImage];

			BGRA *Palette = 0;
			unsigned int PaletteSize = 0;

			if (GetBitsWithPalette(Bitmap, bh.SizeImage, BitCount, Palette, PaletteSize)) {
				bfh.BitsOffset += PaletteSize * sizeof(BGRA);

				fwrite(&bfh, sizeof(BITMAP_FILEHEADER), 1, file);
				fwrite(&bh, sizeof(BITMAP_HEADER), 1, file);
				fwrite(Palette, PaletteSize, sizeof(BGRA), file);
				fwrite(Bitmap, bh.SizeImage, 1, file);
			}
			delete [] Bitmap;
			delete [] Palette;
		} else {
			unsigned int RedMask = 0;
			unsigned int GreenMask = 0;
			unsigned int BlueMask = 0;
			unsigned int AlphaMask = 0;

			if (BitCount == 16) {
				RedMask = 0x0000F800;
				GreenMask = 0x000007E0;
				BlueMask = 0x0000001F;
				AlphaMask = 0x00000000;
			} else if (BitCount == 24) {
				RedMask = 0x00FF0000;
				GreenMask = 0x0000FF00;
				BlueMask = 0x000000FF;
			} else {
				// Other color formats are not valid
				return false;
			}

			if (GetBits(NULL, bh.SizeImage, RedMask, GreenMask, BlueMask, AlphaMask)) {
				unsigned char* Bitmap = new unsigned char[bh.SizeImage];
				if (GetBits(Bitmap, bh.SizeImage, RedMask, GreenMask, BlueMask, AlphaMask)) {
					fwrite(&bfh, sizeof(BITMAP_FILEHEADER), 1, file);
					fwrite(&bh, sizeof(BITMAP_HEADER), 1, file);
					fwrite(Bitmap, bh.SizeImage, 1, file);
				}
				delete [] Bitmap;
			}
		}
		Result = true;

		fclose(file);
		return Result;
	}

	unsigned int GetWidth() {
		return m_BitmapHeader.Width < 0 ? -m_BitmapHeader.Width : m_BitmapHeader.Width;
	}

	unsigned int GetHeight() {
		return m_BitmapHeader.Height < 0 ? -m_BitmapHeader.Height : m_BitmapHeader.Height;
	}

	unsigned int GetBitCount() {
		return m_BitmapHeader.BitCount;
	}

	/* Copies internal RGBA buffer to user specified buffer */

	bool GetBits(void* Buffer, unsigned int &Size) {
		bool Result = false;
		if (Size == 0 || Buffer == 0) {
			Size = m_BitmapSize * sizeof(RGBA);
			Result = m_BitmapSize != 0;
		} else {
			memcpy(Buffer, m_BitmapData, Size);
			Result = true;
		}
		return Result;
	}

	/* Returns internal RGBA buffer */

	void* GetBits() {
		return m_BitmapData;
	}

	/* Copies internal RGBA buffer to user specified buffer and converts it into destination
	 * bit format specified by component masks.
	 *
	 * Typical Bitmap color formats (BGR/BGRA):
	 *
	 * Masks for 16 bit (5-5-5): ALPHA = 0x00000000, RED = 0x00007C00, GREEN = 0x000003E0, BLUE = 0x0000001F
	 * Masks for 16 bit (5-6-5): ALPHA = 0x00000000, RED = 0x0000F800, GREEN = 0x000007E0, BLUE = 0x0000001F
	 * Masks for 24 bit: ALPHA = 0x00000000, RED = 0x00FF0000, GREEN = 0x0000FF00, BLUE = 0x000000FF
	 * Masks for 32 bit: ALPHA = 0xFF000000, RED = 0x00FF0000, GREEN = 0x0000FF00, BLUE = 0x000000FF
	 *
	 * Other color formats (RGB/RGBA):
	 *
	 * Masks for 32 bit (RGBA): ALPHA = 0xFF000000, RED = 0x000000FF, GREEN = 0x0000FF00, BLUE = 0x00FF0000
	 *
	 * Bit count will be rounded to next 8 bit boundary. If IncludePadding is true, it will be ensured
	 * that line width is a multiple of 4. padding bytes are included if necessary.
	 *
	 * NOTE: systems with big endian byte order may require masks in inversion order.
	 */

	bool GetBits(void* Buffer, unsigned int &Size, unsigned int RedMask, unsigned int GreenMask, unsigned int BlueMask, unsigned int AlphaMask, bool IncludePadding = true) {
		bool Result = false;

		unsigned int BitCountRed = CColor::BitCountByMask(RedMask);
		unsigned int BitCountGreen = CColor::BitCountByMask(GreenMask);
		unsigned int BitCountBlue = CColor::BitCountByMask(BlueMask);
		unsigned int BitCountAlpha = CColor::BitCountByMask(AlphaMask);

		unsigned int BitCount = (BitCountRed + BitCountGreen + BitCountBlue + BitCountAlpha + 7) & ~7;

		if (BitCount > 32) {
			return false;
		}

		unsigned int w = GetWidth();
		unsigned int LineWidth = (w + 3) & ~3;

		if (Size == 0 || Buffer == 0) {
			Size = (LineWidth * GetHeight() * BitCount) / 8 + sizeof(unsigned int);
			return true;
		}

		unsigned char* BufferPtr = (unsigned char*) Buffer;

		Result = true;

		unsigned int BitPosRed = CColor::BitPositionByMask(RedMask);
		unsigned int BitPosGreen = CColor::BitPositionByMask(GreenMask);
		unsigned int BitPosBlue = CColor::BitPositionByMask(BlueMask);
		unsigned int BitPosAlpha = CColor::BitPositionByMask(AlphaMask);

		int j = 0;

		for (int i = 0; i < m_BitmapSize; i++) {
			*(unsigned int*) BufferPtr =
			(CColor::Convert(m_BitmapData[i].Blue, 8, BitCountBlue) << BitPosBlue) |
			(CColor::Convert(m_BitmapData[i].Green, 8, BitCountGreen) << BitPosGreen) |
			(CColor::Convert(m_BitmapData[i].Red, 8, BitCountRed) << BitPosRed) |
			(CColor::Convert(m_BitmapData[i].Green, 8, BitCountAlpha) << BitPosAlpha);

			if (IncludePadding) {
				j++;
				if (j >= w) {
					for (unsigned int k = 0; k < LineWidth - w; k++) {
						BufferPtr += (BitCount >> 3);
					}
					j = 0;
				}
			}

			BufferPtr += (BitCount >> 3);
		}

		Size -= sizeof(unsigned int);

		return Result;
	}

	/* See GetBits().
	 * It creates a corresponding color table (palette) which have to be destroyed by the user after usage.
	 *
	 * Supported Bit depths are: 4, 8
	 *
	 * Todo: Optimize, use optimized palette, do ditehring (see my dithering class), support padding for 4 bit bitmaps
	 */

	bool GetBitsWithPalette(void* Buffer, unsigned int &Size, unsigned int BitCount, BGRA* &Palette, unsigned int &PaletteSize, bool OptimalPalette = false, bool IncludePadding = true) {
		bool Result = false;

		if (BitCount > 16) {
			return false;
		}

		unsigned int w = GetWidth();
		unsigned int LineWidth = (w + 3) & ~3;

		if (Size == 0 || Buffer == 0) {
			Size = (LineWidth * GetHeight() * BitCount) / 8;
			return true;
		}


		if (OptimalPalette) {
			PaletteSize = 0;
			// Not implemented
		} else {
			if (BitCount == 1) {
				PaletteSize = 2;
				// Not implemented: Who need that?
			} else if (BitCount == 4) { // 2:2:1
				PaletteSize = 16;
				Palette = new BGRA[PaletteSize];
				for (int r = 0; r < 4; r++) {
					for (int g = 0; g < 2; g++) {
						for (int b = 0; b < 2; b++) {
							Palette[r | g << 2 | b << 3].Red = r ? (r << 6) | 0x3f : 0;
							Palette[r | g << 2 | b << 3].Green = g ? (g << 7) | 0x7f : 0;
							Palette[r | g << 2 | b << 3].Blue = b ? (b << 7) | 0x7f : 0;
							Palette[r | g << 2 | b << 3].Alpha = 0xff;
						}
					}
				}
			} else if (BitCount == 8) { // 3:3:2
				PaletteSize = 256;
				Palette = new BGRA[PaletteSize];
				for (int r = 0; r < 8; r++) {
					for (int g = 0; g < 8; g++) {
						for (int b = 0; b < 4; b++) {
							Palette[r | g << 3 | b << 6].Red = r ? (r << 5) | 0x1f : 0;
							Palette[r | g << 3 | b << 6].Green = g ? (g << 5) | 0x1f : 0;
							Palette[r | g << 3 | b << 6].Blue = b ? (b << 6) | 0x3f : 0;
							Palette[r | g << 3 | b << 6].Alpha = 0xff;
						}
					}
				}
			} else if (BitCount == 16) { // 5:5:5
				// Not implemented
			}
		}

		unsigned int j = 0;
		unsigned char* BufferPtr = (unsigned char*) Buffer;

		for (unsigned int i = 0; i < m_BitmapSize; i++) {
			if (BitCount == 1) {
				// Not implemented: Who needs that?
			} else if (BitCount == 4) {
				*BufferPtr = ((m_BitmapData[i].Red >> 6) | (m_BitmapData[i].Green >> 7) << 2 | (m_BitmapData[i].Blue >> 7) << 3) << 4;
				i++;
				*BufferPtr |= (m_BitmapData[i].Red >> 6) | (m_BitmapData[i].Green >> 7) << 2 | (m_BitmapData[i].Blue >> 7) << 3;
			} else if (BitCount == 8) {
				*BufferPtr = (m_BitmapData[i].Red >> 5) | (m_BitmapData[i].Green >> 5) << 3 | (m_BitmapData[i].Blue >> 5) << 6;
			} else if (BitCount == 16) {
				// Not implemented
			}

			if (IncludePadding) {
				j++;
				if (j >= w) {
					for (unsigned int k = 0; k < (LineWidth - w); k++) {
						BufferPtr += BitCount / 8;
					}
					j = 0;
				}
			}

			BufferPtr++;
		}

		Result = true;

		return Result;
	}

	/* Set Bitmap Bits. Will be converted to RGBA internally */

	void SetBits(void* Buffer, unsigned int Width, unsigned int Height, unsigned int RedMask, unsigned int GreenMask, unsigned int BlueMask, unsigned int AlphaMask = 0) {
		unsigned char *BufferPtr = (unsigned char*) Buffer;

		Dispose();

		m_BitmapHeader.Width = Width;
		m_BitmapHeader.Height = Height;
		m_BitmapHeader.BitCount = 32;
		m_BitmapHeader.Compression = 3;

		m_BitmapSize = GetWidth() * GetHeight();
		m_BitmapData = new RGBA[m_BitmapSize];

		/* Find bit count by masks (rounded to next 8 bit boundary) */

		unsigned int BitCount = (CColor::BitCountByMask(RedMask | GreenMask | BlueMask | AlphaMask) + 7) & ~7;

		unsigned int BitCountRed = CColor::BitCountByMask(RedMask);
		unsigned int BitCountGreen = CColor::BitCountByMask(GreenMask);
		unsigned int BitCountBlue = CColor::BitCountByMask(BlueMask);
		unsigned int BitCountAlpha = CColor::BitCountByMask(AlphaMask);

		for (unsigned int i = 0; i < m_BitmapSize; i++) {
			unsigned int Color = 0;
			if (BitCount <= 8) {
				Color = *((unsigned char*) BufferPtr);
				BufferPtr += 1;
			} else if (BitCount <= 16) {
				Color = *((unsigned short int*) BufferPtr);
				BufferPtr += 2;
			} else if (BitCount <= 24) {
				Color = *((unsigned int*) BufferPtr);
				BufferPtr += 3;
			} else if (BitCount <= 32) {
				Color = *((unsigned int*) BufferPtr);
				BufferPtr += 4;
			} else {
				/* unsupported */
				BufferPtr += 1;
			}
			m_BitmapData[i].Alpha = CColor::Convert(CColor::ComponentByMask(Color, AlphaMask), BitCountAlpha, 8);
			m_BitmapData[i].Red = CColor::Convert(CColor::ComponentByMask(Color, RedMask), BitCountRed, 8);
			m_BitmapData[i].Green = CColor::Convert(CColor::ComponentByMask(Color, GreenMask), BitCountGreen, 8);
			m_BitmapData[i].Blue = CColor::Convert(CColor::ComponentByMask(Color, BlueMask), BitCountBlue, 8);
		}
	}
};
