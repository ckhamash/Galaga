#include "SoundManager.h"

SoundManager::SoundManager() {
	latestSound = 0;
}

SoundManager::SoundManager(int numBuffers) : SoundManager::SoundManager() {
	for (int i = 0; i < numBuffers; i++)
		soundBuffers.push_back(sf::SoundBuffer());
}

SoundManager::~SoundManager() { }

void SoundManager::setSoundBuffer(SoundID id, std::string filename) {
	sf::SoundBuffer buffer;
	buffer.loadFromFile(filename);
	soundBuffers[(int)id] = sf::SoundBuffer(buffer);
}

void SoundManager::playSound(SoundID id) {
	sounds[latestSound].setBuffer(soundBuffers[(int)id]);
	sounds[latestSound].play();
	latestSound++;
	if (latestSound > sounds.size() - 1)
		latestSound = 0;
}
