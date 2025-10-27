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
  localtime_r(&t, &tm);

  std::string str = fmt::v10::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}\n", tm,
                                     getLogLevelPrefix(msg.level), msg.message);

  if (write_stdout) {
    std::cout << str;
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
    messages.push(
        Message{std::string(msg), level, std::chrono::system_clock::now()});
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

} // namespace engine::log
