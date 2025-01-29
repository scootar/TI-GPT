# TI-GPT
A project that leverages your ESP32 to enable your TI-84 family calculators to run Gemini-based applications.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

---

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/ExodowRRGB/TI-GPT.git
```

### 2. Open the Project in PlatformIO
Ensure you have PlatformIO installed and open the project from the cloned directory.

### 3. Flash the ESP32
Connect your ESP32 to your computer and upload the firmware using PlatformIO.

### 4. Wiring the 2.5mm Jack
Connect the following pins on the ESP32 to the corresponding sections of the 2.5mm audio jack:
- **Tip**: GPIO 22
- **Ring**: GPIO 23
- **Sleeve**: Ground

### 5. Transfer Programs to the Calculator
Use either **TILP** or **TI Connect** to send both programs located in the `TiFiles` folder to your calculator.

---

## Usage

### Configure Wi-Fi Settings
1. Edit the Wi-Fi program on your calculator.
2. Change `SSID` and `PASS` to match your Wi-Fi network.
   - **Note**: Since the TI-84 does not support lowercase letters, you must add a `0` for lowercase or a `1` for uppercase after each letter. For example, to enter `HelLo`, type:
     ```
     H1E0L0L1O0
     ```

### Run the Applications
1. Execute the Wi-Fi application on your calculator.
   - Wait for a response: either a **timeout** or a successful connection displaying `Connected IP: [local IP]`.
2. Run the **GPT** application.
3. Type your query and wait for the response.

---

## License
This project is licensed under the [GPL-3.0 License](LICENSE).

