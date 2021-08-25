#ifndef LIQUID_CPP
#define LIQUID_CPP

class Liquid : public Element {
public:
	Liquid(u32 id, u32 color) : Element(id, color) {}

	virtual Vector update(s32 x, s32 y) {
		return { x, y };
	}
};

#endif // LIQUID_CPP