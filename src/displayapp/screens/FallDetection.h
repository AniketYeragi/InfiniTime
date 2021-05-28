#pragma once

#include <lvgl/lvgl.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include <cstdint>
#include "Screen.h"
#include "Symbols.h"

namespace Pinetime {
  namespace Components {
    class LittleVgl;
  }
  namespace Applications {
    namespace Screens {
      enum class EmergencyTimerStates { Init, Running, Halted };
      enum class EmergencyTimerEvents { Start, Stop };
      class FallDetection : public Screen {
      public:
        FallDetection(DisplayApp* app);

        ~FallDetection() override;

        bool Refresh() override;

        void cancelBtnEventHandler(lv_event_t event);

      private:
        EmergencyTimerStates currentState;
        EmergencyTimerEvents currentEvent;
        TickType_t startTime;

        lv_obj_t* imgFlag;
        lv_obj_t* txtNarrative;
        lv_obj_t* btnCancel;
        lv_obj_t* txtCancel;
        lv_obj_t* time;
      };
    }
  }
}
