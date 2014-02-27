#pragma once

class PerlinNoise{

public:
	// Constructor
	PerlinNoise();
	PerlinNoise(float _persistence, float _frequency, float _amplitude, int _octaves, int _randomseed);

	// Get Height
	float getHeight(float x, float y) const;

	// Get
	float getPersistence() const;
	float getFrequency() const;
	float getAmplitude() const;
	int getOctaves() const;
	int getRandomSeed() const;

	// Set
	void set(float _persistence, float _frequency, float _amplitude, int _octaves, int _randomseed);
	void setPersistence(float _persistence);
	void setFrequency(float _frequency);
	void setAmplitude(float _amplitude);
	void setOctaves(int _octaves);
	void setRandomSeed(int _randomseed);

private:
	float total(float i, float j) const;
	float getValue(float x, float y) const;
	float interpolate(float x, float y, float a) const;
	float noise(int x, int y) const;

	float persistence, frequency, amplitude;
	int octaves, randomseed;
};
