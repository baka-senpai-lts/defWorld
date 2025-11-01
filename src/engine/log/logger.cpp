#include "logger.hpp"

#include <cstdio>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <iostream>
#include <mutex>

namespace engine::log {

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

Logger::~Logger() {
  die = true;
  cv.notify_all();

  if (thread.joinable())
    thread.join();
}

void Logger::run() {
  std::unique_lock<std::mutex> lock(messages_mtx);

  while (true) {
    cv.wait(lock, [this] { return die || !messages.empty(); });

    if (die && messages.empty())
      break;

    Message msg = std::move(messages.front());
    messages.pop();

    lock.unlock();
    printMessage(msg);
    lock.lock();
  }
}

void Logger::printMessage(Message msg) {
  auto t = std::chrono::system_clock::to_time_t(msg.timestamp);

  std::tm tm;

#ifdef _WIN32
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif

  std::string str = fmt::v10::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}\n", tm,
                                     getLogLevelPrefix(msg.level), msg.message);

  if (write_stdout) {
    std::cout << getLogLevelColor(msg.level) << str << "\033[0m";
  }

  if (write_file) {
    output_file << str;
  }
}

void Logger::log(LogLevel level, std::string_view msg) {
  if (level < verbosity) {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(messages_mtx);
    messages.push(Message{.message = std::string(msg),
                          .level = level,
                          .timestamp = std::chrono::system_clock::now()});
  }

  cv.notify_one();
}

std::string Logger::getLogLevelPrefix(LogLevel log_level) const {
  switch (log_level) {
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARNING:
    return "WARNING";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::CRITICAL:
    return "CRITICAL";
  }

  return "";
}

std::string Logger::getLogLevelColor(LogLevel log_level) const {
  switch (log_level) {
  case LogLevel::DEBUG:
    return "\033[37m";
  case LogLevel::INFO:
    return "\033[32m";
  case LogLevel::WARNING:
    return "\033[33m";
  case LogLevel::ERROR:
    return "\033[35m";
  case LogLevel::CRITICAL:
    return "\033[31m";
  }

  return "";
}

} // namespace engine::log
