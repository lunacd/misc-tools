#pragma once

#include <chrono>
#include <unordered_map>

namespace Lunacd::Util::ExpiringResource {
template <typename K, typename V, size_t maxDurationSeconds>
class ExpiringResource {
public:
  using item_iterator = std::unordered_map<K, V>::iterator;

  template <typename First, typename... Args>
  std::pair<item_iterator, bool> emplace(First &&key, Args &&...args) {
    prune();
    m_creationTime.emplace(key, std::chrono::steady_clock::now());
    return m_map.emplace(key, args...);
  }

  auto end() const { return m_map.end(); }

  auto find(const K &key) {
    prune();
    return m_map.find(key);
  }

private:
  size_t m_numOperations = 0;
  std::unordered_map<K, V> m_map;
  std::unordered_map<K, std::chrono::time_point<std::chrono::steady_clock>>
      m_creationTime;

  void prune() {
    m_numOperations += 1;
    if (m_numOperations <= m_map.size()) {
      return;
    }
    const auto expiringThreshold = std::chrono::steady_clock::now() +
                                   std::chrono::seconds{maxDurationSeconds};
    auto item_it = m_map.begin();
    auto time_it = m_creationTime.begin();
    while (item_it != m_map.end() && time_it != m_creationTime.end()) {
      if (time_it->second < expiringThreshold) {
        time_it = m_creationTime.erase(time_it);
        item_it = m_map.erase(item_it);
      }
      ++time_it;
      ++item_it;
    }
    m_numOperations = 0;
  }
};
} // namespace Lunacd::Util::ExpiringResource
