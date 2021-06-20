#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "WatchFaceBase.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
    class DateTimeController;
  }
  namespace Applications {
    class DisplayApp;

    namespace Screens {

      class WatchFaceAnalog : public WatchFaceBase {
      public:
        WatchFaceAnalog(DisplayApp* app,
                        Controllers::DateTimeController const& dateTimeController,
                        Controllers::Battery const& batteryController,
                        Controllers::Ble& bleController,
                        Controllers::NotificationManager& notificationManager,
                        Controllers::Settings& settingsController);

        ~WatchFaceAnalog() override;

        bool Refresh() override;

      private:
        DirtyValue<uint8_t> hour{99};
        DirtyValue<uint8_t> minute{99};
        DirtyValue<uint8_t> second{99};

        DirtyValue<bool> notificationState {false};

        lv_obj_t* hour_body;
        lv_obj_t* hour_body_trace;
        lv_obj_t* minute_body;
        lv_obj_t* minute_body_trace;
        lv_obj_t* second_body;

        // ##
        lv_point_t hour_point[2];
        lv_point_t hour_point_trace[2];
        lv_point_t minute_point[2];
        lv_point_t minute_point_trace[2];
        lv_point_t second_point[2];

        // ##
        lv_style_t hour_line_style;
        lv_style_t hour_line_style_trace;
        lv_style_t minute_line_style;
        lv_style_t minute_line_style_trace;
        lv_style_t second_line_style;

        lv_obj_t* label_date_day;
        lv_obj_t* batteryIcon;
        lv_obj_t* notificationIcon;

        Controllers::Ble& bleController;
        Controllers::NotificationManager& notificationManager;
        Controllers::Settings& settingsController;

        void UpdateClock();
      };
    }
  }
}
