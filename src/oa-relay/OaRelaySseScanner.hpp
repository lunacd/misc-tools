#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

namespace Lunacd::OaRelay {
class SseScanner {
public:
  void addInput(const char *inputBuffer, size_t n);
  bool hasEvent();
  std::string getEvent();

  std::mutex mutex;
  std::condition_variable cv;

private:
  std::string m_buffer;
  std::queue<std::string> m_events;
  size_t m_currentIndex;
  bool m_prevIsLf;
};
} // namespace Lunacd::OaRelay
