#include "FallDetection.h"
#include "../DisplayApp.h"
#include "../LittleVgl.h"
#include "FreeRTOSConfig.h"
#include "task.h"

using namespace Pinetime::Applications::Screens;

LV_FONT_DECLARE(lv_font_navi_80)

// Anonymous namespace for local functions
namespace {
  int convertTicksToTimeSegments(const TickType_t timeElapsed) {
    const int timeElapsedMillis = (static_cast<float>(timeElapsed) / static_cast<float>(configTICK_RATE_HZ)) * 1000;

    const int secs = (timeElapsedMillis / 1000) % 60;
    return 5-secs;
  }

  TickType_t calculateDelta(const TickType_t startTime, const TickType_t currentTime) {
    TickType_t delta = 0;
    // Take care of overflow
    if (startTime > currentTime) {
      delta = 0xffffffff - startTime;
      delta += (currentTime + 1);
    } else {
      delta = currentTime - startTime;
    }
    return delta;
  }
}

static void cancel_event_handler(lv_obj_t* obj, lv_event_t event) {
  auto fallDetection = static_cast<FallDetection*>(obj->user_data);
  fallDetection->cancelBtnEventHandler(event);
}

FallDetection::FallDetection(Pinetime::Applications::DisplayApp* app,
                              Controllers::MotorController& motorController)
 : Screen(app),
 currentState {EmergencyTimerStates::Init},
 currentEvent {EmergencyTimerEvents::Start},
 startTime {},
 motorController {motorController} {

  time = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
  lv_obj_set_style_local_text_color(time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text(time, "5");
  lv_label_set_align(time, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(time, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 60);

  txtNarrative = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(txtNarrative, LV_LABEL_LONG_BREAK);
  lv_obj_set_width(txtNarrative, LV_HOR_RES);
  lv_obj_set_style_local_text_color(txtNarrative, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_label_set_text(txtNarrative, "Emergency button");
  lv_label_set_align(txtNarrative, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(txtNarrative, nullptr, LV_ALIGN_CENTER, 0, 60);

  btnCancel = lv_btn_create(lv_scr_act(), nullptr);
  btnCancel->user_data = this;
  lv_obj_set_event_cb(btnCancel, cancel_event_handler);
  lv_obj_align(btnCancel, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 10);
  lv_obj_set_height(btnCancel, 40);
  txtCancel = lv_label_create(btnCancel, nullptr);
  lv_label_set_text(txtCancel, Symbols::phoneSlash);
}

FallDetection::~FallDetection() {
  lv_obj_clean(lv_scr_act());
}

bool FallDetection::Refresh() {
  switch (currentState) {
    // Init state when an user first opens the app
    // and when a stop/reset button is pressed
    case EmergencyTimerStates::Init: {
        startTime = xTaskGetTickCount();
        currentState = EmergencyTimerStates::Running;
      break;
    }
    case EmergencyTimerStates::Running: {
      if (currentEvent == EmergencyTimerEvents::Stop) {
        lv_obj_set_style_local_text_color(time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
        currentState = EmergencyTimerStates::Halted;
        break;
      }

      const auto timeElapsed = calculateDelta(startTime, xTaskGetTickCount());
      currentTimeSeconds = convertTicksToTimeSegments(timeElapsed);
      if (currentTimeSeconds < 0)
      {
        lv_obj_set_style_local_text_color(time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        lv_label_set_text(txtNarrative, "Emergency activated");
        currentState = EmergencyTimerStates::Halted;
        motorController.SetDuration(120);
      }
      else
      {
        lv_label_set_text_fmt(time, "%d", currentTimeSeconds);
      }
      break;
    }
    case EmergencyTimerStates::Halted: {
      break;
    }
  }
  return running;
}

void FallDetection::cancelBtnEventHandler(lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    currentEvent = EmergencyTimerEvents::Stop;
    motorController.SetDuration(30);
  }
}

