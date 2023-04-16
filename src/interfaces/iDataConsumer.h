#pragma once

#include <memory>
#include <string>

template <typename T>
class iDataConsumer {
public:
	virtual ~iDataConsumer() = default;
	virtual void update(const std::string& name, const T& value) = 0;
};
