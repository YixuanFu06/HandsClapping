#pragma once

#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace AI {

template <typename T, unsigned int dim>
class Tensor {
 public:
  Tensor() = default;
  Tensor(const std::initializer_list<uint32_t> &lengths);
  Tensor(const std::vector<uint32_t> &lengths);
  Tensor(const Tensor &t);
  Tensor &operator=(const Tensor &t);
  Tensor operator+(const Tensor &t);
  Tensor operator-(const Tensor &t);
  Tensor operator*(const float f);
  Tensor operator*(const Tensor &t);
  Tensor &operator+=(const Tensor &t);
  Tensor &operator-=(const Tensor &t);
  Tensor &operator*=(const float f);
  Tensor &operator*=(const Tensor &t);

  Tensor<T, dim - 1> &operator[](unsigned int index);
  const Tensor<T, dim - 1> &operator[](unsigned int index) const;

  std::vector<uint32_t> GetSize();

 private:
  std::vector<Tensor<T, dim - 1>> sub_tensor;
};

template <typename T>
class Tensor<T, 1> {
 public:
  Tensor() = default;
  Tensor(const std::initializer_list<uint32_t> &lengths);
  Tensor(const std::vector<uint32_t> &lengths);
  Tensor(const Tensor &t);
  Tensor &operator=(const Tensor &t);
  Tensor operator+(const Tensor &t);
  Tensor operator-(const Tensor &t);
  Tensor operator*(const float f);
  Tensor operator*(const Tensor &t);
  Tensor &operator+=(const Tensor &t);
  Tensor &operator-=(const Tensor &t);
  Tensor &operator*=(const float f);
  Tensor &operator*=(const Tensor &t);

  T &operator[](unsigned int index);
  const T &operator[](unsigned int index) const;

  std::vector<uint32_t> GetSize();

 private:
  std::vector<T> data_;
};

template <typename T, unsigned int dim>
Tensor<T, dim>::Tensor(const std::initializer_list<uint32_t> &lengths) {
  if (lengths.size() != dim) {
    throw std::invalid_argument("Number of lengths must match the dimension.");
  }
  auto it = lengths.begin();
  sub_tensor.resize(*it);
  for (auto &sub : sub_tensor) {
    sub = Tensor<T, dim - 1>(std::vector<uint32_t>(it + 1, lengths.end()));
  }
}

template <typename T, unsigned int dim>
Tensor<T, dim>::Tensor(const std::vector<uint32_t> &lengths) {
  if (lengths.size() != dim) {
    throw std::invalid_argument("Number of lengths must match the dimension.");
  }
  auto it = lengths.begin();
  sub_tensor.resize(*it);
  for (auto &sub : sub_tensor) {
    sub = Tensor<T, dim - 1>(std::vector<uint32_t>(it + 1, lengths.end()));
  }
}

template <typename T, unsigned int dim>
Tensor<T, dim>::Tensor(const Tensor &t) : sub_tensor(t.sub_tensor) {
}

template <typename T, unsigned int dim>
Tensor<T, dim> &Tensor<T, dim>::operator=(const Tensor &t) {
  if (this != &t) {
    sub_tensor = t.sub_tensor;
  }
  return *this;
}

template <typename T, unsigned int dim>
Tensor<T, dim> Tensor<T, dim>::operator+(const Tensor &t) {
  Tensor<T, dim> result(this->GetSize());
  for (int i = 0; i < sub_tensor.size(); i++) {
    result.sub_tensor[i] = sub_tensor[i] + t.sub_tensor[i];
  }
  return result;
}

template <typename T, unsigned int dim>
Tensor<T, dim> Tensor<T, dim>::operator-(const Tensor &t) {
  Tensor<T, dim> result(this->GetSize());
  for (int i = 0; i < sub_tensor.size(); i++) {
    result.sub_tensor[i] = sub_tensor[i] - t.sub_tensor[i];
  }
  return result;
}

template <typename T, unsigned int dim>
Tensor<T, dim> Tensor<T, dim>::operator*(const float f) {
  Tensor<T, dim> result(this->GetSize());
  for (int i = 0; i < sub_tensor.size(); i++) {
    result.sub_tensor[i] = sub_tensor[i] * f;
  }
  return result;
}

template <typename T, unsigned int dim>
Tensor<T, dim> Tensor<T, dim>::operator*(const Tensor &t) {
  Tensor<T, dim> result(this->GetSize());
  for (int i = 0; i < sub_tensor.size(); i++) {
    result.sub_tensor[i] = sub_tensor[i] * t.sub_tensor[i];
  }
  return result;
}

template <typename T, unsigned int dim>
Tensor<T, dim> &Tensor<T, dim>::operator+=(const Tensor &t) {
  for (int i = 0; i < sub_tensor.size(); i++) {
    sub_tensor[i] += t.sub_tensor[i];
  }
  return *this;
}

