#ifndef ELEMENT_CPP
#define ELEMENT_CPP

class Element {
public:
	u32 id;
	u32 color;

	Vector vel;

	Element() : id(0), color(0) {}

	Element(u32 id, u32 color) : id(id), color(color) {}

	virtual u32 getColor() { return 0xff0000; }

	virtual Element* clone() { return new Element(id, color); }

	virtual Vector update(s32 x, s32 y) { return Vector(x, y); }

	virtual b32 isDynamic() { return true; }
};

#endif // ELEMENT_CPP
