#pragma once

#include <stdio.h>
#include <algorithm>
#include <fstream>

#include "core.hpp"
#include "shared/pch.hpp"

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);