#pragma once

template<int interval_ms>
class fixed_interval_timer {
  public:
    void reset() {
      next = millis() + interval_ms;
    }

    bool expired() {
      bool e = millis() > next;

      if (!e) {
        return false;
      }

      do {
        next += interval_ms;
      } while (next > millis());

      return true;
    }

  private:
    long next;
};
