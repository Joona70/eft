#define RGB(r,g,b) ((r & 0xff) << 16) + ((g & 0xff) << 8) + ((b & 0xff))

class RenderTexture {
public:
	ULONG Width;
	ULONG Height;
	ULONG PixelsBGRA[];

	__forceinline ULONG GetTextureSize() {
		return this->Width * this->Height * 4;
	}
	__forceinline void ReleaseTexture() {
		ULONG TextureSize = GetTextureSize();
		MemZero(PixelsBGRA, TextureSize);
	}
	__forceinline void SetPixel(int X, int Y, ULONG Color)
	{
		union PixelRGBA {
			ULONG Color;
			struct ChannelRGBA {
				BYTE B;
				BYTE G;
				BYTE R;
				BYTE A;
			} RGBA;
		} PixelData;

		PixelData.RGBA.B = ((Color >> 16) & 0xFF); PixelData.RGBA.G = ((Color >> 8) & 0xFF);
		PixelData.RGBA.R = ((Color) & 0xFF); PixelData.RGBA.A = 255;

		if ((X >= 0 && X < this->Width) && (Y >= 0 && Y < this->Height)) {
			int Index = X + (Y * (this->Width));
			if (Index > 0 && Index < this->Width * this->Height) {
				((ULONG*)PixelsBGRA)[Index] = PixelData.Color;
			}
		}
	}
	__forceinline Vector2 TextSize(const wchar_t* Str)
	{
		if (!Str) return {};

		float Width = 0.f;

		for (int i = 0; Str[i]; i++) {
			if ((Str[i] >= '!' && Str[i] <= '~') || (Str[i] >= 1040 && Str[i] <= 1103)) {
				int GlyphIndex = (Str[i] >= 1040 && Str[i] <= 1103) ? Str[i] - 945 : Str[i] - '!';
				auto Glyph = &Font[GlyphIndex * 30];
				Width += (float)FontWidth[GlyphIndex];
			}
			else if (Str[i] == ' ') Width += 4.f;
		}

		return { Width, 15.f };
	}
	void String(Vector2 Start, const wchar_t* Str, ULONG Color = RGB(255, 255, 255), bool Center = false, bool Outline = false) {
		if (Center)
			Start.x -= TextSize(Str).x / 2;
		
		for (int i = 0; Str[i]; i++) {
			if ((Str[i] >= '!' && Str[i] <= '~') || (Str[i] >= 1040 && Str[i] <= 1103)) {
				int GlyphIndex = (Str[i] >= 1040 && Str[i] <= 1103) ? Str[i] - 945 : Str[i] - '!';

				auto Glyph = &Font[GlyphIndex * 30];
				auto GlyphOutline = &FontOutline[GlyphIndex * 30];

				for (int h = 0; h < 15; h++)
				{
					auto Row = ((WORD*)Glyph)[h];
					auto RowOutline = ((WORD*)GlyphOutline)[h];

					auto FirstByte = ((BYTE*)&Row)[1];
					((BYTE*)&Row)[1] = ((BYTE*)&Row)[0];
					((BYTE*)&Row)[0] = FirstByte;

					auto FirstByteOutline = ((BYTE*)&RowOutline)[1];
					((BYTE*)&RowOutline)[1] = ((BYTE*)&RowOutline)[0];
					((BYTE*)&RowOutline)[0] = FirstByteOutline;

						for (int w = 0; w < 16; w++) {
							bool Pixel = Row >> w & 1;
							bool PixelOutline = RowOutline >> w & 1;
							if (Pixel) SetPixel(static_cast<int>(Start.x + -w + 16), static_cast<int>(Start.y + h), Color);
							if (Outline &&  PixelOutline) SetPixel(static_cast<int>(Start.x + -w + 16), static_cast<int>(Start.y + h), RGB(1, 1, 1));
						}
				}

				Start.x += (float)FontWidth[GlyphIndex];
			}
			else if (Str[i] == ' ') Start.x += 4.f;
		}
	}
	void Line(Vector2 Start, Vector2 End, ULONG Color = RGB(255, 255, 255))
	{
		int x0 = static_cast<int>(Start.x); int y1 = static_cast<int>(Start.y);
		int x1 = static_cast<int>(End.x); int y0 = static_cast<int>(End.y);

		int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		int dy = -abs(y1 - y0), sy = y1 < y0 ? 1 : -1;
		int err = dx + dy, e2;

		for (;;) {
			SetPixel(x0, y1, Color);
			e2 = 2 * err;
			if (e2 >= dy) {
				if (x0 == x1) break;
				err += dy; x0 += sx;
			}
			if (e2 <= dx) {
				if (y1 == y0) break;
				err += dx; y1 += sy;
			}
		}
	}
	void FillRectangle(Vector2 Start, Vector2 Size, ULONG Color = RGB(255, 255, 255))
	{
		for (int x = 0; x < static_cast<int>(Size.x); x++)
		{
			for (int y = 0; y < static_cast<int>(Size.y); y++)
			{
				SetPixel(static_cast<int>(Start.x + x), static_cast<int>(Start.y + y), Color);
			}
		}
	}
	void Rectangle(Vector2 Start, Vector2 Size, ULONG Color = RGB(255, 255, 255))
	{
		FillRectangle({ Start.x, Start.y }, { Size.x, 1 }, Color);
		FillRectangle({ Start.x, Start.y + Size.y - 1 }, { Size.x, 1 }, Color);
		FillRectangle({ Start.x, Start.y + 1 }, { 1, Size.y - 2 * 1 }, Color);
		FillRectangle({ Start.x + Size.x - 1, Start.y + 1 }, { 1, Size.y - 2 * 1 }, Color);
	}
	void Circle(Vector2 Start, int Radius, ULONG Color = RGB(255, 255, 255))
	{
		int x = -Radius, y = 0, err = 2 - 2 * Radius;
		do {
			SetPixel(static_cast<int>(Start.x - x), static_cast<int>(Start.y + y), Color);
			SetPixel(static_cast<int>(Start.x - y), static_cast<int>(Start.y - x), Color);
			SetPixel(static_cast<int>(Start.x + x), static_cast<int>(Start.y - y), Color);
			SetPixel(static_cast<int>(Start.x + y), static_cast<int>(Start.y + x), Color);
			Radius = err;
			if (Radius <= y) err += ++y * 2 + 1;
			if (Radius > x || err > y) err += ++x * 2 + 1;
		} while (x < 0);
	}
	void Bitmap(ULONG* Bitmap, Vector2 Pos, Vector2 Size)
	{
		int Index = 0;
		for (int y = 0; y < static_cast<int>(Size.y); y++)
		{
			for (int x = 0; x < static_cast<int>(Size.x); x++)
			{
				ULONG Color = Bitmap[Index++];

				if (Color) SetPixel(static_cast<int>(Pos.x + x), static_cast<int>(Pos.y + y), Color);
			}
		}
	}
};