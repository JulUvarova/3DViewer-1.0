#pragma once

#include "obj/obj_data.h"

namespace s21 {

/**
 * @class FileReader
 * @brief Provides functionality to read and process OBJ files.
 *
 * The `FileReader` class offers a simple interface for reading OBJ files. It
 * encapsulates the logic for parsing the file, normalizing the data, and
 * returning the processed information as an `OBJData` object.
 */
class FileReader {
 public:
  /**
   * @brief Reads and processes an OBJ file from the specified path.
   * @param path The file path to the OBJ file to be read.
   * @return An `OBJData` object containing the parsed and normalized data from
   * the OBJ file.
   *
   * This method performs the following steps:
   * - Parses the OBJ file located at the given path.
   * - Normalizes the parsed data to ensure it is suitable for rendering or
   * further processing.
   * - Returns the resulting `OBJData` object.
   */
  OBJData ReadFile(const char *path) {
    OBJData data;
    data.Parse(path);
    data.Normalize();
    return data;
  }
};
}  // namespace s21
