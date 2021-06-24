#include "SettingPineTimeStyle.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void event_handler(lv_obj_t * obj, lv_event_t event) {
    SettingPineTimeStyle* screen = static_cast<SettingPineTimeStyle *>(obj->user_data);
    screen->UpdateSelected(obj, event);
  }
}

lv_color_t pts_colors[17] = {LV_COLOR_WHITE, LV_COLOR_SILVER, LV_COLOR_GRAY, LV_COLOR_BLACK,
                             LV_COLOR_RED, LV_COLOR_MAROON, LV_COLOR_YELLOW, LV_COLOR_OLIVE,
                             LV_COLOR_LIME, LV_COLOR_GREEN, LV_COLOR_CYAN, LV_COLOR_TEAL,
                             LV_COLOR_BLUE, LV_COLOR_NAVY, LV_COLOR_MAGENTA, LV_COLOR_PURPLE,
                             LV_COLOR_ORANGE};

SettingPineTimeStyle::SettingPineTimeStyle(
  Pinetime::Applications::DisplayApp *app, Pinetime::Controllers::Settings &settingsController) :
  Screen(app),
  settingsController{settingsController}
{

  timebar = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(timebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_set_style_local_radius(timebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(timebar, 200, 240);
  lv_obj_align(timebar, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 5, 0);

  /* Display the time */

  timeDD1 = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(timeDD1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &open_sans_light);
  lv_obj_set_style_local_text_color(timeDD1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_TEAL);
  lv_label_set_text(timeDD1, "12");
  lv_obj_align(timeDD1, timebar, LV_ALIGN_IN_TOP_MID, 5, 5);

  timeDD2 = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(timeDD2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &open_sans_light);
  lv_obj_set_style_local_text_color(timeDD2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_TEAL);
  lv_label_set_text(timeDD2, "34");
  lv_obj_align(timeDD2, timebar, LV_ALIGN_IN_BOTTOM_MID, 5, -5);

  timeAMPM = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(timeAMPM, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_TEAL);
  lv_obj_set_style_local_text_line_space(timeAMPM, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, -3);
  lv_label_set_text(timeAMPM, "A\nM");
  lv_obj_align(timeAMPM, timebar, LV_ALIGN_IN_BOTTOM_LEFT, 2, -20);

  /* Create a 40px wide bar down the right side of the screen */

  sidebar = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(sidebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_TEAL);
  lv_obj_set_style_local_radius(sidebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(sidebar, 40, 240);
  lv_obj_align(sidebar, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  /* Display icons */

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_label_set_text(batteryIcon, Symbols::batteryFull);
  lv_obj_align(batteryIcon, sidebar, LV_ALIGN_IN_TOP_MID, 0, 2);

  bleIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(bleIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_label_set_text(bleIcon, Symbols::bluetooth);
  lv_obj_align(bleIcon, sidebar, LV_ALIGN_IN_TOP_MID, 0, 25);

  /* Calendar icon */

  calendarOuter = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(calendarOuter, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_radius(calendarOuter, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(calendarOuter, 34, 34);
  lv_obj_align(calendarOuter, sidebar, LV_ALIGN_CENTER, 0, 0);

  calendarInner = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(calendarInner, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffff));
  lv_obj_set_style_local_radius(calendarInner, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(calendarInner, 27, 27);
  lv_obj_align(calendarInner, calendarOuter, LV_ALIGN_CENTER, 0, 0);

  calendarBar1 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(calendarBar1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_radius(calendarBar1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(calendarBar1, 3, 12);
  lv_obj_align(calendarBar1, calendarOuter, LV_ALIGN_IN_TOP_MID, -6, -3);

  calendarBar2 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(calendarBar2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_radius(calendarBar2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(calendarBar2, 3, 12);
  lv_obj_align(calendarBar2, calendarOuter, LV_ALIGN_IN_TOP_MID, 6, -3);

  calendarCrossBar1 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(calendarCrossBar1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_radius(calendarCrossBar1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(calendarCrossBar1, 8, 3);
  lv_obj_align(calendarCrossBar1, calendarBar1, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  calendarCrossBar2 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(calendarCrossBar2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_radius(calendarCrossBar2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(calendarCrossBar2, 8, 3);
  lv_obj_align(calendarCrossBar2, calendarBar2, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  /* Display date */

  dateDayOfWeek = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(dateDayOfWeek, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_label_set_text(dateDayOfWeek, "THU");
  lv_obj_align(dateDayOfWeek, sidebar, LV_ALIGN_CENTER, 0, -34);

  dateDay = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(dateDay, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_label_set_text(dateDay, "25");
  lv_obj_align(dateDay, sidebar, LV_ALIGN_CENTER, 0, 3);

  dateMonth = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(dateMonth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_label_set_text(dateMonth, "MAR");
  lv_obj_align(dateMonth, sidebar, LV_ALIGN_CENTER, 0, 32);

  // Step count gauge
  needle_colors[0] = LV_COLOR_WHITE;
  stepGauge = lv_gauge_create(lv_scr_act(), nullptr);
  lv_gauge_set_needle_count(stepGauge, 1, needle_colors);
  lv_obj_set_size(stepGauge, 40, 40);
  lv_obj_align(stepGauge, sidebar, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_gauge_set_scale(stepGauge, 360, 11, 0);
  lv_gauge_set_angle_offset(stepGauge, 180);
  lv_gauge_set_critical_value(stepGauge, (100));
  lv_gauge_set_range(stepGauge, 0, (100));
  lv_gauge_set_value(stepGauge, 0, 0);

  lv_obj_set_style_local_pad_right(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, 3);
  lv_obj_set_style_local_pad_left(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, 3);
  lv_obj_set_style_local_pad_bottom(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, 3);
  lv_obj_set_style_local_line_opa(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_obj_set_style_local_scale_width(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, 4);
  lv_obj_set_style_local_line_width(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, 4);
  lv_obj_set_style_local_line_color(stepGauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_set_style_local_line_opa(stepGauge, LV_GAUGE_PART_NEEDLE, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_obj_set_style_local_line_width(stepGauge, LV_GAUGE_PART_NEEDLE, LV_STATE_DEFAULT, 4);
  lv_obj_set_style_local_pad_inner(stepGauge, LV_GAUGE_PART_NEEDLE, LV_STATE_DEFAULT, 4);
  
  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_click(backgroundLabel, true);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

//   lv_style_copy(&style_button, &lv_style_plain_color);
//   style_button.body.opa = LV_OPA_50;
  btnNextTime = lv_btn_create(lv_scr_act(), nullptr);
  btnNextTime->user_data = this;
  lv_obj_set_size(btnNextTime, 50, 50);
  lv_obj_align(btnNextTime, lv_scr_act(), LV_ALIGN_CENTER, 55, -80);
  lv_obj_set_style_local_value_str(btnNextTime, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, ">");
  lv_obj_set_event_cb(btnNextTime, event_handler);

  btnPrevTime = lv_btn_create(lv_scr_act(), nullptr);
  btnPrevTime->user_data = this;
  lv_obj_set_size(btnPrevTime, 50, 50);
  lv_obj_align(btnPrevTime, lv_scr_act(), LV_ALIGN_CENTER, -55, -80);
  lv_obj_set_style_local_value_str(btnPrevTime, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "<");
  lv_obj_set_event_cb(btnPrevTime, event_handler);

  btnNextBar = lv_btn_create(lv_scr_act(), nullptr);
  btnNextBar->user_data = this;
  lv_obj_set_size(btnNextBar, 50, 50);
  lv_obj_align(btnNextBar, lv_scr_act(), LV_ALIGN_CENTER, 55, 0);
  lv_obj_set_style_local_value_str(btnNextBar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, ">");
  lv_obj_set_event_cb(btnNextBar, event_handler);

  btnPrevBar = lv_btn_create(lv_scr_act(), nullptr);
  btnPrevBar->user_data = this;
  lv_obj_set_size(btnPrevBar, 50, 50);
  lv_obj_align(btnPrevBar, lv_scr_act(), LV_ALIGN_CENTER, -55, 0);
  lv_obj_set_style_local_value_str(btnPrevBar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "<");
  lv_obj_set_event_cb(btnPrevBar, event_handler);

  btnNextBG = lv_btn_create(lv_scr_act(), nullptr);
  btnNextBG->user_data = this;
  lv_obj_set_size(btnNextBG, 50, 50);
  lv_obj_align(btnNextBG, lv_scr_act(), LV_ALIGN_CENTER, 55, 80);
  lv_obj_set_style_local_value_str(btnNextBG, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, ">");
  lv_obj_set_event_cb(btnNextBG, event_handler);

  btnPrevBG = lv_btn_create(lv_scr_act(), nullptr);
  btnPrevBG->user_data = this;
  lv_obj_set_size(btnPrevBG, 50, 50);
  lv_obj_align(btnPrevBG, lv_scr_act(), LV_ALIGN_CENTER, -55, 80);
  lv_obj_set_style_local_value_str(btnPrevBG, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "<");
  lv_obj_set_event_cb(btnPrevBG, event_handler);

/*
    
  stepValue = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(stepValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
  lv_label_set_text_fmt(stepValue,"%i", settingsController.GetStepsGoal());
  lv_label_set_align(stepValue, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_CENTER, 0, -10);

  btnPlus = lv_btn_create(lv_scr_act(), nullptr);
  btnPlus->user_data = this;
  lv_obj_set_size(btnPlus, 80, 50);
  lv_obj_align(btnPlus, lv_scr_act(), LV_ALIGN_CENTER, 55, 80);
  lv_obj_set_style_local_value_str(btnPlus, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "+");
  lv_obj_set_event_cb(btnPlus, event_handler);

  btnMinus = lv_btn_create(lv_scr_act(), nullptr);
  btnMinus->user_data = this;
  lv_obj_set_size(btnMinus, 80, 50);
  lv_obj_set_event_cb(btnMinus, event_handler);
  lv_obj_align(btnMinus, lv_scr_act(), LV_ALIGN_CENTER, -55, 80);
  lv_obj_set_style_local_value_str(btnMinus, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "-");
*/
}

SettingPineTimeStyle::~SettingPineTimeStyle() {
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

bool SettingPineTimeStyle::Refresh() {
  return running;
}

void SettingPineTimeStyle::UpdateSelected(lv_obj_t *object, lv_event_t event) {
  uint8_t valueTime = settingsController.GetPTSColorTime();
  uint8_t valueBar = settingsController.GetPTSColorBar();
  uint8_t valueBG = settingsController.GetPTSColorBG();
    
  if((object == btnNextTime) && (event == LV_EVENT_PRESSED)) {
    if ( valueTime < 17 ) {
      valueTime += 1;
      settingsController.SetPTSColorTime(valueTime);
      lv_obj_set_style_local_text_color(timeDD1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueTime]);
      lv_obj_set_style_local_text_color(timeDD2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueTime]);
      lv_obj_set_style_local_text_color(timeAMPM, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueTime]);
    }
  }
  if((object == btnPrevTime) && (event == LV_EVENT_PRESSED)) {
    if ( valueTime > 0 ) {
      valueTime -= 1;
      settingsController.SetPTSColorTime(valueTime);
      lv_obj_set_style_local_text_color(timeDD1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueTime]);
      lv_obj_set_style_local_text_color(timeDD2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueTime]);
      lv_obj_set_style_local_text_color(timeAMPM, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueTime]);
    }
  }
  if((object == btnNextBar) && (event == LV_EVENT_PRESSED)) {
    if ( valueBar < 17 ) {
      valueBar += 1;
      settingsController.SetPTSColorBar(valueBar);
      lv_obj_set_style_local_bg_color(sidebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueBar]);
    }
  }
  if((object == btnPrevBar) && (event == LV_EVENT_PRESSED)) {
    if ( valueBar > 0 ) {
      valueBar -= 1;
      settingsController.SetPTSColorBar(valueBar);
      lv_obj_set_style_local_bg_color(sidebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueBar]);
    }
  }
  if((object == btnNextBG) && (event == LV_EVENT_PRESSED)) {
    if ( valueBG < 17 ) {
      valueBG += 1;
      settingsController.SetPTSColorBG(valueBG);
      lv_obj_set_style_local_bg_color(timebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueBG]);
    }
  }

  if((object == btnPrevBG) && (event == LV_EVENT_PRESSED)) {
    if ( valueBG > 0 ) {
      valueBG -= 1;
      settingsController.SetPTSColorBG(valueBG);
      lv_obj_set_style_local_bg_color(timebar, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, pts_colors[valueBG]);
    }
  }
    
    
    
/*    
  uint32_t value = settingsController.GetStepsGoal();
  if(object == btnPlus && (event == LV_EVENT_PRESSED)) {
    value += 1000;
    if ( value <= 500000 ) {
      settingsController.SetStepsGoal(value);
      lv_label_set_text_fmt(stepValue,"%i", settingsController.GetStepsGoal());
      lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_CENTER, 0, -10);
    }
  }

  if(object == btnMinus && (event == LV_EVENT_PRESSED)) {
    value -= 1000;
    if ( value >= 1000 ) {
      settingsController.SetStepsGoal(value);
      lv_label_set_text_fmt(stepValue,"%i", settingsController.GetStepsGoal());
      lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_CENTER, 0, -10);
    }
  }
*/
}