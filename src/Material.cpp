#include "Material.hpp"

#include "resources/Shader.hpp"
#include "resources/Texture.hpp"

void Material::setTexture(StringId name, Texture* texture)
{
    if (auto* value = _textures.getPtr(name)) {
        *value = texture;
        return;
    }
	_textures.add(name, texture);
}

void Material::removeTexture(StringId name)
{
    _textures.remove(name);
}

void Material::apply(Shader& shader) const
{
	_data.apply(shader);

    for (int textureUnit = 0; textureUnit < _textures.size(); textureUnit++) {
        const auto location = shader.getLocation(_textures.keys()[textureUnit]);
        if (location == static_cast<unsigned int>(-1)) {
            continue;
        }

        _textures.values()[textureUnit]->apply(textureUnit);
        shader.setUniform(location, textureUnit);
    }
}

RenderData& Material::getMaterialData()
{
	return _data;
}

const RenderData& Material::getMaterialData() const
{
	return _data;
}
