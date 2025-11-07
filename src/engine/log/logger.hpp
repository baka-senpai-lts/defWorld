#ifndef LOGGER_H_
#define LOGGER_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

// What does MT mean? We will never know
#define MT_LOG_DEBUG(msg, ...)                                                 \
  if (engine::log::Logger::getInstance().getLogLevel() <=                      \
      engine::log::LogLevel::DEBUG)                                            \
  engine::log::Logger::getInstance().logDebug(                                 \
      fmt::v10::format(msg __VA_OPT__(, ) __VA_ARGS__))
#define MT_LOG_INFO(msg, ...)                                                  \
  if (engine::log::Logger::getInstance().getLogLevel() <=                      \
      engine::log::LogLevel::INFO)                                             \
  engine::log::Logger::getInstance().logInfo(                                  \
      fmt::v10::format(msg __VA_OPT__(, ) __VA_ARGS__))
#define MT_LOG_WARNING(msg, ...)                                               \
  if (engine::log::Logger::getInstance().getLogLevel() <=                      \
      engine::log::LogLevel::WARNING)                                          \
  engine::log::Logger::getInstance().logWarning(                               \
      fmt::v10::format(msg __VA_OPT__(, ) __VA_ARGS__))
#define MT_LOG_ERROR(msg, ...)                                                 \
  if (engine::log::Logger::getInstance().getLogLevel() <=                      \
      engine::log::LogLevel::ERROR)                                            \
  engine::log::Logger::getInstance().logError(                                 \
      fmt::v10::format(msg __VA_OPT__(, ) __VA_ARGS__))
#define MT_LOG_CRITICAL(msg, ...)                                              \
  if (engine::log::Logger::getInstance().getLogLevel() <=                      \
      engine::log::LogLevel::CRITICAL)                                         \
  engine::log::Logger::getInstance().logCritical(                              \
      fmt::v10::format(msg __VA_OPT__(, ) __VA_ARGS__))

namespace engine::log {

enum LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
  CRITICAL = 4,
  NONE = 5
};

class Logger {
private:
  struct Message {
    std::string message;
    LogLevel level;
    std::chrono::system_clock::time_point timestamp;

    // If this is set, it won't print the message, exists purely to make
    // changing file path work in async context
    bool change_file = false;
  };

  std::thread thread;

  std::queue<Message> messages;
  std::mutex messages_mtx;
  std::condition_variable cv;

  std::atomic<LogLevel> verbosity;
  std::filesystem::path file_path;
  std::ofstream output_file;

  std::atomic_bool write_stdout;
  std::atomic_bool write_file;
  std::atomic_bool die;

  Logger(LogLevel verbosity = INFO, bool write_stdout = true,
         bool write_file = true, std::filesystem::path file = "./log.txt")
      : verbosity(verbosity), file_path(file), write_stdout(write_stdout),
        write_file(write_file), die(false), thread(&Logger::run, this) {};

  std::string getLogLevelPrefix(LogLevel log_level) const;
  std::string getLogLevelColor(LogLevel log_level) const;

  void run();
  void printMessage(Message message);

public:
  Logger(const Logger &obj) = delete;
  void operator=(Logger const &) = delete;

  ~Logger();

  static Logger &getInstance();

  void setLogLevel(LogLevel verbosity) { this->verbosity = verbosity; };
  inline LogLevel getLogLevel() const { return verbosity; }

  void setFilePath(std::filesystem::path file);
  inline std::filesystem::path getFilePath() const { return file_path; }

  void setWriteStdout(bool write_stdout) { this->write_stdout = write_stdout; }
  inline bool getWriteStdout() const { return write_stdout; }

  void setWriteFile(bool write_file) { this->write_file = write_file; }
  inline bool getWriteFile() const { return write_file; }

  void log(LogLevel level, std::string_view message);

  inline void logDebug(std::string_view message) {
    log(LogLevel::DEBUG, message);
  }
  inline void logInfo(std::string_view message) {
    log(LogLevel::INFO, message);
  }
  inline void logWarning(std::string_view message) {
    log(LogLevel::WARNING, message);
  }
  inline void logError(std::string_view message) {
    log(LogLevel::ERROR, message);
  }
  inline void logCritical(std::string_view message) {
    log(LogLevel::CRITICAL, message);
  }
};

} // namespace engine::log

#endif // LOGGER_H_
