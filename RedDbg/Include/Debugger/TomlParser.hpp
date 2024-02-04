#pragma once
#include "toml++/toml.hpp"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include <filesystem>

class TomlParser_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	//GlobalClassVars_ GlobalClassVars;
	//Window_ Window;
public:
	TomlParser_();
};