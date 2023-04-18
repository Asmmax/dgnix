#include "objects/Light.hpp"
#include "DrawState.hpp"
#include <string>

int Light::MAX_LIGHT_COUNT = 10;

Light::LightNames::LightNames(int id):
	_id(id),
	_colorName(("PointLights[" + std::to_string(id) + "].Color").c_str()),
	_intensityName(("PointLights[" + std::to_string(id) + "].Intensity").c_str()),
	_radiusName(("PointLights[" + std::to_string(id) + "].Radius").c_str()),
	_fadingAreaName(("PointLights[" + std::to_string(id) + "].FadingArea").c_str())
{
}

std::vector<StringId>& Light::LightNames::getLightPosNames()
{
	static std::vector<StringId> lightPosNames = initLightPosNames();
	return lightPosNames;
}

std::vector<StringId> Light::LightNames::initLightPosNames()
{
	std::vector<StringId> lightPosNames;
	static std::string nameTemplate = "PointLights";

	for (int i = 0; i < Light::MAX_LIGHT_COUNT; i++) {
		std::string name(nameTemplate);
		name += "[" + std::to_string(i) + "]";
		lightPosNames.emplace_back((name + ".Position").c_str());
	}

	return lightPosNames;
}




Light::Light():
	_position(),
	_color(1.0f, 1.0f, 1.0f),
	_intensity(1.0f),
	_radius(100.0f),
	_fadingArea(0.0f)
{
}

void Light::setColor(const glm::vec3& color)
{
	_color = color;
}

void Light::setIntensity(float intensity)
{
	_intensity = intensity;
}

void Light::setRadius(float radius)
{
	_radius = radius;
}

void Light::setFadingArea(float fadingArea)
{
	_fadingArea = fadingArea;
}

void Light::setPosition(const glm::vec3& position)
{
	_position = position;
}

void Light::predraw(DrawStatePoolDef& statePool)
{
	auto& state = statePool.get();

	static StringId viewMatrixName = StringId("ViewMatrix");
	auto& viewMatrix = state.get<glm::mat4>(viewMatrixName);
	glm::vec4 viewPosition = viewMatrix * glm::vec4(_position, 1.0f);

	int id = nextFreeId(state);
	if (id < 0) {
		return;
	}

	if (!_names || _names && _names->getId() != id) {
		_names = std::make_unique<LightNames>(id);
	}

	state.add(LightNames::getLightPosNames()[id], viewPosition);
	state.add(_names->getColorName(), _color);
	state.add(_names->getIntensityName(), _intensity);
	state.add(_names->getRadiusName(), _radius);
	state.add(_names->getFadingAreaName(), _fadingArea);

	static StringId pointLightCountName = StringId("PointLightCount");
	state.addOrSet(pointLightCountName, id + 1);
}

int Light::nextFreeId(const DrawStateDef& state) const
{
	for (int i = 0; i < MAX_LIGHT_COUNT; i++) {

		if (!state.has(LightNames::getLightPosNames()[i])) {
			return i;
		}
	}
	return -1;
}
