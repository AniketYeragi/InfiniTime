#pragma once

#include <lvgl/lvgl.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include <cstdint>
#include "Screen.h"
#include "Symbols.h"
#include "components/motor/MotorController.h"
#include "displayapp/Messages.h"

#include "FreeRTOS.h"
#include "portmacro_cmsis.h"

namespace Pinetime {
  namespace Components {
    class LittleVgl;
  }
  namespace Applications {
    namespace Screens {
      enum class EmergencyTimerStates { Init, Running, Halted, Exit };
      enum class EmergencyTimerEvents { Start, Stop };
      class FallDetection : public Screen {
      public:
        FallDetection(DisplayApp* app,
                      Controllers::MotorController& motorController,
                      System::SystemTask& systemTask,
                      Pinetime::Components::LittleVgl& lvgl);

        ~FallDetection() override;

        bool Refresh() override;

        void cancelBtnEventHandler(lv_event_t event);

        bool OnTouchEvent(TouchEvents event) override;

      private:
        Pinetime::Components::LittleVgl& lvgl;
        Controllers::MotorController& motorController;
        Pinetime::System::SystemTask& systemTask;
        EmergencyTimerStates currentState;
        EmergencyTimerEvents currentEvent;
        TickType_t startTime;
        int currentTimeSeconds;

        lv_obj_t* imgFlag;
        lv_obj_t* txtNarrative;
        lv_obj_t* btnCancel;
        lv_obj_t* txtCancel;
        lv_obj_t* time;
      };
    }
  }
}