template <typename T, unsigned int dim>
Tensor<T, dim> &Tensor<T, dim>::operator-=(const Tensor &t) {
  for (int i = 0; i < sub_tensor.size(); i++) {
    sub_tensor[i] -= t.sub_tensor[i];
  }
  return *this;
}

template <typename T, unsigned int dim>
Tensor<T, dim> &Tensor<T, dim>::operator*=(const float f) {
  for (int i = 0; i < sub_tensor.size(); i++) {
    sub_tensor[i] *= f;
  }
  return *this;
}

template <typename T, unsigned int dim>
Tensor<T, dim> &Tensor<T, dim>::operator*=(const Tensor &t) {
  for (int i = 0; i < sub_tensor.size(); i++) {
    sub_tensor[i] *= t.sub_tensor[i];
  }
  return *this;
}

template <typename T, unsigned int dim>
Tensor<T, dim - 1> &Tensor<T, dim>::operator[](unsigned int index) {
  return sub_tensor[index];
}

template <typename T, unsigned int dim>
const Tensor<T, dim - 1> &Tensor<T, dim>::operator[](unsigned int index) const {
  return sub_tensor[index];
}

template <typename T, unsigned int dim>
std::vector<uint32_t> Tensor<T, dim>::GetSize() {
  std::vector<uint32_t> result;
  result.push_back(sub_tensor.size());
  std::vector<uint32_t> sub_size = sub_tensor[0].GetSize();
  result.insert(result.end(), sub_size.begin(), sub_size.end());
  return result;
}

template <typename T>
Tensor<T, 1>::Tensor(const std::initializer_list<uint32_t> &lengths) {
  data_.resize(*lengths.begin());
}

template <typename T>
Tensor<T, 1>::Tensor(const std::vector<uint32_t> &lengths) {
  data_.resize(lengths[0]);
}

template <typename T>
Tensor<T, 1>::Tensor(const Tensor &t) : data_(t.data_) {
}

template <typename T>
Tensor<T, 1> &Tensor<T, 1>::operator=(const Tensor &t) {
  if (this != &t) {
    data_ = t.data_;
  }
  return *this;
}

template <typename T>
Tensor<T, 1> Tensor<T, 1>::operator+(const Tensor &t) {
  Tensor<T, 1> result(this->GetSize());
  for (int i = 0; i < data_.size(); i++) {
    result.data_[i] = data_[i] + t.data_[i];
  }
  return result;
}

template <typename T>
Tensor<T, 1> Tensor<T, 1>::operator-(const Tensor &t) {
  Tensor<T, 1> result(this->GetSize());
  for (int i = 0; i < data_.size(); i++) {
    result.data_[i] = data_[i] - t.data_[i];
  }
  return result;
}

template <typename T>
Tensor<T, 1> Tensor<T, 1>::operator*(const float t) {
  Tensor<T, 1> result(this->GetSize());
  for (int i = 0; i < data_.size(); i++) {
    result.data_[i] = data_[i] * t;
  }
  return result;
}

template <typename T>
Tensor<T, 1> Tensor<T, 1>::operator*(const Tensor &t) {
  Tensor<T, 1> result(this->GetSize());
  for (int i = 0; i < data_.size(); i++) {
    result.data_[i] = data_[i] * t.data_[i];
  }
  return result;
}

template <typename T>
Tensor<T, 1> &Tensor<T, 1>::operator+=(const Tensor &t) {
  for (int i = 0; i < data_.size(); i++) {
    data_[i] += t.data_[i];
  }
  return *this;
}

template <typename T>
Tensor<T, 1> &Tensor<T, 1>::operator-=(const Tensor &t) {
  for (int i = 0; i < data_.size(); i++) {
    data_[i] -= t.data_[i];
  }
  return *this;
}

template <typename T>
Tensor<T, 1> &Tensor<T, 1>::operator*=(const float f) {
  for (int i = 0; i < data_.size(); i++) {
    data_[i] *= f;
  }
  return *this;
}

template <typename T>
Tensor<T, 1> &Tensor<T, 1>::operator*=(const Tensor &t) {
  for (int i = 0; i < data_.size(); i++) {
    data_[i] *= t.data_[i];
  }
  return *this;
}

template <typename T>
std::vector<uint32_t> Tensor<T, 1>::GetSize() {
  return {static_cast<uint32_t>(data_.size())};
}

template <typename T>
T &Tensor<T, 1>::operator[](unsigned int index) {
  return data_[index];
}

template <typename T>
const T &Tensor<T, 1>::operator[](unsigned int index) const {
  return data_[index];
}

}  // namespace AI