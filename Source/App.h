#pragma once

#include <string>
#include <cstdint>

#include <Lib/Graphics/Graphics.h>
#include <Fwk/Asset/Asset.h>
#include <Fwk/Graphics.h>
#include <Fwk/Graphics/LineSegments.h>
#include <Fwk/Audio/WaveSound.h>
#include <Fwk/Audio/MP3Sound.h>
#include <Fwk/Audio/SoundSource.h>
#include <Fwk/File/CSVFile.h>
#include <Fwk/File/KeyValueFile.h>
#include <Lib/Math/Vector.h>
#include <Lib/Math/Math.h>
#include <Fwk/Framework.h>

#include "AppDef.h"

using namespace Lib::Graphics;
using namespace std;

class App
{
public:

	App() {}
	~App() { Term(); }

	void Init();
	void Term();

	void Update();
	void Render();

private:

};