#include "objects/DirectLight.hpp"
#include "DrawState.hpp"
#include <string>

int DirectLight::MAX_LIGHT_COUNT = 4;

DirectLight::LightNames::LightNames(int id):
	_id(id),
	_colorName(("DirectLights[" + std::to_string(id) + "].Color").c_str()),
	_intensityName(("DirectLights[" + std::to_string(id) + "].Intensity").c_str())
{
}

std::vector<StringId>& DirectLight::LightNames::getLightDirNames()
{
	static std::vector<StringId> lightDirNames = initLightDirNames();
	return lightDirNames;
}

std::vector<StringId> DirectLight::LightNames::initLightDirNames()
{
	std::vector<StringId> lightDirNames;
	static const std::string nameTemplate = "DirectLights";

	for (int i = 0; i < DirectLight::MAX_LIGHT_COUNT; i++) {
		std::string name(nameTemplate);
		name += "[" + std::to_string(i) + "]";
		lightDirNames.emplace_back((name + ".Direction").c_str());
	}

	return lightDirNames;
}




DirectLight::DirectLight():
	_direction(0.f, -1.f, 0.f),
	_color(1.f, 1.f, 1.f),
	_intensity(1.f)
{
}

void DirectLight::setColor(const glm::vec3& color)
{
	_color = color;
}

void DirectLight::setIntensity(float intensity)
{
	_intensity = intensity;
}

void DirectLight::setDirection(const glm::vec3& dir) 
{
	_direction = dir;
}

void DirectLight::predraw(DrawStatePoolDef& statePool)
{
	auto& state = statePool.get();

	static const StringId viewMatrixName = StringId("ViewMatrix");
	const auto& viewMatrix = state.get<glm::mat4>(viewMatrixName);
	const glm::vec4 viewDirection = viewMatrix * glm::vec4(_direction, 0.0f);

	const int id = nextFreeId(state);
	if (id < 0) {
		return;
	}

	if (!_names || (_names && _names->getId() != id)) {
		_names = std::make_unique<LightNames>(id);
	}

	state.add(LightNames::getLightDirNames()[id], viewDirection);
	state.add(_names->getColorName(), _color);
	state.add(_names->getIntensityName(), _intensity);

	static const StringId directLightCountName = StringId("DirectLightCount");
	state.addOrSet(directLightCountName, id + 1);
}

int DirectLight::nextFreeId(const DrawStateDef& state)
{
	for (int i = 0; i < MAX_LIGHT_COUNT; i++) {

		if (!state.has(LightNames::getLightDirNames()[i])) {
			return i;
		}
	}
	return -1;
}
