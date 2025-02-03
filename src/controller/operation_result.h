#ifndef FACADEOPERATIONRESULT_H
#define FACADEOPERATIONRESULT_H

#include <string>

namespace s21 {

class FacadeOperationResult {
 public:
  FacadeOperationResult(const std::string em, bool is)
      : err_message(em), is_success(is) {}
  FacadeOperationResult() : err_message(), is_success(true) {};

  inline std::string GetErrorMessage() noexcept { return err_message; };
  inline bool IsSuccess() noexcept { return is_success; };

 private:
  const std::string err_message;
  bool is_success;
};
}  // namespace s21
#endif  // FACADEOPERATIONRESULT_H
