#ifndef WORLD_CPP
#define WORLD_CPP

class World {
public:
	s32 chunkWidth = 64;
	s32 chunkHeight = 64;
	unordered_map<pair<s32, s32>, Chunk*, pair_hash> chunks;
	vector<Chunk*> chunkList;

	World() {}

	void prepareUpdate() {
		for (s32 i = 0; i < chunkList.size(); i++) {
			Chunk* chunk = chunkList[i];
			pair<s32, s32> location = getChunkLocation(chunk->x, chunk->y);

			if (chunk->filledPixelAmount == 0) {
				chunks.erase(location);
				chunkList[i] = chunkList.back();
				chunkList.pop_back();
				i--;

				delete chunk;
				continue;
			}

			chunk->prepareUpdate();
		}
	}

	void update() {
		prepareUpdate();
		for (Chunk* chunk : chunkList) {
			for (s32 offX = 0; offX < chunkWidth; offX++) {
				for (s32 offY = 0; offY < chunkHeight; offY++) {
					Vector from = { offX, offY };
					Vector pos = chunk->getIndex(offX, offY);

					Element* e = chunk->getPixel(offX, offY);
					if (e == nullptr || e->updatedThisFrame) continue;

					e->updatedThisFrame = true;

					Vector newPos = e->update(pos.x, pos.y);
					// Chunk* toChunk = getChunkWithTiles(newPos.x, newPos.y);
					// Vector to = newPos - Vector(toChunk->tileX, toChunk->tileY);

					/*
					if (newPos.x == pos.x && newPos.y == pos.y)
						continue;

					chunk->movePixel(toChunk, from, to);
					*/
				}
			}
		}

		for (Chunk* chunk : chunkList) {
			chunk->updateChanges();
		}
	}

	void render() {
		for (Chunk* chunk : chunkList) {
			chunk->render();
		}

		for (GUI* gui : GUIs) {
			Rect pos = gui->pos;
			fillRectWithoutScale(pos.left, pos.top, pos.right - pos.left, pos.bottom - pos.top, gui->color);
		}
	}

	// HELPERS

	pair<s32, s32> getChunkLocation(s32 x, s32 y) {
		return { floor((f32)x / (f32)chunkWidth), floor((f32)y / (f32)chunkHeight) };
	}

	Chunk* createChunk(pair<s32, s32> location) {
		Chunk* c = new Chunk(location.first, location.second, chunkWidth, chunkHeight);

		chunks.insert({ location, c });
		chunkList.push_back(c);

		return c;
	}

	Chunk* getChunkDirect(pair<s32, s32> location) {
		auto itr = chunks.find(location);
		auto end = chunks.end();
		return itr != end ? itr->second : nullptr;
	}

	Chunk* getChunk(pair<s32, s32> location) {
		Chunk* chunk = getChunkDirect(location);
		return chunk == nullptr ? createChunk(location) : chunk;
	}

	Chunk* getChunk(s32 x, s32 y) {
		pair<s32, s32> location = getChunkLocation(x, y);
		return getChunk(location);
	}

	Element* getPixel(s32 x, s32 y) {
		pair<s32, s32> location = getChunkLocation(x, y);
		Chunk* chunk = getChunk(location);
		Vector pos = chunk->getWorldIndex(x, y);
		return chunk->getPixel(pos.x, pos.y);
	}

	Element* getPixelDirect(s32 x, s32 y) {
		pair<s32, s32> location = getChunkLocation(x, y);
		Chunk* chunk = getChunkDirect(location);
		if (chunk == nullptr) return nullptr;
		Vector pos = chunk->getWorldIndex(x, y);
		return chunk->getPixel(pos.x, pos.y);
	}

	void setPixel(s32 x, s32 y, Element* element) {
		pair<s32, s32> location = getChunkLocation(x, y);
		Chunk* chunk = getChunk(location);
		Vector pos = chunk->getWorldIndex(x, y);
		chunk->setPixel(pos.x, pos.y, element);
	}

	void setCirclePixel(s32 x, s32 y, s32 r, Element* e) {
		s32 sqrR = r * r;
		for (s32 offX = -r; offX < r; offX++) {
			for (s32 offY = -r; offY < r; offY++) {
				s32 sqrLen = offX * offX + offY * offY;
				if (sqrLen > sqrR)
					continue;
				setPixel(x + offX, y + offY, e->clone());
			}
		}

		delete e;
	}

	void setCircleLine(s32 fromX, s32 fromY, s32 toX, s32 toY, s32 r, Element* e) {
		if (toX < fromX) {
			s32 temp = toX;
			toX = fromX;
			fromX = temp;
		}
		if (toY < fromY) {
			s32 temp = toY;
			toY = fromY;
			fromY = temp;
		}

		s32 dx = toX - fromX;
		s32 dy = toY - fromY;

		if (dx == 0 && dy == 0) {
			setCirclePixel(fromX, fromY, r, e->clone());
			return;
		}

		s32 step = (dx > dy) ? dx : dy;

		s32 xin = dx / step;
		s32 yin = dy / step;

		s32 x = fromX;
		s32 y = fromY;
		for (s32 i = 0; i < step; i++) {
			x += xin;
			y += yin;

			setCirclePixel(x, y, r, e->clone());
		}

		delete e;
	}
};

World world = World();

#endif // WORLD_CPP