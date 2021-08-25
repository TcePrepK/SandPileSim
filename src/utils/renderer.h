void scale(s32 valX, s32 valY);
void clearScreen(u32 color);

void fillPixel(u32 x, u32 y, u32 color);
void fillRect(s32 x, s32 y, s32 w, s32 h, u32 color);
void fillRectWithoutScale(s32 x, s32 y, s32 w, s32 h, u32 color);
void strokeRect(s32 x, s32 y, s32 w, s32 h, u32 color);

void fillElement(s32 x, s32 y, u32 color);
Vector mouseToTile(Vector pos32);
b32 inBounds(s32 x, s32 y);

s32 scaleX = 1;
s32 scaleY = 1;

void scale(s32 valX, s32 valY) {
	scaleX = valX;
	scaleY = valY;
}

void clearScreen(u32 color) {
	for (u32 x = 0; x < globalVariables.screenWidth; x++)
		for (u32 y = 0; y < globalVariables.screenHeight; y++)
			fillPixel(x, y, color);
}

void fillPixel(u32 x, u32 y, u32 color) {
	u32 idx = x + y * globalVariables.screenWidth;
	globalVariables.pixels[idx] = color;
}

void fillRect(s32 x, s32 y, s32 w, s32 h, u32 color) {
	x *= scaleX;
	y *= scaleY;
	w *= scaleX;
	h *= scaleY;

	if (!inBounds(x, y))
		return;

	w = clamp(w, 0, globalVariables.screenWidth - x);
	h = clamp(h, 0, globalVariables.screenHeight - y);

	for (s32 i = 0; i < w; i++) {
		for (s32 j = 0; j < h; j++) {
			s32 curX = x + i;
			s32 curY = y + j;
			fillPixel(curX, curY, color);
		}
	}
}

void fillRectWithoutScale(s32 x, s32 y, s32 w, s32 h, u32 color) {
	if (!inBounds(x, y))
		return;

	w = clamp(w, 0, globalVariables.screenWidth - x);
	h = clamp(h, 0, globalVariables.screenHeight - y);

	for (s32 i = 0; i < w; i++) {
		for (s32 j = 0; j < h; j++) {
			fillPixel(x + i, y + j, color);
		}
	}
}

void strokeRect(s32 x, s32 y, s32 w, s32 h, u32 color) {
	x *= scaleX;
	y *= scaleY;
	w *= scaleX;
	h *= scaleY;

	if (!inBounds(x, y))
		return;

	w = clamp(w, 0, globalVariables.screenWidth - x);
	h = clamp(h, 0, globalVariables.screenHeight - y);

	for (s32 i = 0; i < w; i++) {
		fillPixel(x + i, y, color);
		fillPixel(x + i, y + (h - 1), color);
	}

	for (s32 i = 0; i < h; i++) {
		fillPixel(x, y + i, color);
		fillPixel(x + (w - 1), y + i, color);
	}
}

void fillElement(s32 x, s32 y, u32 color) {
	x *= scaleX;
	y *= scaleY;

	if (!inBounds(x, y))
		return;

	s32 w = scaleX;
	s32 h = scaleY;

	w = clamp(w, 0, globalVariables.screenWidth - x);
	h = clamp(h, 0, globalVariables.screenHeight - y);

	for (s32 i = 0; i < w; i++) {
		for (s32 j = 0; j < h; j++) {
			s32 curX = x + i;
			s32 curY = y + j;
			fillPixel(curX, curY, color);
		}
	}
}

Vector mouseToTile(Vector pos32) {
	Vector final = {};

	s32 x = pos32.x;
	s32 y = pos32.y;

	final.x = x / scaleX;
	final.y = y / scaleY;

	return final;
}

b32 inBounds(s32 x, s32 y) {
	s32 width = globalVariables.screenWidth;
	s32 height = globalVariables.screenHeight;
	return (x >= 0 && x < width&& y >= 0 && y < height);
}