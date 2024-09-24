#pragma once

class visitor;

namespace ast {

/**
 * @class node
 * @brief
 *
 */
class node {
public:
  node() {};
  /**
   * @brief
   */
  virtual ~node() {};

  /**
   * @brief
   *
   * @param visitor
   */
  virtual void accept(visitor &visitor) = 0;
};
} // namespace ast
