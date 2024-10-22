#pragma once

namespace renderer {

class renderer {
private:
	renderer();

	void setup();

public:
	static inline renderer* get() {
		static renderer* _i = new renderer();
		return _i;
	}

	void update();
	void render();
};

} // namespace renderer
