#include <OaRelaySseScanner.hpp>

#include <cassert>

namespace Lunacd::OaRelay {
void SseScanner::addInput(const char *inputBuffer, size_t n) {
  bool newEvent = false;
  std::string newInput = std::string(inputBuffer, n);
  m_buffer += newInput;
  for (; m_currentIndex < m_buffer.length(); m_currentIndex += 1) {
    if (m_buffer[m_currentIndex] == '\n') {
      if (m_prevIsLf) {
        // new sse event
        m_events.emplace(m_buffer.substr(0, m_currentIndex - 1));
        m_buffer.erase(0, m_currentIndex + 1);
        m_currentIndex = 0;
        m_prevIsLf = false;
        newEvent = true;
      } else {
        m_prevIsLf = true;
      }
    }
  }
  if (newEvent) {
    cv.notify_one();
  }
}

std::string SseScanner::getEvent() {
  assert(!m_events.empty());
  std::string front = std::move(m_events.front());
  m_events.pop();
  return front;
}

bool SseScanner::hasEvent() { return !m_events.empty(); }
} // namespace Lunacd::OaRelay
