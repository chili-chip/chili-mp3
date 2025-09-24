# Chili MP3 - ESP32 Bluetooth A2DP Source

This project demonstrates how to use the ESP32 as a Bluetooth A2DP (Advanced Audio Distribution Profile) source to stream audio to Bluetooth headphones or speakers.

## Hardware Requirements

- ESP32 Development Board (ESP32-WROOM-32 or similar)
- Bluetooth headphones or speaker for testing

## Features

- **A2DP Source**: Streams audio to Bluetooth headphones/speakers
- **Sine Wave Generator**: Generates demo audio (left: 440Hz, right: 523.25Hz)
- **Connection Management**: Auto-reconnection and connection status monitoring
- **Configurable Volume**: Adjustable volume level (0-127)
- **Multiple Device Support**: Can connect to various Bluetooth audio devices

## Getting Started

### 1. Hardware Setup
Simply connect your ESP32 board to your computer via USB. No additional hardware is required for basic functionality.

### 2. Configure Your Headphones Name
In `src/main.c`, find this line:
```c
#define HEADPHONES_NAME "WH-1000XM4"
```

Replace `"YourHeadphonesName"` with the exact name of your Bluetooth headphones. You can find this name by:
- Pairing your headphones with your phone and checking the device name
- Common names include: "WH-1000XM4", "AirPods", "Beats Solo", "JBL", etc.

Alternatively, you can use one of these options:
- `a2dp_source.start();` - Connects to any available A2DP device
- Use the vector approach to try multiple device names

### 3. Build and Flash

```bash
# Build the project
pio build

# Upload to ESP32
pio upload

# Monitor serial output
pio monitor
```

### 4. Usage

1. **Put your headphones in pairing mode**
2. **Power on the ESP32**
3. **Watch the serial monitor** for connection status
4. **Listen for audio** - you should hear two-tone audio (left and right channels)

## Configuration Options

### Audio Settings
- **Sample Rate**: 44.1kHz (standard A2DP)
- **Channels**: Stereo (2 channels)
- **Volume**: Adjustable (0-127), default is 80

### Bluetooth Settings
- **Auto Reconnection**: Enabled by default
- **Connection Callbacks**: Monitor connection and audio states

## Customization

### Audio Source
The current implementation generates sine waves for demonstration. To play real audio:

1. **From SD Card**: Add SD card support and read audio files
2. **From Internet**: Add WiFi and stream from internet radio
3. **From Microphone**: Add I2S microphone input
4. **From Audio Files**: Store audio data in flash memory

### Example for Custom Audio:
```c
int32_t get_sound_data(uint8_t *data, int32_t len) {
    // Replace this with your audio source:
    // - Read from SD card
    // - Stream from internet
    // - Process microphone input
    // - Play stored audio files
    
    return len; // Return actual bytes written
}
```

## Troubleshooting

### Common Issues

**"Device not found"**:
- Ensure headphones are in pairing mode
- Check the device name is correct
- Try using `a2dp_source.start();` without device name

**"Connection failed"**:
- Check if headphones support A2DP profile
- Try resetting Bluetooth on headphones
- Ensure no other device is connected to headphones

**"No audio output"**:
- Check volume level (`a2dp_source.set_volume(80);`)
- Verify headphones support the audio codec (SBC)
- Check serial monitor for audio state messages

**"Build errors"**:
- Ensure all dependencies are installed
- Check platformio.ini configuration
- Clean and rebuild: `pio clean && pio build`

### Serial Monitor Output
Monitor the serial output at 115200 baud to see:
- Bluetooth initialization status
- Device discovery and connection attempts  
- Audio streaming status
- Error messages and debugging info

## Technical Details

### A2DP Profile
- **Role**: Source (sends audio)
- **Codec**: SBC (SubBand Codec)
- **Sample Rate**: 44.1kHz
- **Bit Depth**: 16-bit
- **Channels**: Stereo

### Memory Usage
- The project uses the large partition table for Bluetooth stack
- Main task stack size increased to 4KB
- Event task stack size set to 2.3KB

### Pin Usage
No external pins are used for basic Bluetooth functionality. All audio processing is done internally and transmitted via Bluetooth.

## Library Credits

This project uses:
- **ESP32-A2DP**: https://github.com/pschatzmann/ESP32-A2DP
- **Arduino Audio Tools**: https://github.com/pschatzmann/arduino-audio-tools

## License

This project is provided as-is for educational and development purposes.