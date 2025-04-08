#pragma once

#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core.h"

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);