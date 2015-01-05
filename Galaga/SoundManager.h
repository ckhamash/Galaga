#pragma once
#include "SFML\Audio.hpp"
#include <vector>
#include <array>

enum class SoundID { ShipDestroy, LaserFire, LevelUp };

class SoundManager {
private:
	int latestSound;
	std::array<sf::Sound, 64> sounds;
	std::vector<sf::SoundBuffer> soundBuffers;
public:
	SoundManager();
	SoundManager(int numBuffers);
	~SoundManager();

	void setSoundBuffer(SoundID id, std::string filename);
	void playSound(SoundID id);
};