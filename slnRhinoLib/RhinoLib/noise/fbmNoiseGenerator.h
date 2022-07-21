#ifndef FBMNOISEGENERATOR_H
#define FBMNOISEGENERATOR_H

#include <memory>
#include <vector>
#include <math.h>

#include "SimplexNoise.h"

namespace RhinoLib
{
	struct FbmNoiseSettings
	{
		int octaves{ 6 };
		float redistribution{ 3.f };
		float noiseScale{ 0.015f };
		float startFrequency{ 1.f };
		float startAmplitude{ 1.f };
	};

	class FbmNoiseGenerator
	{
	private:
		std::unique_ptr<RhinoLib::OpenSimplexNoise> _simplex;

		float _octaveOffset = 0.5f;
		int _octaves{ 6 };
		float _redistribution{ 3.f };
		float _noiseScale{ 0.015f };

	public:
		FbmNoiseGenerator(long seed)
		{
			_simplex = std::make_unique<RhinoLib::OpenSimplexNoise>(seed);
		}

		void generateNoise(
			std::vector<float>& heightmap,
			const FbmNoiseSettings& settings,
			const unsigned int width, const unsigned int height,
			const float startX = 0.f, const float startY = 0.f
		)
		{
			heightmap.clear();

			for (unsigned int y = 0; y < height; ++y)
			{
				for (unsigned int x = 0; x < width; ++x)
				{
					float xx{ (startX + x) * _noiseScale };
					float yy{ (startY + y) * _noiseScale };

					float height = getNoiseAt(xx, yy);
					heightmap.emplace_back(height);
				}
			}
		}

		float getNoiseAt(float x, float y)
		{
			float noise{ 0.f };

			float frequency{ 1.f };
			float amplitude{ 1.f };
			float sumAmplitude{ 0.f };

			for (int octave = 0; octave < _octaves; ++octave)
			{
				noise += (float)_simplex->eval(
					frequency * (x + (_octaveOffset * octave)),
					frequency * (y + (_octaveOffset * octave))
				) * amplitude;

				sumAmplitude += amplitude;
				frequency *= 2.f;
				amplitude *= 0.5f;
			}

			noise /= sumAmplitude;
			noise = (noise + 1.f) / 2.f; // map between 0 and 1
			noise = std::powf(noise * 1.2f, _redistribution);

			if (noise > 1.f) noise = 1.f;
			if (noise < 0.f) noise = 0.f;

			return noise;
		}
	};
}
#endif