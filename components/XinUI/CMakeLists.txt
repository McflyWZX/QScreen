idf_component_register(SRCS "XinPIC.cpp" "XinText.cpp" "XinWindow.cpp" "XinLayout.cpp" "XinUI.cpp"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES sdmmc fatfs XinCore sdCard)
