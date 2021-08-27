#ifndef CHUNK_CPP
#define CHUNK_CPP

class Chunk {
public:
	s32 x, y, tileX, tileY;
	u32 width, height;
	string name;

	u32 filledPixelAmount = 0;

	Element** grid;
	vector<tuple<Chunk*, Vector, Vector>> changes;

	Chunk(s32 x, s32 y, u32 width, u32 height) : x(x), y(y), tileX(x* width), tileY(y* height), width(width), height(height) {
		grid = new Element * [(s32)(width * height)]{};
	}

	~Chunk() {
		delete[] grid;
	}

	void updateChanges() {
		for (s32 i = 0; i < changes.size(); i++) {
			auto change = changes[i];
			Chunk* toC = get<0>(change);
			Vector to = get<2>(change);
			if (toC->getPixel(to.x, to.y) != nullptr) {
				changes[i] = changes.back();
				changes.pop_back();
				i--;
			}
		}

		while (changes.size() != 0) {
			s32 r = rand() % changes.size();
			auto& change = changes[r];

			Chunk* toC = get<0>(change);
			Vector from = get<1>(change);
			Vector to = get<2>(change);

			toC->setPixel(to.x, to.y, getPixel(from.x, from.y));
			setPixel(from.x, from.y, nullptr);

			changes[r] = changes.back();
			changes.pop_back();
		}
	}

	void render() {
		for (u32 i = 0; i < width; i++) {
			for (u32 j = 0; j < height; j++) {
				Element* e = getPixel(i, j);

				u32 color = 0x000000;
				if (e != nullptr)
					color = e->color;

				fillElement(i + tileX, j + tileY, color);
			}
		}

		if (filledPixelAmount > 0) {
			strokeRect(tileX, tileY, width, height, 0x00ff00);
		} else {
			strokeRect(tileX, tileY, width, height, 0x000000);
		}
	}

	// HELPERS

	Vector getIndex(s32 x, s32 y) {
		return { tileX + x, tileY + y };
	}

	Vector getWorldIndex(s32 x, s32 y) {
		return { x - tileX, y - tileY };
	}

	Element* getPixel(u32 x, u32 y) {
		return grid[x + y * width];
	}

	void movePixel(Chunk* toC, Vector from, Vector to) {
		changes.emplace_back(toC, from, to);
	}

	void setPixel(s32 x, s32 y, Element* element) {
		Element* dest = getPixel(x, y);

		grid[x + y * width] = element;

		if (element != nullptr && dest == nullptr) {
			filledPixelAmount++;
		} else if (dest != nullptr && element == nullptr) {
			filledPixelAmount--;
		}
	}
};

#endif // CHUNK_CPP