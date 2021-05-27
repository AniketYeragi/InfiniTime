#pragma once

#include <lvgl/lvgl.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include <cstdint>
#include "Screen.h"

namespace Pinetime {
  namespace Components {
    class LittleVgl;
  }
  namespace Applications {
    namespace Screens {

      class FallDetection : public Screen {
      public:
        FallDetection(DisplayApp* app, Pinetime::Components::LittleVgl& lvgl);

        ~FallDetection() override;

        bool Refresh() override;

      private:
        Pinetime::Components::LittleVgl& lvgl;
        lv_obj_t* imgFlag;
      };
    }
  }
}
