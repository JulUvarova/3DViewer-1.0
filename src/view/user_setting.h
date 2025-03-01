#pragma once

#include <QColor>
#include <QSettings>
#include <QString>

/**
 * @class UserSetting
 * @brief A class for managing user settings related to rendering.
 *
 * This class provides methods to save, read, and remove user settings for
 * rendering parameters such as vertices and edges properties, background color,
 * and projection type.
 */
class UserSetting {
 public:
  /**
   * @brief Constructor for the UserSetting class.
   *
   * Initializes the user settings by reading the render settings from the file.
   */
  UserSetting();

  /**
   * @brief Saves the current render settings to a file.
   *
   * This method saves the vertices type, color, size, edges type, color, size,
   * background color, and projection type to the settings file.
   */
  void SaveRenderSettings();

  /**
   * @brief Reads the render settings from a file.
   *
   * This method loads the vertices type, color, size, edges type, color, size,
   * background color, and projection type from the settings file.
   */
  void ReadRenderSettings();

  /**
   * @brief Removes the render settings from the file.
   *
   * This method deletes the render settings group from the settings file.
   */
  void RemoveRenderSettings();

  /**
   * @brief Checks if the current projection is parallel.
   *
   * @return True if the projection is parallel, false otherwise.
   */
  inline bool IsParallelProjectrion() const { return isParallelProjection_; }

  /**
   * @brief Sets the projection type.
   *
   * @param isParallel True for parallel projection, false for perspective.
   */
  inline void SetProjection(bool isParallel) {
    isParallelProjection_ = isParallel;
  }

  /**
   * @brief Gets the background color.
   *
   * @return The current background color.
   */
  inline QColor GetBackgroundColor() const { return backgroundColor_; }

  /**
   * @brief Sets the background color.
   *
   * @param backgroundColor The new background color to set.
   */
  inline void SetBackgroundColor(const QColor &backgroundColor) {
    backgroundColor_ = backgroundColor;
  }

  /**
   * @brief Gets the type of vertices.
   *
   * @return The current vertices type.
   */
  inline QString GetVerticesType() const { return verticesType_; }

  /**
   * @brief Sets the type of vertices.
   *
   * @param verticesType The new vertices type to set.
   */
  inline void SetVerticesType(const QString &verticesType) {
    verticesType_ = verticesType;
  }

  /**
   * @brief Gets the color of vertices.
   *
   * @return The current vertices color.
   */
  inline QColor GetVerticesColor() const { return verticesColor_; }

  /**
   * @brief Sets the color of vertices.
   *
   * @param verticesColor The new vertices color to set.
   */
  inline void SetVerticesColor(const QColor &verticesColor) {
    verticesColor_ = verticesColor;
  }

  /**
   * @brief Gets the size of vertices.
   *
   * @return The current vertices size.
   */
  inline int GetVerticesSize() const { return verticesSize_; }

  /**
   * @brief Sets the size of vertices.
   *
   * @param verticesSize The new vertices size to set.
   */
  inline void SetVerticesSize(int verticesSize) {
    verticesSize_ = verticesSize;
  }

  /**
   * @brief Gets the type of edges.
   *
   * @return The current edges type.
   */
  inline QString GetEdgesType() const { return edgesType_; }

  /**
   * @brief Sets the type of edges.
   *
   * @param edgesType The new edges type to set.
   */
  inline void SetEdgesType(const QString &edgesType) { edgesType_ = edgesType; }

  /**
   * @brief Gets the color of edges.
   *
   * @return The current edges color.
   */
  inline QColor GetEdgesColor() const { return edgesColor_; }

  /**
   * @brief Sets the color of edges.
   *
   * @param edgesColor The new edges color to set.
   */
  inline void SetEdgesColor(const QColor &edgesColor) {
    edgesColor_ = edgesColor;
  }

  /**
   * @brief Gets the size of edges.
   *
   * @return The current edges size.
   */
  inline int GetEdgesSize() const { return edgesSize_; }

  /**
   * @brief Sets the size of edges.
   *
   * @param edgesSize The new edges size to set.
   */
  inline void SetEdgesSize(int edgesSize) { edgesSize_ = edgesSize; }

 private:
  const QString fileMemory_{
      "view/settings/usersettings.xml"};  ///< File path for saving user
                                          ///< settings

  QString verticesType_;  ///< Type of vertices (e.g., "circle", "square")
  QColor verticesColor_;  ///< Color of vertices
  int verticesSize_;      ///< Size of vertices

  QString edgesType_;  ///< Type of edges (e.g., "solid", "dashed")
  QColor edgesColor_;  ///< Color of edges
  int edgesSize_;      ///< Size of edges

  QColor backgroundColor_;  ///< Background color of the scene

  bool
      isParallelProjection_;  ///< Flag indicating if the projection is parallel
};