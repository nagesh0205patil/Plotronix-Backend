// Entry point for PlatformIO/Arduino framework
#include "core/app_manager.h"

void setup() {
  AppManager::getInstance().init();
}

void loop() {
  AppManager::getInstance().update();
}
