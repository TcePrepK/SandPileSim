#define STONE 3

class Stone : public Solid {
public:
	Stone() : Solid(STONE, getColor()) {}

	u32 getColor() {
		f32 r = (f32)(randomVal(0, 1)) / 2.f;
		s32 red = (s32)(lerp(0.5f, 0.65f, r) * 255.f);
		s32 green = (s32)(lerp(0.5f, 0.65f, r) * 255.f);
		s32 blue = (s32)(lerp(0.5f, 0.65f, r) * 255.f);
		s32 alpha = 255;
		return rgbaToHex(red, green, blue, alpha);
	}

	Element* clone() { return new Stone(); }
};