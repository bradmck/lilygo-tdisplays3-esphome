#ifndef TDISPLAY_S3
#define TDISPLAY_S3

#include "TFT_eSPI.h"
#include "esphome.h"

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"

namespace esphome {
namespace tdisplays3 {

static const char *const TAG = "tdisplays3";

class TDisplayS3 : public PollingComponent,
                   public display::DisplayBuffer
{
  public:
    void setup() override {
        tft.init();
        spr.setColorDepth(this->color_depth_);
        spr.createSprite(get_width_internal(), get_height_internal());
        tft.fillScreen(TFT_BLACK);
    }

    void loop() override {
    }

    void dump_config() {
        ESP_LOGCONFIG(TAG, "TDisplayS3:");
        setup_t tftSetup;
        tft.getSetup(tftSetup);            
        ESP_LOGCONFIG(TAG, "  TFT_eSPI Library Version: %s", tftSetup.version.c_str());
        ESP_LOGCONFIG(TAG, "  TFT Width: %d", tftSetup.tft_width);
        ESP_LOGCONFIG(TAG, "  TFT Height: %d", tftSetup.tft_height);
        ESP_LOGCONFIG(TAG, "  Color Depth: %d", spr.getColorDepth());

        int iTFTDriver = tftSetup.tft_driver;
        ESP_LOGCONFIG(TAG, "  TFT_eSPI Driver: %x", iTFTDriver);
        if(tftSetup.serial == 1) {
            ESP_LOGCONFIG(TAG, "  TFT_eSPI Interface: Serial");
        }
        else {
            ESP_LOGCONFIG(TAG, "  TFT_eSPI Interface: 8-bit Parallel");
        }

    }

    //////////
    // DisplayBuffer methods
    //////////
    void fill(Color color) override {
        spr.fillScreen(display::ColorUtil::color_to_565(color));
    }

    int get_width_internal() override {
        return tft.getViewportWidth();
    }

    int get_height_internal() override {
	return tft.getViewportHeight();
    }

    display::DisplayType get_display_type() override {
        return display::DisplayType::DISPLAY_TYPE_COLOR;
    }

    void draw_absolute_pixel_internal(int x, int y, Color color) override {
        spr.drawPixel(x, y, display::ColorUtil::color_to_565(color));
    }

    /////////////
    // PollingComponent Methods
    /////////////
    void update() override {
        this->do_update_();
        spr.pushSprite(0, 0);
    }

    void set_color_depth(int8_t color_depth) {
        this->color_depth_ = color_depth;
        spr.setColorDepth(color_depth);
    }

    private:
    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite spr = TFT_eSprite(&tft);

    protected:
    int8_t color_depth_;

};

}  // namespace tdisplays3
}  // namespace esphome

#endif
