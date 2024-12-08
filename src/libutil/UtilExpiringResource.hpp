#pragma once

#include <chrono>
#include <functional>
#include <unordered_map>

namespace Lunacd::Util::ExpiringResource {
/**
 * @brief This class helps manages resources that could expire. After a resource
 * expires, the given cleanup callback is called with the expired key.
 *
 * @details It's not guaranteed when the callback will be called, only that it
 * will eventually be called. In this current implementation, elements are
 * checked for expiration when the container doubles in size. That is, when
 * expiring resources are being managed by this class, expired resources are
 * guaranteed to not overflow resources when the load factor is below 50%.
 *
 * @tparam K Key for the resources being managed
 */
template <typename K> class ExpiringResource {
public:
  using cleanup_func_t = std::function<void(const K &key)>;

  ExpiringResource(size_t maxDurationSeconds, cleanup_func_t cleanupFunc)
      : m_maxDurationSeconds{maxDurationSeconds}, m_cleanupFunc{cleanupFunc} {}

  void insert(const K &key) {
    // Add two, one to account for the added element, another to "mark" an
    // existing element as needing to be checked.
    m_numOperations += 2;
    prune();
    m_creationTime.emplace(key, std::chrono::steady_clock::now());
  }

  void erase(const K &key) {
    m_cleanupFunc(key);
    m_creationTime.erase(key);
  }

private:
  size_t m_numOperations = 0;
  size_t m_maxDurationSeconds = 600;
  std::unordered_map<K, std::chrono::time_point<std::chrono::steady_clock>>
      m_creationTime;
  cleanup_func_t m_cleanupFunc;

  void prune() {
    if (m_numOperations <= m_creationTime.size()) {
      return;
    }
    const auto expiringThreshold = std::chrono::steady_clock::now() -
                                   std::chrono::seconds{m_maxDurationSeconds};
    auto it = m_creationTime.begin();
    while (it != m_creationTime.end()) {
      if (it->second < expiringThreshold) {
        m_cleanupFunc(it->first);
        it = m_creationTime.erase(it);
      } else {
        ++it;
      }
    }
    m_numOperations = 0;
  }
};
} // namespace Lunacd::Util::ExpiringResource
