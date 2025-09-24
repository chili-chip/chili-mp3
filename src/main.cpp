#include <Arduino.h>
#include <math.h>
#include "BluetoothA2DPSource.h"

// BluetoothA2DPSource instance
BluetoothA2DPSource a2dp_source;

#define HEADPHONES_NAME "WH-1000XM4"

// Audio parameters
#define SAMPLE_RATE 44100
#define CHANNELS 2
#define FRAME_SIZE (CHANNELS * sizeof(int16_t))

// Sine wave generation for demo
static double frequency_left = 440.0;   // A4 note
static double frequency_right = 523.25; // C5 note
static double phase_left = 0.0;
static double phase_right = 0.0;

// Provide audio data to A2DP source (stereo 16-bit signed PCM)
int32_t get_sound_data(uint8_t *data, int32_t len) {
  int16_t *samples = reinterpret_cast<int16_t*>(data);
  int sample_count = len / FRAME_SIZE;

  for (int i = 0; i < sample_count; i++) {
    double left_sample = sin(phase_left) * 0.3;  // volume scale
    double right_sample = sin(phase_right) * 0.3;
    samples[i * 2] = (int16_t)(left_sample * 32767);
    samples[i * 2 + 1] = (int16_t)(right_sample * 32767);

    phase_left += 2.0 * M_PI * frequency_left / SAMPLE_RATE;
    phase_right += 2.0 * M_PI * frequency_right / SAMPLE_RATE;

    if (phase_left > 2.0 * M_PI) phase_left -= 2.0 * M_PI;
    if (phase_right > 2.0 * M_PI) phase_right -= 2.0 * M_PI;
  }

  return len;
}

static void on_connection_state_changed(esp_a2d_connection_state_t state, void*) {
  switch (state) {
    case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
      Serial.println("[BT] Disconnected");
      break;
    case ESP_A2D_CONNECTION_STATE_CONNECTING:
      Serial.println("[BT] Connecting...");
      break;
    case ESP_A2D_CONNECTION_STATE_CONNECTED:
      Serial.println("[BT] Connected!");
      break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
      Serial.println("[BT] Disconnecting...");
      break;
  }
}

static void on_audio_state_changed(esp_a2d_audio_state_t state, void*) {
  switch (state) {
    case ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND:
      Serial.println("[BT] Audio suspended by remote");
      break;
    case ESP_A2D_AUDIO_STATE_STOPPED:
      Serial.println("[BT] Audio stopped");
      break;
    case ESP_A2D_AUDIO_STATE_STARTED:
      Serial.println("[BT] Audio started");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Starting ESP32 A2DP Source...");

  // callbacks
  a2dp_source.set_on_connection_state_changed(on_connection_state_changed);
  a2dp_source.set_on_audio_state_changed(on_audio_state_changed);

  // provide audio samples
  a2dp_source.set_data_callback(get_sound_data);

  // optional tweaks
  a2dp_source.set_volume(80);          // 0..127
  a2dp_source.set_auto_reconnect(true);

  // Start A2DP source
  // Option A: connect to a specific device name (replace HEADPHONES_NAME above)
  a2dp_source.start(HEADPHONES_NAME);

  // Option B: connect to any available A2DP device
  // a2dp_source.start();

  // Option C: try multiple device names
  // std::vector<char*> names = {"WH-1000XM4", "AirPods", "Bose"};
  // a2dp_source.start(names);

  Serial.println("A2DP Source started. Put your headphones in pairing mode if not already paired.");
}

void loop() {
  // keep things alive and provide a heartbeat
  static uint32_t last = 0;
  if (millis() - last > 1000) {
    last = millis();
    Serial.println(a2dp_source.is_connected() ? "Streaming..." : "Waiting for connection...");
  }
}
